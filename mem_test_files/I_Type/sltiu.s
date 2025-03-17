.globl _start

_start:

    li x14, 100                # (0x64)

    sltiu x15, x14, 150        # x15 = 1 (0x64 < 0x96)
    sltiu x16, x14, 0          # x16 = 0 (0x64 >= 0x0)
    sltiu x17, x14, -200       # x17 = 1 (0x64 < 0xFFFFFF38)

    li x10, -100               # (0xFFFFFF9C)

    sltiu x11, x10, 50         # x11 = 0 (0xFFFFFF9C >= 0x32)
    sltiu x12, x10, -200       # x12 = 0 (0xFFFFFF9C >= 0xFFFFFF38)
    sltiu x13, x10, 0          # x13 = 0 (0xFFFFFF9C >= 0x0)

    sltiu x0, x10, 50          # x0 = 0 (Result discarded)

