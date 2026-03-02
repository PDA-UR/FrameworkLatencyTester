import time
import PySpin
import cv2
import sys

NUM_IMAGES = 0

# run with argument to capture set amount of images
# run without arguments to capture images indefinitely
if len(sys.argv) > 1:
    try:
        NUM_IMAGES = int(sys.argv[1])
    except:
        NUM_IMAGES = 0

def acquire_images(cam, n):
    try:
        images = []

        #for i in range(n):
        counter = 0

        while True:
            cam.BeginAcquisition()
            print('acquire_images() 1')
            image = cam.GetNextImage()
            print('acquire_images() 2')
            if image.IsIncomplete():
                print('Warning: image {} incomplete'.format(image.GetFrameID()))
                continue
            print('acquire_images() 3')
            #image.Save(f'img_{i}.png')
            image.Save(f'test_images_2/screen_capture_{counter}.png')
            print('acquire_images() 4')
            #image_data = image.GetNDArray()
            #cv2.imwrite(f'img_{i}.png', image_data)

            print(f'capture image {counter}')
            cam.EndAcquisition()

            counter += 1

            if NUM_IMAGES != 0:
                if counter > NUM_IMAGES:
                    break

    except PySpin.SpinnakerException as ex:
        print('ERROR:', ex)
    except Exeption as e:
        print(e)

def init_camera():
    system = PySpin.System.GetInstance()
    print('init_camera() 1')
    cam = system.GetCameras()[0]
    print('init_camera() 2')
    cam.Init()
    print('init_camera() 3')
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
    cam.Gain.SetValue(6) #4
    print('init_camera() 4')
    #cam.TriggerDelay.SetValue(0)
    #cam.TriggerMode.SetValue(PySpin.TriggerMode_On)
    return cam, system

def main():
    cam, system = init_camera()
    print('camera initialized')

    acquire_images(cam, NUM_IMAGES)
    print('images acquired')

    # Clean up
    del cam
    system.ReleaseInstance()
    print('camera released')

if __name__ == '__main__':
    main()
