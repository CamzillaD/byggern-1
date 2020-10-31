import serial
from time import sleep

def message(key, value):
    return bytes([0x02, key, value, ((key + value) & 0xff)])

while True:
    with serial.Serial("/dev/ttyACM0", 9600) as s:
        s.write(message(0x41, 0x42))
        sleep(1)
        s.write(message(0x42, 0x41))
        sleep(1)
