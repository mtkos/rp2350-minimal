# the picolib-example program

Using picolibc is easier than for RP2040, because there is no need for a secondary bootloader. The startup code consists simply of a minimal vector table and bootloader magic bytes. The picolibc
linker script defines a \_start symbol which can be used for the second entry in the vector table.
