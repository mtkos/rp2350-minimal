#include <stdint.h>

#include "../include/common.h"
#include "../include/resets.h"
#include "../include/sio.h"
#include "../include/io_bank0.h"
#include "../include/pads_bank0.h"
#include "../include/pwm.h"
#include "../include/m33.h"

#define PWM_IRQ_WRAP_0_IRQn 8

#define CH0 0

#define PIN25 25

#define CPU_FREQ 11000000
#define DIVIDER CPU_FREQ/0x10000 //to get 1Hz pulses, because the counter is 16 bits

void PWM_IRQ_WRAP_Handler() __attribute__((interrupt));

void PWM_IRQ_WRAP_Handler(){

    REG(PWM_BASE + PWM_INTR_OFFSET) = 1<<CH0;
}

void prog(){

    REG(RESETS_BASE + RESETS_RESET_OFFSET + REG_ALIAS_CLR_BITS) = RESETS_RESET_IO_BANK0_BITS | RESETS_RESET_PADS_BANK0_BITS | RESETS_RESET_PWM_BITS;
    while((~(REG(RESETS_BASE + RESETS_RESET_DONE_OFFSET)) & (RESETS_RESET_DONE_IO_BANK0_BITS | RESETS_RESET_DONE_IO_BANK0_BITS | RESETS_RESET_DONE_PWM_BITS)));

    REG(IO_BANK0_BASE + IO_BANK0_GPIO25_CTRL_OFFSET) = GPIO_FUNC_SIO;
    REG(PADS_BANK0_BASE + PADS_BANK0_GPIO25_OFFSET + REG_ALIAS_CLR_BITS) = PADS_BANK0_GPIO0_ISO_BITS;
    REG(SIO_BASE + SIO_GPIO_OE_SET_OFFSET) = 1<<PIN25;

    REG(PWM_BASE + PWM_CH0_DIV_OFFSET) = DIVIDER<<PWM_CH0_DIV_INT_LSB;

    REG(PWM_BASE + PWM_IRQ0_INTE_OFFSET) = 1<<CH0;
    REG(PPB_BASE + M33_NVIC_ISER0_OFFSET) = 1<<PWM_IRQ_WRAP_0_IRQn;

    REG(PWM_BASE + PWM_EN_OFFSET) = 1<<CH0;

    for(;;){
        __asm__ ("wfi");
        REG(SIO_BASE + SIO_GPIO_OUT_XOR_OFFSET) = 1<<PIN25;
    }
}
