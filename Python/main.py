import serial, sys
import serial.tools.list_ports


ser = serial.Serial('COM4', baudrate=9600, timeout=1)


def fct():
    while True:
        msg = ser.readline().decode('ascii')
        if len(msg) != 0:
            msg_split = msg.split("*")
            print("Nombre de personne à l'intérieur : " + msg_split[1] + " " + msg_split[2])


if __name__ == '__main__':
    fct()
