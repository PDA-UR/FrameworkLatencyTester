import time
import PySpin
import cv2
import sys
import shutil

NUM_IMAGES = 0

# run with argument to capture set amount of images
# run without arguments to capture images indefinitely
if len(sys.argv) > 1:
    try:
        NUM_IMAGES = int(sys.argv[1])
    except:
        NUM_IMAGES = 0

def acquire_images(cam, n):
    print('acquire_images()')
    try:
        images = []

        #for i in range(n):
        counter = 0

        while True:
            cam.BeginAcquisition()
            image = cam.GetNextImage()
            if image.IsIncomplete():
                print('Warning: image {} incomplete'.format(image.GetFrameID()))
                continue
            #image.Save(f'img_{i}.png')
            image.Save(f'screen_capture.png')
            if NUM_IMAGES != 0:
                #print('save image')
                #image_data = image.GetNDArray()
                #cv2.imwrite(f'test_images_2/img_{counter}.png', image_data)
                #image.Save(f'test_images_2/img_{counter}.png')
                shutil.copyfile('screen_capture.png', f'test_images_2/img_{counter:03}.png')
            #image_data = image.GetNDArray()
            #cv2.imwrite(f'img_{i}.png', image_data)

            print(f'capture image {counter}')
            cam.EndAcquisition()

            counter += 1

            if NUM_IMAGES != 0:
                #print('save image')
                #image_data = image.GetNDArray()
                #cv2.imwrite(f'test_images_2/img_{counter}.png', image_data)
                #image.Save(f'test_images_2/img_{counter}.png')
                if counter > NUM_IMAGES:
                    break

    except PySpin.SpinnakerException as ex:
        print('ERROR:', ex)
    except Exception as ex:
        print('Exception:', ex)

def init_camera():
    system = PySpin.System.GetInstance()
    cam = system.GetCameras()[0]
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
    cam.Gain.SetValue(2) #4
    #cam.SensorShutterMode.SetValue(PySpin.SensorShutterMode_Global)
    #cam.TriggerDelay.SetValue(0)
    #cam.TriggerMode.SetValue(PySpin.TriggerMode_On)
    return cam, system

def init_camera_4k():
    system = PySpin.System.GetInstance()
    cam = system.GetCameras()[0]
    cam.Init()
    #cam.TriggerMode.SetValue(PySpin.TriggerMode_Off)
    #cam.AcquisitionMode.SetValue(PySpin.AcquisitionMode_Continuous)#SingleFrame)
    cam.AcquisitionMode.SetValue(PySpin.AcquisitionMode_SingleFrame)
    cam.ExposureMode.SetValue(PySpin.ExposureMode_TriggerWidth) # Timed
    cam.TriggerSelector.SetValue(PySpin.TriggerSelector_FrameStart)
    cam.TriggerSource.SetValue(PySpin.TriggerSource_Line3)
    #cam.TriggerOverlap.SetValue(PySpin.TriggerOverlap_Off)
    #cam.TriggerOverlap.SetValue(PySpin.TriggerOverlap_ReadOut)
    cam.TriggerActivation.SetValue(PySpin.TriggerActivation_LevelLow)#FallingEdge
    cam.ExposureAuto.SetValue(PySpin.ExposureAuto_Off)
    #cam.ExposureTime.SetValue(float(100)) #250
    cam.GainAuto.SetValue(PySpin.GainAuto_Off)
    cam.Gain.SetValue(12) #10
    # switch camera to line mode
    #cam.Height.SetValue(100)
    #cam.OffsetY.SetValue(2600)
    #cam.Height.SetValue(600)
    #cam.OffsetY.SetValue(2300)
    cam.OffsetY.SetValue(0)
    cam.Height.SetValue(600)
    #cam.PixelFormat.SetValue(PySpin.PixelFormat_Mono8)
    cam.SensorShutterMode.SetValue(PySpin.SensorShutterMode_GlobalReset)
    #cam.SensorShutterMode.SetValue(PySpin.SensorShutterMode_Rolling)
    cam.PixelFormat.SetValue(PySpin.PixelFormat_BayerRG8)
    #cam.PixelFormat.SetValue(PySpin.PixelFormat_RGB8)
    #cam.TriggerDelay.SetValue(0)
    #cam.TriggerMode.SetValue(PySpin.TriggerMode_On)
    return cam, system

def main():
    #cam, system = init_camera()
    cam, system = init_camera_4k()

    acquisition_rate_hz = cam.AcquisitionLineRate.GetValue()
    print('acquisition rate:', acquisition_rate_hz)

    acquire_images(cam, NUM_IMAGES)

    # Clean up
    del cam
    system.ReleaseInstance()

if __name__ == '__main__':
    main()
