import time
import serial.tools.list_ports
import serial
import signal # For trapping ctrl-c or SIGINT
import sys # For exiting program with exit code

def SIGINT_handler(signal, frame):
        print('Quitting program!')
        ser.close()
        sys.exit(0)

signal.signal(signal.SIGINT, SIGINT_handler)

print("Running time.py...")

signal.signal(signal.SIGINT, SIGINT_handler)

ser=serial.Serial("COM3",baudrate=9600,timeout=10)

while(1):
    mycmd=ser.read()
    if (len(mycmd)>0):
        epoch=int(time.time())
        ser.write(str(epoch).encode())
        ser.write(b'\n')
		print(str(epoch))