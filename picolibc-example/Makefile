TARGET = picolibc-example

SRC = main.c
SRC += startup.s

include ../build.mk

FLAGS = -Os -specs=picolibc.specs -mcpu=cortex-m33 -T../ld/Link-pico.ld -Wall -Wl,-Map,$(TARGET).map
