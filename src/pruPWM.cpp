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

PRUPWM::PRUPWM(unsigned int frequency) : PRU(0) {
	this->stop();
	this->setFrequency(frequency);
	this->setFailsafeTimeout(0);
	for (int i = 0; i < 8; i++) {
		this->setChannelValue(i, 1500000);
		this->setFailsafeValue(i, 1500000);
	}
}

void PRUPWM::start() {
	this->execute("pwm.bin");
}

void PRUPWM::setFrequency(unsigned int frequency) {
	this->pwmFrequency = frequency;
	this->setSharedMemoryInt(0, 1000000000 / (PRUPWM::nanosecondsPerCycle*this->pwmFrequency));
}

void PRUPWM::setChannelValue(unsigned int channel, unsigned long pwm_ns) {
	this->setPRUDuty(channel, pwm_ns);
}
void PRUPWM::setFailsafeValue(unsigned int channel, unsigned long pwm_ns) {
	this->setPRUDuty(channel+9, pwm_ns);
}

void PRUPWM::setFailsafeTimeout(unsigned int timeout_ms) {
	this->failsafeTimeout = timeout_ms;
	this->updateFailsafe();
}

void PRUPWM::setPRUDuty(unsigned int channel, unsigned long pwm_ns) {
	this->setSharedMemoryInt(channel+1, (unsigned int)((unsigned long long)pwm_ns / PRUPWM::nanosecondsPerCycle));
	this->updateFailsafe();
}
void PRUPWM::updateFailsafe() {
	this->setSharedMemoryInt(9, this->failsafeTimeout * this->pwmFrequency / 1000);
}
