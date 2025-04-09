# the exti-interrupt program

This works as in my rp2040-minimal repository. Only the interrupt numbers are different. They can be found in section 3.2 of [RP2350]. IO\_IRQ\_BANK0, which is the one we need, has interrupt
number 21. This corresponds to position 21 + 16 = 37 in the vector table, so startup.s uses this number. The electrical characteristics of a GPIO pin are determined by the PADS\_BANK0 registers
([rp2350], section 9.11.3). In my board, closing the switch connected to GPIO23 pulls it low. However, it is not necessary to activate the pullup for this pin, because the board already provides
a 5.1K pullup.
