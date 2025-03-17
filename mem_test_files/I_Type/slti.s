.globl _start

_start:

    li x14, 100                # (0x64)

    slti x15, x14, 150         # x15 = 1 (0x64 < 0x96)
    slti x16, x14, -200        # x16 = 0 (0x64 >= -0xC8)
    slti x17, x14, 0           # x17 = 0 (0x64 >= 0x0)

    li x10, -100               # (0xFFFFFF9C)

    slti x11, x10, 50          # x11 = 1 (0xFFFFFF9C < 0x32)
    slti x12, x10, -200        # x12 = 0 (0xFFFFFF9C >= -0xC8)
    slti x13, x10, 0           # x13 = 1 (0xFFFFFF9C < 0x0)
    slti x20, x10, -50         # x20 = 1 (-100 < -50)
    slti x21, x10, -1          # x21 = 1 (-100 < -1)

    slti x0, x10, 50           # x0 = 0 (Result discarded)

