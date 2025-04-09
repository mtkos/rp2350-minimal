#define REG(x) (*((volatile uint32_t *)(x)))
#define REG_ALIAS_RW_BITS 0x0000
#define REG_ALIAS_XOR_BITS 0x1000
#define REG_ALIAS_SET_BITS 0x2000
#define REG_ALIAS_CLR_BITS 0x3000
