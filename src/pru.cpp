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
 
#include "pru.h"

unsigned int* PRU::memoryPtr = NULL;

PRU::PRU(int number) {
	// Store the PRU number (0 or 1)
	if (number < 2) {
		this->pruNumber = number;
	} else {
		this->pruNumber = 0;
	}

	// Initialise driver
	prussdrv_init ();
	
	// Open interrupt
	unsigned int ret = prussdrv_open(PRU_EVTOUT_0);
	if (ret) {
		printf("prussdrv_open open failed\n");
		return;
	}
	
	//Initialise interrupt
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;	
	prussdrv_pruintc_init(&pruss_intc_initdata);
	
	// Allocate shared PRU memory
	if (!this->memoryPtr) {
		static void *sharedMem;
		prussdrv_map_prumem(PRUSS0_SHARED_DATARAM, &sharedMem);
		this->memoryPtr = (unsigned int*) sharedMem;
	}
}

PRU::~PRU() {
	this->stop();
    prussdrv_exit ();
};

void PRU::execute(const char * program) {
	prussdrv_exec_program (this->pruNumber, (char *)program);
	
};

void PRU::stop() {
	prussdrv_pru_disable(this->pruNumber); 
}

void PRU::setSharedMemoryInt(unsigned int index, unsigned int value) {
	this->memoryPtr[PRU::OFFSET_SHAREDRAM + index] = value;
};

void PRU::setSharedMemory(unsigned int * array, unsigned int start, unsigned int length) {
	for (int i = 0; i<length; i++) {
		this->setSharedMemoryInt(start + i, array[i]);
	}
};

unsigned int PRU::getSharedMemoryInt(unsigned int index) {
	return this->memoryPtr[PRU::OFFSET_SHAREDRAM + index];
};

void PRU::getSharedMemory(unsigned int * memory, unsigned int start, unsigned int length) {
	for (unsigned int i = 0; i<length; i++) {
		memory[i] = this->getSharedMemoryInt(start + i);
	}
};

// TODO: Implement interrupt handling
/*-------------------This is mostly copy/pasted from here: ---------------------*/
/*----------------http://kkaefer.github.io/node-cpp-modules/--------------------*/
/*struct Baton {
    uv_work_t request;
    Persistent<Function> callback;
    int error_code;
    std::string error_message;
    int32_t result;
};

void AsyncWork(uv_work_t* req) {
//    Baton* baton = static_cast<Baton*>(req->data);
	prussdrv_pru_wait_event(PRU_EVTOUT_0);
}

void AsyncAfter(uv_work_t* req, int status) {
    HandleScope scope;
    Baton* baton = static_cast<Baton*>(req->data);
	baton->callback->Call(Context::GetCurrent()->Global(), 0, 0);
    baton->callback.Dispose();
    delete baton;
}

Handle<Value> waitForInterrupt(const Arguments& args) {
	HandleScope scope;
	Local<Function> callback = Local<Function>::Cast(args[0]);

	Baton* baton = new Baton();
    baton->request.data = baton;
    baton->callback = Persistent<Function>::New(callback);
	
	uv_queue_work(uv_default_loop(), &baton->request, AsyncWork, AsyncAfter);
	return scope.Close(Undefined());
}

/*---------------------------Here ends the copy/pasting----------------------------*/

/* Clear Interrupt */
/*Handle<Value> clearInterrupt(const Arguments& args) {
	HandleScope scope;
	prussdrv_pru_clear_event(PRU0_ARM_INTERRUPT);
	return scope.Close(Undefined());
};

Handle<Value> interruptPRU(const Arguments& args) {
	HandleScope scope;
	prussdrv_pru_send_event(ARM_PRU0_INTERRUPT);
	return scope.Close(Undefined());
};*/