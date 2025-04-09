## rp2350-minimal

This is one in a series of repositories about bare metal programming. The main ones are ch32v003-minimal, about the ch32v003 RISC-V MCU, and stm32f103-minimal, about the ARM STM32F103 MCU.
The structure of the present repository is similar: a series of small programs of increasing complexity, using not much more than the datasheet and the GNU compiler and binutils. There
is also a repository rp2040-minimal. The present repository is very similar to that one, so only the differences are highlighted. The main difference is in the boot process.
This is discussed in the Readme of blink-small.
Apart from that, there are a few changes in the hardware which deserve mention. This is done in the Readme's of the respective programs.
RP2350 also features two RISC-V cores. Programming these is discussed in the rp2350-riscv-minimal
repository. The board I used, WeAct RP2350A_V10, has two LEDs built in, at GPIO 24 and 25, and one user button, at GPIO 23. To try the programs on other boards you will need to make some small
changes. Everything was tested on Debian 12. All the necessary software (GCC, picolibc, Python) is available as a Debian package. Many of the programs were ported from RP2040 without
significant modifications. For these, no Readme has been included.

The programs are, in order of increasing complexity:

1. blink-small. The most basic blink program.

2. blink-asm. The same in assembly.

3. uart. Shows how to do serial communication with the chip. The program repeatedly reads a character and writes back the next ASCII character. If you type 'a' in the serial console you
will read 'b'.

4. blink. A somewhat bigger example, using auxiliary functions and global variables. The linker script comes into play.

5. systick. Shows how to use the SysTick timer, present in all ARM Cortex-M microcontrollers. Shows how to use the associated interrupt.

6. exti-interrupt. Shows how to use the external interrupt raised when a button is pressed.

7. timer. Shows how to use PWM. The program blinks two LEDs with different duty cycles, without further intervention from the processor.

8. timer-interrupt. The PWM timer can also generate interrupts.

9. picolibc-example. Shows how to get the picolibc library working, which offers math functions and formatted IO, among other things.

10. ws2812d. Shows how to use the PIO peripheral to drive ws2812 RGB LEDs.

11. blink-multicore. RP2350 has two ARM cores. This program shows how to run a program on both of them.

Programming without using the SDK means that you are on your own. At times it can be difficult to resolve bugs. They can be in the hardware (a bad soldering joint) and in several pieces of software:
linker script, startup code, main code, ... On the other hand, you have complete control: no initializations of clocks or other peripherals you do not need, no superfluous error checking, no
unnecessarily long vector tables, etc. As a
bonus, the resulting binaries are really small: about 100 bytes for the blink-small program. Try to write the same program using the SDK and be surprised at the size of the binary.

The datasheet can be found at <https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf>. I will refer to it as [RP2350].
