# the blink-small program

This is the program to start with. The same program is discussed in my rp2040-minimal repository. A difference with RP2040 is that to use a GPIO you have to clear the ISO bit of the GPIOx
register after configuring the GPIO. See table 852 of [RP2350]. The main difference is in the boot process. RP2040's bootrom hands control over to the program at the lowest flash address, usually
this will be the second-stage bootloader. The RP2350
bootrom expects your program to start in the standard ARM way, with a vector table. See [RP2350], section 5.9.5.1. This means that the first two words of flash need to contain the initial value
for the stack pointer and the entry address. This is done in the startup code (startup.s). Because we do not use interrupts this two-word vector table is sufficient. Apart from that, the bootrom
expects some metadata. Is the program intended to run on the ARm or on the RISC-V cores, does it require ARM secure execution, etc. All this is documented in section 5.1 of [RP2350]. No need to
read all of this. For our purposes we just need five magic words (20 bytes) which can be found in [RP2350], section 5.9.5. The bootrom looks for them in the first 4kB of the image. The startup.s
code takes care of this.
