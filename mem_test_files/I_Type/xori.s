.globl _start

_start:

    li x14, 0x12345678         # Load initial value (0x12345678)

    xori x15, x14, 0          # x15 = 0x12345678 ^ 0 = 0x12345678
    xori x16, x14, 0x7FF      # x16 = 0x12345678 ^ 0x7FF = 0x12345F87
    xori x17, x14, -1         # x17 = 0x12345678 ^ 0xFFF = 0x12345F87
    xori x18, x14, 0x1F0      # x18 = 0x12345678 ^ 0x1F0 = 0x12345788
    xori x19, x14, -0x1F0     # x19 = 0x12345678 ^ 0xFE10 = 0x12345968

    li x10, -100              # Load negative value (0xFFFFFF9C)

    xori x20, x10, 0x50       # x20 = 0xFFFFFF9C ^ 0x50 = 0xFFFFFFCC
    xori x21, x10, -1         # x21 = 0xFFFFFF9C ^ 0xFFF = 0xFFFFFF63
    xori x22, x10, 0          # x22 = 0xFFFFFF9C ^ 0 = 0xFFFFFF9C

    xori x0, x10, 0x50        # x0 = 0 (Result discarded)

