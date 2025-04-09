#include <stdint.h>

#include "../include/common.h"
#include "../include/resets.h"
#include "../include/io_bank0.h"
#include "../include/pads_bank0.h"
#include "../include/pio.h"
#include "../include/xosc.h"
#include "../include/clocks.h"

#include "ws2812d.pio.h"

uint32_t x = 0x000001;

extern uint32_t _sidata, _sdata, _edata, _ebss, _sbss;

void flash_to_ram(){

    uint32_t *p_idata = &_sidata;

    for(uint32_t *p_data = &_sdata; p_data < &_edata; *p_data++ = *p_idata++);
    for(uint32_t *p_bss = &_sbss; p_bss < &_ebss; *p_bss++ = 0);
}

#define NUM_LEDS 8

uint32_t colors[NUM_LEDS] = {0x000001, 0x000002, 0x000004, 0x000008, 0x010000, 0x020000, 0x040000, 0x080000};

void send_colors(){

    for(int i = 0; i < NUM_LEDS; i++){
        while((REG(PIO0_BASE + PIO_FSTAT_OFFSET) & (1<<(PIO_FSTAT_TXFULL_LSB + 0))));
        REG(PIO0_BASE + PIO_TXF0_OFFSET) = colors[i];
    }
}

void cycle(){

    uint32_t c = colors[0];
    for(int i = 0; i < NUM_LEDS - 1; i++)
        colors[i] = colors[i + 1];
    colors[NUM_LEDS - 1] = c;
}

void prog(){

   flash_to_ram();

    for(int i = 0; i < NUM_LEDS; i++)
        colors[i] <<= 8;

    REG(RESETS_BASE + RESETS_RESET_OFFSET + REG_ALIAS_CLR_BITS) = (RESETS_RESET_IO_BANK0_BITS | RESETS_RESET_PIO0_BITS);
    while((~(REG(RESETS_BASE + RESETS_RESET_DONE_OFFSET)) & (RESETS_RESET_DONE_IO_BANK0_BITS | RESETS_RESET_DONE_PIO0_BITS)));

    REG(XOSC_BASE + XOSC_CTRL_OFFSET) = XOSC_CTRL_ENABLE_VALUE_ENABLE<<XOSC_CTRL_ENABLE_LSB;
    while(!(REG(XOSC_BASE + XOSC_STATUS_OFFSET) & XOSC_STATUS_STABLE_BITS));

    REG(CLOCKS_BASE + CLOCKS_CLK_REF_CTRL_OFFSET) = CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC;
    while(REG(CLOCKS_BASE + CLOCKS_CLK_REF_SELECTED_OFFSET) != 1<<CLOCKS_CLK_REF_CTRL_SRC_VALUE_XOSC_CLKSRC);

    REG(IO_BANK0_BASE + IO_BANK0_GPIO0_CTRL_OFFSET) = GPIO_FUNC_PIO0;
    REG(PADS_BANK0_BASE + PADS_BANK0_GPIO0_OFFSET + REG_ALIAS_CLR_BITS) = PADS_BANK0_GPIO0_ISO_BITS;

    uint32_t *pio_instructions = (uint32_t *)(PIO0_BASE + PIO_INSTR_MEM0_OFFSET);
    for(int i = 0; i < sizeof(ws2812d_program_instructions)/sizeof(ws2812d_program_instructions[0]); i++)
        pio_instructions[i] = ws2812d_program_instructions[i];

    REG(PIO0_BASE + PIO_SM0_EXECCTRL_OFFSET) = ws2812d_wrap<<PIO_SM0_EXECCTRL_WRAP_TOP_LSB | ws2812d_wrap_target<<PIO_SM0_EXECCTRL_WRAP_BOTTOM_LSB;
    REG(PIO0_BASE + PIO_SM0_PINCTRL_OFFSET + REG_ALIAS_SET_BITS) = 0<<PIO_SM0_PINCTRL_SET_BASE_LSB | 1<<PIO_SM0_PINCTRL_SET_COUNT_LSB;
    REG(PIO0_BASE + PIO_SM0_SHIFTCTRL_OFFSET) = PIO_SM0_SHIFTCTRL_AUTOPULL_BITS | 24<<PIO_SM0_SHIFTCTRL_PULL_THRESH_LSB;
    REG(PIO0_BASE + PIO_CTRL_OFFSET) = 1<<(PIO_CTRL_SM_ENABLE_LSB + 0);//enable state machine 0

    for(;;){
        send_colors();
        cycle();
        for(volatile int i = 200000; i; i--);
    }
}
