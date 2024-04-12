#!/bin/bash 

sudo ls 


sudo modprobe vcan


ip link show



sudo ip link add dev vcan0 type vcan 


sudo ip link set up vcan0


read -s -n1 -p 'press any key ... '

echo " "
echo "candump vcan0"

candump vcan0
