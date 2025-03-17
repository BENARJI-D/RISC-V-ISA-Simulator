.globl _start

_start:

    li x14, 100                # (0x64)

    srai x15, x14, 1          # x15 = 100 >> 1 = 50 (0x32)
    srai x16, x14, 31         # x16 = 100 >> 31 = 0 (0x0)

    li x10, -100               # (0xFFFFFF9C)

    srai x11, x10, 1          # x11 = -100 >> 1 = -50 (0xFFFFFFCE)
    srai x12, x10, 31         # x12 = -100 >> 31 = -1 (0xFFFFFFFF)

    li x13, 0                 # (0x0)

    srai x17, x13, 5          # x17 = 0 >> 5 = 0 (0x0)

    li x18, -1                # (0xFFFFFFFF)

    srai x19, x18, 1          # x19 = -1 >> 1 = -1 (0xFFFFFFFF)
    srai x20, x18, 31         # x20 = -1 >> 31 = -1 (0xFFFFFFFF)

    srai x0, x18, 1           # x0 = 0 (Result discarded)

