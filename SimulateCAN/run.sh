#!/bin/bash

source venv/bin/activate

sudo ip link set can0 down
sudo ip link set can0 type can bitrate 1000000
sudo ip link set can0 up

if [[ -z "$1" ]]; then
    :
else
    python3 "$1"
fi
