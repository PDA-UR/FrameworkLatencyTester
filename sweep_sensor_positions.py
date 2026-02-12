#!/usr/bin/python3

import pygame
import sys
import time
import serial
import numpy as np
import pandas as pd

ser = serial.Serial('/dev/ttyUSB0')  # open serial port
ser.flushInput()

time.sleep(1)

(width, height) = (1920,1080)
running = True
screen = pygame.display.set_mode((width, height), pygame.FULLSCREEN)

background_color_black = (0,0,0)
color_white = (255,255,255)

screen.fill(background_color_black)

pygame.display.flip()

line_width = 1


y_values = list(range(0, 80)) + list(range(500, 580)) + list(range(1000, 1080))

lines = []

#for y in range(height):
#for y in np.arange(0, height, 10):
for line_width in [1, 3, 5]:
    for y in y_values:
        event = pygame.event.poll()
        if event.type == pygame.QUIT:
            running = False
        if not running:
            break
        
        screen.fill(background_color_black)
        pygame.draw.line(screen, color_white, (0, y), (width, y), line_width)
        
        pygame.display.update()

        time.sleep(0.02)
        ser.write('b'.encode())
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
        substrings = decoded_bytes.split(',')
        top = int(substrings[0][3:])
        center = int(substrings[1][3:])
        bottom = int(substrings[2][3:])
        print(y, top, center, bottom)
        #print(decoded_bytes)
        line = {'y' : y, 'linewidth' : line_width, 'top' : top, 'center' : center, 'bottom' : bottom}
        lines.append(line)

ser.close()

df = pd.DataFrame(lines)
df.to_csv('sensor_positions.csv')
