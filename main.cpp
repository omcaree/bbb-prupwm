/* Copyright (c) 2013 Owen McAree
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "pruPWM.h"

// Entry point
int main() {
	// Initialise PRU PWM
	PRUPWM *myPWM = new PRUPWM();

	// Set Channel 0 failsafe value
	myPWM->setFailsafeValue(0, 1000000);
	
	// Set a 2s failsafe timeout
	myPWM->setFailsafeTimeout(2000);
	
	// Start the PRU
	myPWM->start();

	// An example loop
	int pwm0 = 1000000, pwm7 = 2000000;
	int step = 100;
	while (true) {
		myPWM->setChannelValue(0,pwm0);
		myPWM->setChannelValue(7,pwm7);
		pwm0 += step;
		pwm7 -= step;
		if (pwm0 >= 2000000 || pwm0 <= 1000000 || pwm7 >= 2000000 || pwm7 <= 1000000) {
			step *= -1;
			usleep(1000000);
		}
		usleep(100);
	}
}
 