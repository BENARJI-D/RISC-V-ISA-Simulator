.globl _start

_start:

    li x10, 100                # (0x64)

    slli x11, x10, 0          # x11 = 0x64 << 0 = 0x64
    slli x12, x10, 1          # x12 = 0x64 << 1 = 0xC8
    slli x13, x10, 5          # x13 = 0x64 << 5 = 0xC80
    slli x14, x10, 31         # x14 = 0x64 << 31 = 0x20000000 (last valid shift)

    li x15, -100              # (0xFFFFFF9C)

    slli x16, x15, 0          # x16 = 0xFFFFFF9C << 0 = 0xFFFFFF9C
    slli x17, x15, 1          # x17 = 0xFFFFFF9C << 1 = 0xFFFFFF38
    slli x18, x15, 5          # x18 = 0xFFFFFF9C << 5 = 0xFFFFF380

    li x19, 0                 # (0x0)

    slli x20, x19, 0          # x20 = 0x0 << 0 = 0x0
    slli x21, x19, 5          # x21 = 0x0 << 5 = 0x0

    li x22, 0xFFFFFFFF         # (0xFFFFFFFF)

    slli x23, x22, 1          # x23 = 0xFFFFFFFF << 1 = 0xFFFFFFFE
    slli x24, x22, 31         # x24 = 0xFFFFFFFF << 31 = 0x80000000

    slli x0, x10, 5           # x0 = 0 (Result discarded)

