import serial

print("Hello")

ser = serial.Serial()
ser.baudrate = 9600
ser.port = '/dev/cu.usbmodem141301'

# TODO, get specific usb device info.

ser.open()



for i in range(5):
    ser.write(i.to_bytes(2, 'little'))

while True:
    if (ser.in_waiting):
        print(ser.readline())

