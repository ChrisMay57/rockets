import time
import serial

"""
    Python text based ground station. 
"""

xbee = serial.Serial(
        port='/dev/tty/1', # figure out what this is 
        baudrate=9600,
        parity=serial.PARITY_ODD,
        stopbits=serial.STOPBITS_TWO,
        bytesize=serial.SEVENBITS
    )

filename = "xbee_log.txt"

"""
    Get command from user 
"""
def getCommand(): 
    return 

"""
    Setup XBee and log data continuously. Receive and transmit commands. 
"""
if __name__ == '__main__':     
    xbee.isOpen()
    
    print 'Enter your commands below.\r\nType "exit" to leave the application.'
    
    logfile = open(filename, "r+")

    while True: 
        # log data from serial 
        while ser.inWaiting() > 0:
            out += ser.read(1)
        logfile.write(out)