.globl _start

_start:

    li x10, 0x1000
    addi x11, x10, 4
    jalr x12, x11, 0x4  # x12 = 0x1004 + 0x4 = 0x1008

    li x13, 0x2000
    addi x14, x13, -4
    jalr x15, x14, -0x4  # x15 = 0x1FFC - 0x4 = 0x1FF8

    li x16, 0x3000
    jalr x17, x16, 0     # x17 = 0x3000 + 0 = 0x3000

    li x18, 0x4000
    addi x19, x18, 2
    jalr x20, x19, 0x2  # x20 = 0x4002 + 0x2 = 0x4004

    li x21, 0x5000
    jalr x0, x21, 0x10  # x0 = 0 (Result discarded)

    li x22, 0x0
    jalr x23, x22, 0x8  # x23 = 0x0 + 0x8 = 0x8

    li x24, 0x7FFFFFF0
    jalr x25, x24, 0x20 # x25 = 0x7FFFFFF0 + 0x20 = 0x80000010

    li x26, 0x80000010
    jalr x27, x26, -0x20 # x27 = 0x80000010 - 0x20 = 0x7FFFFFF0

    li x28, 0x3000
    jalr x29, x28, 0x300 # x29 = 0x3000 + 0x300 = 0x3300

    li x30, 0x7000
    jalr x30, x30, 0    # x30 = 0x7000 (infinite loop test)

