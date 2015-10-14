"""
Created on Wednesday October 14 2015

@author: Aneesh Pappu

This file takes a photo with the Raspberry Pi camera module. Make sure 
that prior to use you run "sudo raspi-config" from the command line to
check that the Pi's camera features are enabled.

Also, make sure picamera is installed on the Pi. Run "sudo apt-get update"
followed by "sudo apt-get install python-picamera" to do this.
"""

import picamera #Used to take pictures
import time #so that camera can sleep


def takePhoto():
    with picamera.PiCamera() as camera: #get access to the picamera
        camera.start_preview() #Show the image of the object on monitor
    	time.sleep(2) #Sleep so that there is time to take a picture - sorta optional.
        camera.capture('item.jpg') #capture an image and store it to file item.jpg

def takeVideo():
    with picamera.PiCamera() as camera:
        camera.start_recording('exampleVid.h264')
        camera.wait_recording(10)
        camera.stop_recording()

def main():
    while 1:
        raw_input("Press Enter to take a photo") #Wait for the user to press "Enter" to initiate photo
        takePhoto() #Takes photo

        raw_input("Press Enter to take video") #Wait for user input
        takeVideo() #Takes video for ten seconds

main()
