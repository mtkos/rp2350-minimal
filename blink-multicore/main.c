#include <stdint.h>

#include "../include/common.h"
#include "../include/resets.h"
#include "../include/sio.h"
#include "../include/io_bank0.h"
#include "../include/pads_bank0.h"

#define PIN24 24
#define PIN25 25

extern uint32_t _estack1, _flash_base;

void blink(){

    if(REG(SIO_BASE + SIO_CPUID_OFFSET) == 0)
        for(;;){
            REG(SIO_BASE + SIO_GPIO_OUT_XOR_OFFSET) = 1<<PIN24;
            for(uint32_t i = 500000; i; i--);
        }
    else
        for(;;){
            REG(SIO_BASE + SIO_GPIO_OUT_XOR_OFFSET) = 1<<PIN25;
            for(uint32_t i = 1600000; i; i--);
        }
}

void prog(){

    REG(RESETS_BASE + RESETS_RESET_OFFSET + REG_ALIAS_CLR_BITS) = RESETS_RESET_IO_BANK0_BITS;
    while((~(REG(RESETS_BASE + RESETS_RESET_DONE_OFFSET)) & RESETS_RESET_DONE_IO_BANK0_BITS));

    REG(IO_BANK0_BASE + IO_BANK0_GPIO24_CTRL_OFFSET) = GPIO_FUNC_SIO;
    REG(IO_BANK0_BASE + IO_BANK0_GPIO25_CTRL_OFFSET) = GPIO_FUNC_SIO;
    REG(PADS_BANK0_BASE + PADS_BANK0_GPIO24_OFFSET + REG_ALIAS_CLR_BITS) = PADS_BANK0_GPIO0_ISO_BITS;
    REG(PADS_BANK0_BASE + PADS_BANK0_GPIO25_OFFSET + REG_ALIAS_CLR_BITS) = PADS_BANK0_GPIO0_ISO_BITS;
    REG(SIO_BASE + SIO_GPIO_OE_SET_OFFSET) = 1<<PIN24 | 1<<PIN25;

    uint32_t cmd_sequence[] = {0, 0, 1, (uint32_t) &_flash_base, (uint32_t) &_estack1, (uint32_t) blink};

    uint32_t seq = 0;
    while(seq < sizeof(cmd_sequence)/sizeof(cmd_sequence[0])){
        uint32_t cmd = cmd_sequence[seq];
        if(!cmd){
            while(REG(SIO_BASE + SIO_FIFO_ST_OFFSET) & SIO_FIFO_ST_VLD_BITS)
                (void) REG(SIO_BASE + SIO_FIFO_RD_OFFSET);
            __asm__ ("sev");
        }

        while(!(REG(SIO_BASE + SIO_FIFO_ST_OFFSET) & SIO_FIFO_ST_RDY_BITS));
        REG(SIO_BASE + SIO_FIFO_WR_OFFSET) = cmd;
        __asm__ ("sev");

        while(!(REG(SIO_BASE + SIO_FIFO_ST_OFFSET) & SIO_FIFO_ST_VLD_BITS));
        uint32_t response = REG(SIO_BASE + SIO_FIFO_RD_OFFSET);

        seq = cmd == response ? seq + 1 : 0;
    }
    blink();
}
