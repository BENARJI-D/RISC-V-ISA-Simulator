.globl _start

_start:

    li x14, 100                 # (0x64)

    srli x15, x14, 1           # x15 = 0x32 (100 >> 1)
    srli x16, x14, 0           # x16 = 0x64 (100 >> 0)
    srli x17, x14, 31          # x17 = 0x0 (100 >> 31)

    li x10, -100               # (0xFFFFFF9C)

    srli x11, x10, 1           # x11 = 0x7FFFFFFE (0xFFFFFF9C >> 1, zero extended)
    srli x12, x10, 31          # x12 = 0x1 (most significant bit shifted out)

    li x13, 0xFFFFFFFF         # Maximum unsigned 32-bit value
    srli x18, x13, 4           # x18 = 0x0FFFFFFF (0xFFFFFFFF >> 4)

    srli x0, x10, 1            # x0 = 0 (Result discarded)

