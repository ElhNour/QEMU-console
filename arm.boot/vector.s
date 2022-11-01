/*-------------------------------------------------------------------
 * BOOT ENTRY POINT
 * This is to load the page-0 vector for processor execptions,
 * including both interrupts and traps.
 *
 * Look at the linker script where it is layed up at 0x0000-0000.
 * Also consider the section optional flags "xa" that are necessary
 * to force the linker to mark this section as
 *    CONTENTS, ALLOC, LOAD, READONLY, CODE
 * The essential attributes are ALLOC and LOAD, without LOAD,
 * the QEMU ELF loader will not load the section in memory,
 * and neither will the gdb load to the target.
 *------------------------------------------------------------------*/
 	.text
     .section .vector, "xa"  /* xa forces the section to be ELF PT_LOAD and PT_ALLOC */
    .global _vector
_vector:
	ldr pc,[pc,#0x18] /* 0x00 reset */
	ldr pc,[pc,#0x18] /* 0x04 undefined instruction */
	ldr pc,[pc,#0x18] /* 0x08 software interrupt */
	ldr pc,[pc,#0x18] /* 0x0c prefetch abort */
	ldr pc,[pc,#0x18] /* 0x10 data abort */
	ldr pc,[pc,#0x18] /* 0x14 reserved */
	ldr pc,[pc,#0x18] /* 0x18 IRQ */
	ldr pc,[pc,#0x18] /* 0x1c FIQ */
	.word _reset
	.word _undefined_instruction
	.word _software_interrupt
	.word _prefetch_abort
	.word _data_abort
	.word _reserved
	.word _irq_handler
	.word _fiq_handler

_undefined_instruction:
	b _reset

_software_interrupt:
	b _halt
_prefetch_abort:
	b _halt
_data_abort:
	b _halt
_reserved:
	b _halt
_fiq_handler:
	b _halt
_irq_handler:
	b _halt

