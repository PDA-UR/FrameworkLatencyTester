import serial
import time
import sys

wait_time = int(sys.argv[1])

ser = serial.Serial('/dev/ttyUSB0')
ser.flushInput()

time.sleep(wait_time)

ser.write('o'.encode())
time.sleep(0.1)
ser.write('o'.encode())
time.sleep(0.1)

ser.close()
