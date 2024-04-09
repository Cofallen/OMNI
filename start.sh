#!/usr/bin/env bash
echo "start run start.sh"
sudo slcand -o -c -s8 /dev/ttyACM0 can0
sudo ifconfig can0 up
sudo chmod 777 /dev/ttyTHS0
#sudo cat /dev/ttyTHS0
#sudo reboot
#ls /dev | grep ACM
echo "start.sh done"