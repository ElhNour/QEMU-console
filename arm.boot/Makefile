#------------------------------
# Machine options:
#   QEMU_MACHINE=versatileab
#   QEMU_MACHINE=versatilepb
QEMU_MACHINE=versatilepb

ifeq ($(QEMU_MACHINE),versatileab) 
  QEMU_CPU=arm926
  QEMU_MEMORY=1M
  ASFLAGS=-mcpu=arm926ej-s 
  CFLAGS= -DBOARD_VERSATILEAB -mcpu=arm926ej-s
endif

ifeq ($(QEMU_MACHINE),versatilepb) 
  QEMU_CPU=cortex-a8
  QEMU_MEMORY=32M
  ASFLAGS=
  CFLAGS= -DBOARD_VERSATILEPB -mcpu=cortex-a8
endif


QEMU=qemu-system-arm

QEMU_LOADER=-device loader,file=kernel.elf

#------------------------------
# Serial options:
#   with the QEMU console on stdio
#     -serial mon:stdio
#   with a tcp server on port 4444
#     -serial tcp::4444,server,nowait
#   with a telnet server on port 4321
#     -serial telnet:localhost:4321,server,nowait
#   attach via a shell, on a the matching port:
#     $ telnet locahost 4321
#-------------------------------------------------------------------
QEMU_SERIAL=-serial mon:stdio

#-------------------------------------------------------------------
# Prefix for the toolchain, like:
#   TOOLCHAIN=arm-none-eabi
# assuming that your compiler is 
#   arm-none-eabi-gcc 
# and that it is in your PATH.
# Otherwise, put a full path + prefix here, something
#   TOOLCHAIN=/usr/bin/arm-none-eabi
#-------------------------------------------------------------------
TOOLCHAIN=arm-none-eabi

#-------------------------------------------------------------------
# Compiler, assembler, and linker flags
#-------------------------------------------------------------------
ASFLAGS+= -g
CFLAGS+= -g -c -nostdlib -O0
LDFLAGS= -g -T kernel.ld 

#-------------------------------------------------------------------
# Object file list
#-------------------------------------------------------------------
OBJS=vector.o reset.o uart.o main.o 

#-------------------------------------------------------------------
# Compilation rules
#-------------------------------------------------------------------
%.o: %.c
	$(TOOLCHAIN)-gcc $(CFLAGS) -o $@ $<

%.o: %.s
	$(TOOLCHAIN)-as $(ASFLAGS) -o $@ $<


#-------------------------------------------------------------------
all: $(OBJS) 
	$(TOOLCHAIN)-ld $(LDFLAGS) $(OBJS) -o kernel.elf

clean: 
	rm -f *.o kernel.elf

#-------------------------------------------------------------------
run: all
	$(QEMU) -M $(QEMU_MACHINE) -cpu $(QEMU_CPU) -m $(QEMU_MEMORY) -nographic $(QEMU_LOADER) $(QEMU_SERIAL)

debug: all
	$(QEMU) -M $(QEMU_MACHINE) -cpu $(QEMU_CPU) -m $(QEMU_MEMORY) -nographic $(QEMU_LOADER) $(QEMU_SERIAL) -gdb tcp::1234 -S

kill:
	pkill qemu-system-arm

