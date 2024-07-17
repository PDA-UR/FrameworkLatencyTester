import numpy as np
import cv2
import os

BORDER_TOP = int(os.environ['BORDER_TOP'])
BORDER_BOTTOM = int(os.environ['BORDER_BOTTOM'])
OFFSET = int(os.environ['OFFSET'])
THRESHOLD = 50

img = cv2.imread('screen_capture.png', cv2.IMREAD_GRAYSCALE)

line = img[OFFSET,BORDER_TOP:BORDER_BOTTOM]

deriv = []

for x in range(len(line)):
    if x == 0:
        continue

    dx = int(line[x]) - line[x-1]
    deriv.append(dx)

#plt.plot(line)
#plt.plot(deriv)
#ax = plt.gca()
position = np.argmax(deriv)

if deriv[position] > THRESHOLD:
    #plt.axvline(position, color='r', dashes=(1, 4))
    print(position / len(deriv))
else:
    print(0.0)
