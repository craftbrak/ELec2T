import serial, sys
import serial.tools.list_ports


ser = serial.Serial('COM4', baudrate=9600, timeout=1)


def fct():
    while True:
        msg = ser.readline()
        if len(msg) != 0:
            msg.split("*")
            print(msg)


if __name__ == '__main__':
    fct()
