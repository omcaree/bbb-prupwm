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

// Class derived from node-pru Node.js module
 
#ifndef pru_h
#define pru_h

// System headers
#include <stdio.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string>

// PRU Driver headers
#include <prussdrv.h>
#include <pruss_intc_mapping.h>	 

class PRU {
public:
	PRU(int number = 0);
	~PRU();
	
	void execute(const char * program);
	void stop();
	void setSharedMemoryInt(unsigned int index, unsigned int value);
	void setSharedMemory(unsigned int * array, unsigned int start, unsigned int length);
	unsigned int getSharedMemoryInt(unsigned int index);
	void getSharedMemory(unsigned int * memory, unsigned int start, unsigned int length);

private:
	int pruNumber;
	static unsigned int* memoryPtr;
	static const unsigned int OFFSET_SHAREDRAM = 2048;
};
#endif