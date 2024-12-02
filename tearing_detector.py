import numpy as np
import cv2
import os
import json
import sys

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

if len(sys.argv) > 1:
    IMAGE_PATH = sys.argv[1]

img = cv2.imread(IMAGE_PATH, cv2.IMREAD_GRAYSCALE)
#img = cv2.imread('test_bright_screen.png', cv2.IMREAD_GRAYSCALE)

#print(img.shape)

line = img[OFFSET,BORDER_BOTTOM:BORDER_TOP]
line = np.flip(line)

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

#print(np.argmax(deriv), np.argmin(deriv))
position = np.argmax(deriv)
#print('position', position)

if deriv[position] > THRESHOLD:
    #plt.axvline(position, color='r', dashes=(1, 4))
    print((position / len(deriv)))
else:
    print(0.0)
