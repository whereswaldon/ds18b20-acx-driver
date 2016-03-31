
/*
 * acx_asm.s
 *
 * Created: 
 * Author: efb
 */ 
 #define __SFR_OFFSET 0
 #include <avr/io.h>
 #include "acx.h"


		.section .text
		.global x_yield
x_yield:
//------------------------------------------------------------------
// Save "callee-save" registers
//------------------------------------------------------------------
		push	r28
		push	r29
		push	r2
		push	r3
		push	r4
		push	r5
		push	r6
		push	r7
		push	r8
		push	r9
		push	r10
		push	r11
		push	r12
		push	r13
		push	r14
		push	r15
		push	r16
		push	r17


//------------------------------------------------------------------
//   Get thread ID and mask
//------------------------------------------------------------------

		lds	r20,	x_thread_id
		lds	r21,	x_thread_mask

//------------------------------------------------------------------
// Store SP into current thread's stack-save area
//------------------------------------------------------------------
		;compute index into stacks array
		mov	r22,	r20			;make a copy of the thread id
		lsl	r22	  				;left shift two to multiply by 2
		lsl	r22	  				;left shift two to multiply by 2
		ldi	r30,	lo8(stacks)	;load the address of the array
		ldi r31,	hi8(stacks)	;load the other byte
		add	r30,	r22			;increment the address by index
		adc r31,	0			;pull in the carry from previous, if any
		
		;write the SP into the stack save area
		lds	r16,	(0x5d)		;Load SP low byte
		st	Z,		r16			;save SP low byte
		lds r16,	(0x5e)		;Load SP high byte
		std	Z+1,	r16			;save SP high byte


;------------------------------------------------------------------------
; Schedule next thread -- must use caller-save registers below here
;                         because we can reach this without saving regs
;                         if entered from x_new() function
;-------------------------------------------------------------------------
		.global	x_schedule
x_schedule:
	// determine READY status of each thread
		lds r18,	disables	;load the disabled status
		lds r19,	suspends	;load the suspended status
		or	r18,	r19			;or disables and suspends
		lds r19,	delays		;load the delayed status
		or	r18,	r19			;or disables, suspends, and delays

		lds	r20,	x_thread_id	;load the id of the current thread
		lds r21,	x_thread_mask	;load the thread id mask

;------------------------------------------------
;   Loop through all threads to test for READY
;------------------------------------------------
loop:
		inc	r20					;increment thread id
		rol r21					;rotate thread mask left
		cpi	r21,	0			;check if thread mask is zero
		brne skip				;jump over second rotate
		inc r21					;reset the thread mask to one
		ldi	r20,	0			;reset thread id to zero
skip:	
		mov r22,	r21			;copy the thread mask
		and	r22,	r18			;compare thread mask to or-ed statuses
		cpi r22,	0			;if the result is zero, this next thread is ready
		brne loop				;else restart the loop

		jmp	restore				;skip the nap code

;----------------------------------------------------------
;  SLEEP HERE:  Here's where we sleep (no threads are READY)
;  but for now we'll jump back to schedule loop again
;----------------------------------------------------------
nap:
		; add sleep instructions here...
		;sleep
		;TODO: uncomment & implement
		rjmp	x_schedule			

;---------------------------------------------------
; Restore context of next READY thread
;---------------------------------------------------
restore:
		;r20 holds thread id
		;r21 holds thread mask

		;compute index into stacks array
		mov	r22,	r20			;make a copy of the thread id
		lsl	r22	  				;left shift two to multiply by 2
		lsl	r22	  				;left shift two to multiply by 2
		ldi	r30,	lo8(stacks)	;load the address of the array
		ldi r31,	hi8(stacks)	;load the other byte
		add	r30,	r22			;increment the address by index
		adc r31,	0			;pull in the carry from previous, if any
		
		// update hardware SP
		ld	r23,	Z+			;load new thread's low SP byte
		ld	r24,	Z			;load new thread's high SP byte
		sts	(0x5d),	r23			;change lower SP byte
		sts (0x5e),	r24			;change upper SP byte

		// Restore registers
		pop	r17
		pop	r16
		pop	r15
		pop	r14
		pop	r13
		pop	r12
		pop	r11
		pop	r10
		pop	r9
		pop	r8
		pop	r7
		pop	r6
		pop	r5
		pop	r4
		pop	r3
		pop	r2
		pop	r29
		pop	r28

		// Return to next thread
		ret
