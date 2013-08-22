// Copyright (c) 2013 Owen McAree
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

.origin 0
.entrypoint START
#include "../include/pru.hp"

START:
// Preamble to set up OCP and shared RAM
	LBCO	r0, CONST_PRUCFG, 4, 4		// Enable OCP master port
	CLR 	r0, r0, 4					// Clear SYSCFG[STANDBY_INIT] to enable OCP master port
	SBCO	r0, CONST_PRUCFG, 4, 4
	MOV		r0, 0x00000120				// Configure the programmable pointer register for PRU0 by setting c28_pointer[15:0]
	MOV		r1, CTPPR_0					// field to 0x0120.  This will make C28 point to 0x00012000 (PRU shared RAM).
	ST32	r0, r1
// End of preamble
	
// Shared memory registers 
//	Int 0: 		Total PWM period (number of PRU cycles)
//	Int 1-8:	Pulse width for each channel (number of PRU cycles)
//	Int 9:		Failsafe timeout (number of PWM cycles)
//				e.g. for a PWM frequency of 50Hz, a value of 100 would be a 2s timeout
//				Must be reset everytime channels are updated
//				Set to 0 to disable failsafe feature
//	Int 10-17:	Failsafe PWM periods (number of PRU cycles)
LOOP1:										// Outer loop repeats everything at PWM period
	LBCO	r0, CONST_PRUSHAREDRAM, 0, 40	// Load in registers from shared memory
	QBEQ	NO_FAILSAFE, r9, 0				// Check to see if failsafe is enabled
	QBEQ	FAILSAFE, r9, 1					// Check to see if failsafe timeout has occured
	SUB		r9, r9, 1						// If not timed out, decrement timeout counter
	SBCO	r9, CONST_PRUSHAREDRAM, 36, 4	// Write timeout counter back to shared RAM
	QBA		NO_FAILSAFE						// Skip failsafe action
FAILSAFE:
	LBCO	r1, CONST_PRUSHAREDRAM, 40, 32	// Overwrite commanded positions with failsafe positions

NO_FAILSAFE:
	MOV		r30.b0, 0xFF					// Turn on all output channels for start of cycle
	
LOOP2:										// Inner loop to handle channels
		SUB		r0, r0, 1					// Subtract one from each register
		SUB		r1, r1, 1
		SUB		r2, r2, 1
		SUB		r3, r3, 1
		SUB		r4, r4, 1
		SUB		r5, r5, 1
		SUB		r6, r6, 1
		SUB		r7, r7, 1
		SUB		r8, r8, 1
		QBNE	SKIP1, r1, 0				// Compare each register with zero
		CLR		r30.t0						// If zero then turn off the corresponding channel
SKIP1:										// Otherwise skip that channel (leaving it high)
		QBNE	SKIP2, r2, 0
		CLR		r30.t1
SKIP2:
		QBNE	SKIP3, r3, 0
		CLR		r30.t2
SKIP3:
		QBNE	SKIP4, r4, 0
		CLR		r30.t3
SKIP4:
		QBNE	SKIP5, r5, 0
		CLR		r30.t4
SKIP5:
		QBNE	SKIP6, r6, 0
		CLR		r30.t5
SKIP6:
		QBNE	SKIP7, r7, 0
		CLR		r30.t6
SKIP7:
		QBNE	SKIP8, r8, 0
		CLR		r30.t7
SKIP8:
		QBEQ	LOOP1, r0, 0			// If cycle register is empty, restart
		QBA		LOOP2					// Total of 19 statements per cycle, 95ns	
	HALT								// Halt the processor (although we will never get here...)
	