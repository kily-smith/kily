#!/bin/bash 

sudo ls 


sudo modprobe can
sudo modprobe vcan
sudo modprobe slcan


ip link show

ifconfig can0


sudo ip link set can0 type can bitrate 1000000


sudo ip link set up can0


read -s -n1 -p 'press any key ... '

echo " "
echo "candump can0"

candump can0
