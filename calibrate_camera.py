import serial
import time
from run_camera import init_camera, acquire_images
import os
import numpy as np
#import subprocess
import json

with open('offsets.conf', 'r') as f:
    offset_data = f.read()

offsets = json.loads(offset_data)

OFFSET = int(offsets['OFFSET'])

#try:
#    OFFSET = int(os.environ['OFFSET'])
#except:
#    OFFSET = 650
#    os.environ['OFFSET'] = str(OFFSET)

ser = serial.Serial('/dev/ttyUSB0')  # open serial port
ser.flushInput()

time.sleep(1)

# run test program


# init camera
cam, system = init_camera()

# click
print('click')
ser.write('t'.encode())
time.sleep(1)

# capture image

print('capture image')
cam.BeginAcquisition()
print('send p to yalmd')
ser.write('p'.encode())
time.sleep(1)
print('getNextImage')
image = cam.GetNextImage()

# release click
print('release')
ser.write('t'.encode())


# grab image
print('grab image')
image.Save(f'test_bright_screen.png')
img = image.GetNDArray()


# find screen borders
print('find borders')
line = img[OFFSET,:]
print(len(line))

deriv = []

for x in range(len(line)):
    if x == 0:
        continue

    dx = int(line[x]) - line[x-1]
    deriv.append(dx)

rise = np.argmax(deriv)
fall = np.argmin(deriv)

print(rise, fall)

# in the image, left is bottom and right is top
# therefore, rise is bottom and fall is top
#os.environ['BORDER_TOP'] = str(fall)
#os.environ['BORDER_BOTTOM'] = str(rise)
#os.system(f'export BORDER_TOP={fall}')
#os.system(f'export BORDER_BOTTOM={rise}')

offsets['BORDER_TOP'] = int(fall)
offsets['BORDER_BOTTOM'] = int(rise)

with open('offsets.conf', 'w') as f: 
    f.write(json.dumps(offsets))

# kill test program
ser.close()
del cam
system.ReleaseInstance()
