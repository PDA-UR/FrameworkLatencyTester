import numpy as np
import cv2
import os
import json

#BORDER_TOP = int(os.environ['BORDER_TOP'])
#BORDER_BOTTOM = int(os.environ['BORDER_BOTTOM'])
#OFFSET = int(os.environ['OFFSET'])
THRESHOLD = -50

with open('offsets.conf', 'r') as f:
    offset_data = f.read()

offsets = json.loads(offset_data)
#print(offsets)

OFFSET = int(offsets['OFFSET'])
BORDER_TOP = int(offsets['BORDER_TOP'])
BORDER_BOTTOM = int(offsets['BORDER_BOTTOM'])

img = cv2.imread('screen_capture.png', cv2.IMREAD_GRAYSCALE)
#img = cv2.imread('test_bright_screen.png', cv2.IMREAD_GRAYSCALE)

#print(img.shape)

line = img[OFFSET,BORDER_BOTTOM:BORDER_TOP]

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
position = np.argmin(deriv)
#print('position', position)

if deriv[position] < THRESHOLD:
    #plt.axvline(position, color='r', dashes=(1, 4))
    print(1 - (position / len(deriv)))
else:
    print(0.0)
