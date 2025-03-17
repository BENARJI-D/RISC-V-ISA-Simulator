.globl _start

_start:

    li x14, 100                # (0x64)

    ori x15, x14, 50           # x15 = 0x64 | 0x32 = 0x76
    ori x16, x14, 0            # x16 = 0x64 | 0x0 = 0x64
    ori x17, x14, 2047        # x17 = 0x64 | 0xFFF = 0xFFF
    ori x18, x14, -50          # x18 = 0x64 | 0xFFFFFFCE = 0xFFFFFFF6

    li x10, -100               # (0xFFFFFF9C)

    ori x11, x10, 50           # x11 = 0xFFFFFF9C | 0x32 = 0xFFFFFFBE
    ori x12, x10, 0            # x12 = 0xFFFFFF9C | 0x0 = 0xFFFFFF9C
    ori x13, x10, 2047        # x13 = 0xFFFFFF9C | 0xFFF = 0xFFFFFFFF
    ori x20, x10, -50          # x20 = 0xFFFFFF9C | 0xFFFFFFCE = 0xFFFFFFDE

    ori x21, x14, 20         # x21 = 0x64 | 0x64 = 0x64
    ori x22, x14, 2047        # x22 = 0x64 | 0xFFF = 0xFFF

    ori x0, x10, 50            # x0 = 0 (Result discarded)

