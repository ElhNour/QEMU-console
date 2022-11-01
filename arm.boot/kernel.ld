/*
 * Define the entry point in the code, see vector.s
 */
ENTRY(_vector)
/*
 * Define the different sections included in the ELF file.
 */
SECTIONS
{
  . = 0x00; 
  .vector : { KEEP(vector.o(.text)) }
  . = 0x1000;
  .reset : { KEEP(reset.o(.text)) }
  .text : { *(.text) }

 /*
  * Include the initialized data sections
  */
 .data : { *(.data) }
 
  /*
  * Include the data sections that must be zeroed upon starting up.
  * Align the section on a 16-byte boundary, because we zero 16 bytes 
  * at a time (look at the code in reset.s).
  * Not only it is clean, simple, and efficient, but also some 
  * ARM processor do not allow unaligned accesses in memory.
  */ 
 .bss : {
   . = ALIGN(16); 
   _bss_start = .;
   bss = .;
   *(.bss COMMON)
   . = ALIGN(16); 
   _bss_end = .;
 } 
 
 /* 
  * Finally, reserve some memory for the C stack. By default,
  * on the ARM cores used in the Versatile AB and PB,  
  * the C stacks are growing downward. 
  * Nota Bene: a C stack must be 8-byte aligned. 
  */
  . = ALIGN(8);
  . = . + 0x1000; /* 4kB of stack memory */
  stack_top = .;
}
