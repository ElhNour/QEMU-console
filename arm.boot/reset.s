
.global _reset
_reset:

	/*-------------------------------------------
	 * Clear out the bss section, located from _bss_start to _bss_end.
	 * This is a C convention, the GNU GCC compiler will group all global variables
	 * that need to be zeroed on startup in the bss section of the ELF.
	 *-------------------------------------------*/
.clear:
	ldr	r4, =_bss_start
	ldr	r9, =_bss_end
	mov	r5, #0
2:
	stmia	r4!, {r5} 
	cmp	r4, r9
	blo	2b

 	/*
 	 * Now upcall the C entry function, _start(void)
 	 */
    ldr sp, =stack_top
    bl _start

.global _halt
_halt:
	b	_halt

