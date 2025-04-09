TARGET = ws2812d

SRC = main.c
SRC += startup.s

include ../build.mk

FLAGS += -Wl,-T../ld/Link.ld

main.c: $(TARGET).pio.h
	touch $@

$(TARGET).pio.h: $(TARGET).asm
	../utils/instr.py < $^ > $@

asm: $(TARGET).asm

$(TARGET).asm: $(TARGET).pio
	../utils/pioasm $^ > $@
