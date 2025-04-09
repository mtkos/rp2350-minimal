#include <stdint.h>

#include "../include/common.h"
#include "../include/resets.h"
#include "../include/sio.h"
#include "../include/io_bank0.h"
#include "../include/pads_bank0.h"
#include "../include/m33.h"

#define IO_IRQ_BANK0_IRQn 21

#define PIN25 25

void IO_IRQ_BANK0_Handler() __attribute__((interrupt));

void IO_IRQ_BANK0_Handler(){

    REG(IO_BANK0_BASE + IO_BANK0_INTR2_OFFSET) = IO_BANK0_INTR2_GPIO23_EDGE_LOW_BITS;
}

void prog(){

    REG(RESETS_BASE + RESETS_RESET_OFFSET + REG_ALIAS_CLR_BITS) = RESETS_RESET_IO_BANK0_BITS | RESETS_RESET_PADS_BANK0_BITS;
    while((~(REG(RESETS_BASE + RESETS_RESET_DONE_OFFSET)) & (RESETS_RESET_DONE_IO_BANK0_BITS | RESETS_RESET_DONE_PADS_BANK0_BITS)));

    REG(IO_BANK0_BASE + IO_BANK0_GPIO24_CTRL_OFFSET) = GPIO_FUNC_SIO;
    REG(IO_BANK0_BASE + IO_BANK0_GPIO25_CTRL_OFFSET) = GPIO_FUNC_SIO;
    REG(PADS_BANK0_BASE + PADS_BANK0_GPIO23_OFFSET + REG_ALIAS_CLR_BITS) = PADS_BANK0_GPIO0_ISO_BITS;
    REG(PADS_BANK0_BASE + PADS_BANK0_GPIO25_OFFSET + REG_ALIAS_CLR_BITS) = PADS_BANK0_GPIO0_ISO_BITS;
    REG(PADS_BANK0_BASE + PADS_BANK0_GPIO23_OFFSET + REG_ALIAS_SET_BITS) = PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_OD_BITS;
    REG(SIO_BASE + SIO_GPIO_OE_SET_OFFSET) = 1<<PIN25;

    REG(IO_BANK0_BASE + IO_BANK0_PROC0_INTE2_OFFSET) = IO_BANK0_PROC0_INTE2_GPIO23_EDGE_LOW_BITS;

    REG(PPB_BASE + M33_NVIC_ISER0_OFFSET) = 1<<IO_IRQ_BANK0_IRQn;

    REG(PADS_BANK0_BASE + PADS_BANK0_GPIO23_OFFSET) = PADS_BANK0_GPIO0_IE_BITS | PADS_BANK0_GPIO0_SCHMITT_BITS;

    for(;;){
        __asm__ ("wfi");
        REG(SIO_BASE + SIO_GPIO_OUT_XOR_OFFSET) = 1<<PIN25;
    }
}
