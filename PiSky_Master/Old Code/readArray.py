import smbus
import time

bus = smbus.SMBus(1)
address =  0x04

def readPacket():
    data = ''
    length = int(bus.read_byte(address));
    print 'length of data: ' + str(length)
    for i in range (0,length):
        data += str(bus.read_byte(address))+ ' / ';
    print data

if __name__ == "__main__":
    readPacket()