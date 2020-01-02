from time import sleep
from serial import *
# these codes are for bluetooth
# hint: please check the function "sleep". how does it work?

class bluetooth:
    def __init__(self):
        self.ser = Serial()

    def do_connect(self,port):
        self.ser.port = port
        self.ser.baudrate = 9600
        self.ser.timeout = 2 
        self.ser.open()
        if  not self.ser.isOpen():
            print ("Connection failed")
            return False
        #TODO: Connect the port with Serial. A clear description for exception may be helpful.'''
        return True

    def disconnect(self):
        self.ser.close()

    def SerialWrite(self,output):
        send = output.encode("utf-8")
        self.ser.write(send)

    def SerialReadString(self):
        rv = self.ser.readline().decode("utf-8")
        #TODO: Get the information from Bluetooth. Notice that the return type should be transformed into hex. 
        self.ser.reset_input_buffer()
        return rv
        
    def SerialReadByte(self):
        rv = self.ser.read().decode("utf-8")
        self.ser.reset_input_buffer()
        #TODO: Get the UID from bytes. Notice that the return type should be transformed into hex. 
        return rv