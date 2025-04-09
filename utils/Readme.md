# utils

To upload the program to the MCU you need to convert it to a UF2 file. This is done by the bin2uf2.py in the utils directory. It is simpler than its RP2040 counterpart
because it is no longer necessary that byte 252-255 of the binary contain a CRC32 checksum of the preceding bytes (usually the second-stage bootloader code).
The family ID is 0xe48bff57, see [RP2350], section 5.5.3, which can be used for both ARM and RISC-V images.
