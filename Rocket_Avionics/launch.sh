# edit /etc/rc.local to ~/Desktop/Code/Rocket_Avionics/launch.sh
# put this file there

#!/bin/sh
sleep 10

cd
cd Desktop
cd Code
cd Rocket_Avionics
sudo python rasp-i2c.py