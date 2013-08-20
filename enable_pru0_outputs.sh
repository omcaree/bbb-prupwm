#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run with sudo" 1>&2
   exit 1
fi

set -e

# Build and install PRU DTBO
echo "Installing PRU Device Tree Overlay"
cd dtbos/
make
make install

# Enable PRU
echo "Enabling PRU"
echo bone_pru0_out > /sys/devices/bone_capemgr.*/slots

exit 0