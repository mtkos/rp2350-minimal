.set PWM_IRQ_WRAP_0_IRQn,24
.section .init

.word _estack
.word prog
.skip 4*(PWM_IRQ_WRAP_0_IRQn - 2)
.word PWM_IRQ_WRAP_Handler

.word 0xffffded3
.word 0x10210142
.word 0x000001ff
.word 0x00000000
.word 0xab123579
