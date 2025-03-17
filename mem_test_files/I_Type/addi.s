.globl _start

_start:

    li x14, 100                # (0x64)

    addi x15, x14, 150         # x15 = 100 + 150 = 0xF6
    addi x16, x14, -200        # x16 = 100 - 200 = -100 (0xFFFFFF9C)
    addi x17, x14, 2047        # x17 = 100 + 2047 = 0x8FF
    addi x18, x14, -2048       # x18 = 100 - 2048 = -1948 (0xFFFFF824)

    li x10, 0x7FFFFFFF         # Largest positive 32-bit value
    addi x19, x10, 1           # x19 = 0x7FFFFFFF + 1 = 0x80000000 (Overflow)

    li x11, 0x80000000         # Largest negative 32-bit value
    addi x20, x11, -1          # x20 = 0x80000000 - 1 = 0x7FFFFFFF (Underflow)

    addi x0, x14, 100          # x0 = 0 (Result discarded)

