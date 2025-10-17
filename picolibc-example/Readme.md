# the picolib-example program

Using picolibc is easier than for RP2040. Picolibc defines a symbol _start, pointing to the program start address. So to make everything work we add a little startup code consisting of the
initial stackpointer, program start address (in the usual ARM way) and the necessary magic bytes.
