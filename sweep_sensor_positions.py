#!/usr/bin/python3

import pygame
import sys
import time
import serial

ser = serial.Serial('/dev/ttyACM0')  # open serial port
ser.flushInput()

time.sleep(1)

(width, height) = (1920,1080)
running = True
screen = pygame.display.set_mode((width, height), pygame.FULLSCREEN)

background_color_black = (0,0,0)
color_white = (255,255,255)

screen.fill(background_color_black)

pygame.display.flip()

line_width = 3

for y in range(height):
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
    print(decoded_bytes)
