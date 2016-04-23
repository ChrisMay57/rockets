"""
Main executable for the Kythera flight computer

Overview:
The flight computer software occupies 4 major modes:
 --> power on:   initializes radio communication, sensors, and pings all
                 connected CAN devices. Blocks on additional input via the XBEE.

 --> power-up:   Keeps launch bit in pre-flight state regardless of sensor input.
                 Can execute custom setup functions

 --> testing:    Kythera enters testing state. Output on all functions is set to
                 verbose. Blocks on a testfile name via the XBEE. Will then
                 execute the testfile which contains a relative path to
                 a custom data input file. If no data file is provided, sensor
                 data is read. Optionally can send testing bit via CAN.

  --> flight:    Flight loop is entered. State logic is allowed to execute in
                 order to dynamically update flight data bit sent to CAN devices.
                 GUI formatted downlink via radio begins. Provides ability to end
                 flight by sending "abort abort abort". Will respond with custom
                 code. Code must be sent back in order to abort flight. Flight
                 loop will automatically resume unless the code is entered TWICE.

"""

# IMPORTS
import smbus
import time
import struct
import csv
import subprocess


# INIT GLOBALS



# INIT XBEE
import datalink
Ithread = datalink.beginInput()

# INIT SENSORS


# PING CAN BUSSES


# POWERUP


# testing
datalink.runTest()

# FLIGHT



if __name__ == '__main__' :
    # run everything required
