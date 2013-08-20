#!/bin/bash

if [[ $EUID -ne 0 ]]; then
   echo "This script must be run with sudo" 1>&2
   exit 1
fi

set -e

# Clear anything that may already exist
rm -rf am335x_pru_package

# Load the PRU kernel driver
echo "Loading PRU kernel driver (uio_pruss)"
modprobe uio_pruss

# Get the userspace driver
echo "Grabbing userspace driver"
git clone https://github.com/beagleboard/am335x_pru_package.git
cd am335x_pru_package

# Apply the patch to prevent interrupts being fired twice
echo "Patching driver to prevent double interrupts"
wget http://e2e.ti.com/cfs-file.ashx/__key/telligent-evolution-components-attachments/00-791-00-00-00-23-97-35/attachments.tar.gz
tar -xzf attachments.tar.gz
patch -p1 <  0001-Fix-for-duplicated-interrupts-when-interrupts-are-se.patch

# Apply patch to remove needless printf statements
patch -p1 < ../patches/Remove-needless-printfs.patch

# Build driver as shared library
echo "Building driver"
cd pru_sw/app_loader/interface/
gcc -I. -Wall -I../include   -c -fPIC -O3 -mtune=cortex-a8 -march=armv7-a -shared -o prussdrv.o prussdrv.c
gcc -shared -o libprussdrv.so prussdrv.o

# Install driver and headers
echo "Installing driver"
cp libprussdrv.so /usr/lib/
cp ../include/*.h /usr/include/

# Build the assembler
echo "Building assembler"
cd ../../utils/pasm_source
./linuxbuild

# Install the assembler
echo "Installing assembler"
cp ../pasm /usr/bin/

echo "PRUSS successfully installed"