#!/bin/sh
# launcher.sh
# This code navigates to home directory, then to this directory, then execute python script, then back home

# To make this file executable, run the following two commands on the pi. Only has to be done once:

# chmod 755 launcher.sh //Makes it executable
# sh launcher.sh // Tests whether the bash script runs

# Then create a directory for the logs in the home directory, using:

# mkdir logs

# After, type in 
# sudo crontab -e
# and enter the following line into the file that pops up, replacing bbt with where our launcher file lives:

# @reboot sh /home/pi/bbt/launcher.sh >/home/pi/logs/cronlog 2>&1

cd /
cd Desktop/Code/Rocket_Avionics
sudo python rasp-i2c.py
cd /