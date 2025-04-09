.set IO_IRQ_BANK0_IRQn, 37
.section .init

.word _estack
.word prog
.skip 4*(IO_IRQ_BANK0_IRQn - 2)
.word IO_IRQ_BANK0_Handler

.word 0xffffded3
.word 0x10210142
.word 0x000001ff
.word 0x00000000
.word 0xab123579
