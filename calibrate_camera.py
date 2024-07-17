import serial
import time
from run_camera import init_camera, acquire_images
import os

OFFSET = int(os.environ['OFFSET'])

ser = serial.Serial('/dev/ttyACM0')  # open serial port
ser.flushInput()

time.sleep(1)

# run test program


# init camera
cam, system = init_camera()

# click
ser.write('t'.encode())
time.sleep(1)

# capture image

cam.BeginAcquisition()
ser.write('p'.encode())
time.sleep(1)
image = cam.GetNextImage()

# release click
ser.write('t'.encode())


# grab image
image.Save(f'test_bright_screen.png')
img = image.GetNDArray()


# find screen borders
line = img[OFFSET,:]

deriv = []

for x in range(len(line)):
    if x == 0:
        continue

    dx = int(line[x]) - line[x-1]
    deriv.append(dx)

rise = np.argmax(deriv)
fall = np.argmin(deriv)

print(rise, fall)

os.environ['BORDER_TOP'] = rise
os.environ['BORDER_BOTTOM'] = fall

# kill test program
ser.close()
del cam
system.ReleaseInstance()
