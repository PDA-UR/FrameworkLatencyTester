import time
import PySpin
import cv2
import sys

NUM_IMAGES = 10

if len(sys.argv) > 1:
    try:
        NUM_IMAGES = int(sys.argv[1])
    except:
        NUM_IMAGES = 10

def acquire_images(cam):
    try:
        images = []

        for i in range(NUM_IMAGES):
            cam.BeginAcquisition()
            image = cam.GetNextImage()
            if image.IsIncomplete():
                print('Warning: image {} incomplete'.format(image.GetFrameID()))
                continue
            image.Save(f'img_{i}.png')
            #image_data = image.GetNDArray()
            #cv2.imwrite(f'img_{i}.png', image_data)

            print(f'capture image {i}')
            cam.EndAcquisition()

    except PySpin.SpinnakerException as ex:
        print('ERROR:', ex)

def init_camera(cam):
    cam.Init()
    #cam.TriggerMode.SetValue(PySpin.TriggerMode_Off)
    #cam.AcquisitionMode.SetValue(PySpin.AcquisitionMode_Continuous)#SingleFrame)
    cam.AcquisitionMode.SetValue(PySpin.AcquisitionMode_SingleFrame)
    cam.ExposureMode.SetValue(PySpin.ExposureMode_TriggerWidth) # Timed
    cam.TriggerSelector.SetValue(PySpin.TriggerSelector_FrameStart)
    cam.TriggerSource.SetValue(PySpin.TriggerSource_Line3)
    #cam.TriggerOverlap.SetValue(PySpin.TriggerOverlap_Off)
    cam.TriggerActivation.SetValue(PySpin.TriggerActivation_LevelLow)#FallingEdge
    cam.ExposureAuto.SetValue(PySpin.ExposureAuto_Off)
    #cam.ExposureTime.SetValue(float(100)) #250
    cam.GainAuto.SetValue(PySpin.GainAuto_Off)
    cam.Gain.SetValue(4)
    #cam.TriggerDelay.SetValue(0)
    #cam.TriggerMode.SetValue(PySpin.TriggerMode_On)

def main():
    system = PySpin.System.GetInstance()
    cam = system.GetCameras[0]

    init_camera(cam)

    acquire_images(cam)

    # Clean up
    del cam
    system.ReleaseInstance()

if __name__ == '__main__':
    main()
