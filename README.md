PWM servo control with the BeableBone Black PRUs
================================================

This project contains everything you need to get a simple C++ application controlling servos via with BBB PRUs. The reason for using the PRUs over the onboard PWMs is the acheivable resolution (~100ns with the PRU) and number of channels (24 across both PRUs, only 8 used by this project at present).

This work is based on my [node.js module](https://github.com/omcaree/node-pru), but designed to work from C++ instead.

Update 28th August 2013:

 * Modified PRU code and C++ class to implement a simple failsafe timeout. If host code hangs or crashes the PRU will revert the channels to predefined PWM values after a timeout has elapsed

Prerequisites
-------------
Make sure you have build-essential installed so that you can compile things

	sudo apt-get install build-essential

PRU set up
-----------
I've previous written up the PRU set up proceedure in my [node.js module](https://github.com/omcaree/node-pru), however I have now automated this process in an bash script. Firstly, get the source

	git clone https://github.com/omcaree/bbb-prupwm
	cd bbb-prupwm
	
Now install the PRU userspace driver

	sudo ./install_pruss.sh
	
In the dtbos/ directory I have created a device tree overlay which enables the PRU and muxes all the available output pins for PRU0 (P8_11, P8_12, P9_25, P9_27, P9_28, P9_29, P9_30, P9_31, P9_41, P9_42). To install and enable this overlay, run

	sudo ./enable_pru0_outputs.sh
	
Run example
-----------
Once the PRU is set up you can build and run the example code with

	make
	sudo ./prupwm
	
This code defaults all outputs to 1500us at 50Hz, then cycles channel 0 (P9_31) and channel 7 (P9_25) indefinately. Pressintg Ctrl+C will return all channels to 1500us before exiting.