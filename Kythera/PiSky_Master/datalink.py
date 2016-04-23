"""
Abstraction and definition of all communications capabilities via
XBEE radio.

MESSAGE STRUCTURE

flightStatus --> 2 digit flight status at front of every message
system --> "K" = kythera data downlink
           "N" = node health report downlink
           "D" = directive requiring reply
info --> data to be transmitted
info_sum --> checksum of info to be examined on ground

"""

import serial
from datetime import datetime
import time
import threading

ser = serial.Serial('/dev/ttyUSB0', 19200)
Kythera_TestData = './testing/Kythera_messages.txt'
logFile = open('./logs/RadioLog.txt', 'a')
logFile.write('\n\nSTARTING NEW SESSION\n')
connected = False

currentData = [];
currentIndex = 0;

"""
Send passed arguments down
"""
def sendDown(flightStatus, system, info, info_sum):
    sendingString=flightStatus+system+info+info_sum
    ser.write(sendingString)
    logFile.write("SENDING\n"+datetime.now().time().isoformat()+"\n"+sendingString+"\n")

"""
Format string to send down from Kythera
"""
def KythMessage(flightStatus, heading, yaw, pitch, ax, ay, az, atm, temp, tplus):
    info_sum = heading+yaw+pitch+ax+ay+az+atm+temp+tplus
    flightStatus = str(flightStatus).zfill(3)
    heading = str(heading).zfill(3)
    yaw = str(yaw).zfill(3)
    pitch = str(pitch).zfill(3)
    ax = str(ax).zfill(2)
    ay = str(ay).zfill(2)
    az = str(az).zfill(3)
    atm = str(atm).zfill(3)
    temp = str(temp).zfill(3)
    tplus = str(tplus).zfill(4)
    info = heading+yaw+pitch+ax+ay+az+atm+temp+tplus
    sendDown(flightStatus, "K", info, info_sum)

"""
Run a test output with simulated Kythera data
"""
def runTest():
    testFile = open('./logs/RadioLog.txt', 'r')
    while True:
        line = testFile.readline()
        if line == '':
            break
        line.replace(" ", "")
        ser.write(line)
        logFile.write("TEST SENDING\n"+datetime.now().time().isoformat()+"\n"+line+"\n")
        time.sleep(4)

def handle_data(data):
    currentData.append(data)
    currentIndex = currentIndex+1

def read_from_port(ser):
    while not connected:
        connected = True

        while True:
           reading = ser.readline()
           handle_data(reading)

def beginInput():
    Ithread = threading.Thread(target=read_from_port, args=(serial_port,))
    Ithread.start()
    return Ithread
