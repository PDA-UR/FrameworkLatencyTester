import serial
import time
import sys
import os

if len(sys.argv) > 1:
    n = int(sys.argv[1])
else:
    n = 10

#ser = serial.Serial('/dev/ttyACM0')  # open serial port
ser = serial.Serial('/dev/ttyUSB0')  # open serial port
ser.flushInput()

time.sleep(1)

ser.write('c'.encode())

ser_bytes = ser.readline()
decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
#print(decoded_bytes)

refresh_rates = [30, 60, 120, 165, 240, 255, 300, 360, 480]

for r in refresh_rates:
    os.system(f'xrandr --output DP-4 --mode 1920x1080 --refresh {r}')
    time.sleep(1)

    for i in range(n):
        ser.write('d'.encode())
        ser_bytes = ser.readline()
        decoded_bytes = ser_bytes[0:len(ser_bytes)-2].decode("utf-8")
        print(r, decoded_bytes)
        time.sleep(1)

ser.close()
