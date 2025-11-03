import numpy as np
import cv2
import os
import json
import sys
import time
from matplotlib import pyplot as plt

#BORDER_TOP = int(os.environ['BORDER_TOP'])
#BORDER_BOTTOM = int(os.environ['BORDER_BOTTOM'])
#OFFSET = int(os.environ['OFFSET'])
THRESHOLD = 30

with open('offsets.conf', 'r') as f:
    offset_data = f.read()

offsets = json.loads(offset_data)
#print(offsets)

OFFSET = int(offsets['OFFSET'])
BORDER_TOP = int(offsets['BORDER_TOP'])
BORDER_BOTTOM = int(offsets['BORDER_BOTTOM'])

IMAGE_PATH = 'screen_capture.png'
SHOW_PLOT = False

if len(sys.argv) > 1:
    IMAGE_PATH = sys.argv[1]

if len(sys.argv) > 2:
    try:
        SHOW_PLOT = bool(sys.argv[2])
    except:
        pass

# HACK: wait a bit so the file is saved for sure
# failed a few times without the wait in debug mode (run_camera.py saving the image twice)
time.sleep(0.1)

img = cv2.imread(IMAGE_PATH, cv2.IMREAD_GRAYSCALE)
#img = cv2.imread('test_bright_screen.png', cv2.IMREAD_GRAYSCALE)

#print(img.shape)

## set offset to 0 if camera is in line scan mode
#OFFSET = 0
#line = img[OFFSET,BORDER_BOTTOM:BORDER_TOP]

# sum up multiple lines to increase dynamic range and reduce influence of noise and moiree
line = np.sum(img[OFFSET:OFFSET+5,BORDER_BOTTOM:BORDER_TOP], axis=0)
#print(len(line))
#line = np.flip(line)

#print(len(line))

deriv = []

for x in range(len(line)):
    if x == 0:
        continue

    dx = int(line[x]) - line[x-1]
    deriv.append(dx)

#print(deriv)

#plt.plot(line)
#plt.plot(deriv)
#ax = plt.gca()


# if left side of the monitor is on top of the camera frame:
# use argmin() on derivative and invert result (1 - (pos / len))
# because the white part of the teared screen is on the left hand side
# therefore we need to find a drop in brightness from white to black
# alternative would be to invert the image

#print(np.argmax(deriv), np.argmin(deriv))
#position = np.argmax(deriv)
position = np.argmin(deriv)
#print('position', position)

if abs(deriv[position]) > THRESHOLD:
    #plt.axvline(position, color='r', dashes=(1, 4))
    # position is percent from top to bottom, therefore we have to invert it
    print(1 - (position / len(deriv)))
else:
    print(0.0)

if SHOW_PLOT:
    plt.plot(line)
    plt.plot(deriv)
    plt.imshow(img[:,BORDER_BOTTOM:BORDER_TOP])
    ax = plt.gca()
    ax.axvline(position, color='black', dashes=(2, 4))
    ax.set_ylim(img.shape[0], -200)
    plt.show()
