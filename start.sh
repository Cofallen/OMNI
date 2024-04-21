#!/usr/bin/env bash

# 应是nx开机后自启内容
echo "start run start.sh"

sudo modprobe mttcan
sudo ip link set can1 type can bitrate 1000000
sudo ip link set can1 up

sudo slcand -o -c -s8 /dev/ttyACM0 can2
sudo ifconfig can2 up

sudo chmod 777 /dev/ttyTHS0
#sudo cat /dev/ttyTHS0
#ls /dev | grep ACM
echo "start.sh done"