.globl _start

_start:

    # Store data in memory
    li x10, 0x1000
    sw x10, 0(x10)        # Store 0x1000 at address 0x1000

    li x11, -100
    sw x11, 4(x10)        # Store -100 at address 0x1004

    li x12, 0x7FFFFFFF
    sw x12, 8(x10)        # Store 0x7FFFFFFF at address 0x1008

    li x13, 0x80000000
    sw x13, 12(x10)       # Store 0x80000000 at address 0x100C

    li x14, 0             # Zero for result discard test
    sw x14, 16(x10)       # Store 0 at address 0x1010

    li x15, 0x12340000
    sw x15, 20(x10)       # Store 0x12340000 at address 0x1014

    li x16, 0x00001234
    sw x16, 24(x10)       # Store 0x00001234 at address 0x1018

    # LW Test cases
    lw x20, 0(x10)        # x20 = 0x1000
    lw x21, 4(x10)        # x21 = 0xFFFFFF9C (-100)
    lw x22, 8(x10)        # x22 = 0x7FFFFFFF
    lw x23, 12(x10)       # x23 = 0x80000000
    lw x0, 16(x10)        # x0 = 0 (discarded result)
    lw x24, 20(x10)       # x24 = 0x12340000
    lw x25, 24(x10)       # x25 = 0x00001234

