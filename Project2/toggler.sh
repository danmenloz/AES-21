#!/bin/bash
#  Toggle a GPIO pin

# please clean up on exit
# use Ctrl-C or kill <PID> to kill this process, so this trap runs  
trap "echo 18 > /sys/class/gpio/unexport" EXIT

# configure
echo 18 > /sys/class/gpio/export
sleep 0.5
echo "out" > /sys/class/gpio/gpio18/direction

COUNTER=0
while [ $COUNTER -lt 10000 ]; do
    echo 1 > /sys/class/gpio/gpio18/value
    sleep 0.005
    echo 0 > /sys/class/gpio/gpio18/value
    sleep 0.005
    let COUNTER=COUNTER+1
done
echo 18 > /sys/class/gpio/unexport
