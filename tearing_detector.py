import numpy as np
import cv2
import os
import json
import sys
import time
from matplotlib import pyplot as plt
from matplotlib import patches as patches

#BORDER_TOP = int(os.environ['BORDER_TOP'])
#BORDER_BOTTOM = int(os.environ['BORDER_BOTTOM'])
#OFFSET = int(os.environ['OFFSET'])
THRESHOLD = 20
WINDOW_SIZE = 5

# 257 3876
OFFSET = 0
#BORDER_TOP = 257
#BORDER_BOTTOM = 3876
BORDER_BOTTOM = 257
BORDER_TOP = 3876

try:
    with open('offsets.conf', 'r') as f:
        offset_data = f.read()
    offsets = json.loads(offset_data)
    BORDER_BOTTOM = int(offsets['BORDER_BOTTOM'])
    BORDER_TOP = int(offsets['BORDER_TOP'])
except:
    pass

IMAGE_PATH = 'screen_capture.png'
SHOW_PLOT = False

if len(sys.argv) > 1:
    IMAGE_PATH = sys.argv[1]

if len(sys.argv) > 2:
    try:
        SHOW_PLOT = bool(sys.argv[2])
    except:
        pass

img = cv2.imread(IMAGE_PATH, cv2.IMREAD_GRAYSCALE)
#img = cv2.imread('test_bright_screen.png', cv2.IMREAD_GRAYSCALE)


# sum up multiple lines to increase dynamic range and reduce influence of noise and moiree
line = np.mean(img[OFFSET:OFFSET+7,BORDER_BOTTOM:BORDER_TOP], axis=0)
#print(len(line))
#line = np.flip(line)

filter_size = 7
filter = np.ones(filter_size) / filter_size
line = np.convolve(line, filter)
line = line[filter_size:-filter_size]

#print('line length:', len(line))
#print(type(line))
#print(list(line.astype(int)))

deriv = []

for x in range(len(line)):
    if x == 0:
        continue

    dx = int(line[x]) - line[x-1]
    deriv.append(dx)


# if left side of the monitor is on top of the camera frame:
# use argmin() on derivative and invert result (1 - (pos / len))
# because the white part of the teared screen is on the left hand side
# therefore we need to find a drop in brightness from white to black
# alternative would be to invert the image

#print(np.argmax(deriv), np.argmin(deriv))
#position = np.argmax(deriv)
position = np.argmin(deriv)
#print('position', position)


try:
    window = line[position - WINDOW_SIZE : position + WINDOW_SIZE]
    difference = np.abs(min(window) - max(window))
    #print(difference)
    if difference > THRESHOLD:
        print(1 - (position / len(deriv)))
    else:
        print(0.0)
except:
    print(0.0)

#with open('tearing_capture_raw.dat', 'wb') as f:
#    f.write(bytearray(list(line.astype(int))))

cv2.imwrite('tearing_capture_line.png', cv2.transpose(line))
#print(line.shape)
#print(line.T.shape)


"""
if abs(deriv[position]) > THRESHOLD:
    #plt.axvline(position, color='r', dashes=(1, 4))
    # position is percent from top to bottom, therefore we have to invert it
    print(1 - (position / len(deriv)))
else:
    print(0.0)
"""

if SHOW_PLOT:
    fig, axes = plt.subplots(2, 1, figsize=(10, 5), sharex=True)
    #axes[0].axvline(position, linewidth = (1 + 2 * WINDOW_SIZE), color='gray', alpha=0.2)

    try:
        window = line[position - WINDOW_SIZE : position + WINDOW_SIZE]
        difference = np.abs(min(window) - max(window))
        #axes[0].plot((position-50, position-50), (min(window), max(window)), marker='o', markersize=4, color='black')
        #axes[0].plot((position, position), (line[position] - (THRESHOLD / 2), line[position] + (THRESHOLD / 2)), marker='o', markersize=4, color='red')
        axes[0].plot((position, position), (line[position] - (THRESHOLD / 2), line[position] + (THRESHOLD / 2)), linewidth=WINDOW_SIZE, color='black', alpha=0.5)
        #axes[0].plot((position, position), (min(window), max(window)), marker='_', markersize=8, color='red', linewidth=0)
        axes[0].plot((position, position), (min(window), max(window)), color='red', linewidth=WINDOW_SIZE, alpha=0.5)
        #axes[0].add_patch(line_range)
        #print('added line')
    except Exception as e:
        #print(e)
        pass

    axes[0].axvline(position, color='black', dashes=(2, 4))
    axes[0].plot(line)
    axes[0].plot(deriv)


    axes[1].imshow(img[:,BORDER_BOTTOM:BORDER_TOP], 'gray')
    axes[1].axhline(3, linewidth=6, color='red', alpha=0.3)
    #ax = plt.gca()
    #ax.set_ylim(img.shape[0], -200)
    plt.savefig('screen_tearing_detection.pdf', dpi=300, bbox_inches='tight')
    plt.show()
