.globl _start

_start:

    # Load byte tests
    li x10, 0x1000
    sb x5, 0(x10)        # Store 0 in memory
    lb x11, 0(x10)       # x11 = 0x0 (positive value check)

    li x12, 0xFF         # Store -1 in memory (0xFF as byte)
    sb x12, 1(x10)
    lb x13, 1(x10)       # x13 = 0xFFFFFFFF (-1 sign-extended)

    li x14, 0x7F         # Store 127 in memory (maximum positive value)
    sb x14, 2(x10)
    lb x15, 2(x10)       # x15 = 0x7F

    li x16, 0x80         # Store -128 in memory (minimum negative value)
    sb x16, 3(x10)
    lb x17, 3(x10)       # x17 = 0xFFFFFF80 (-128 sign-extended)

    li x18, 0x01         # Store 1 in memory
    sb x18, 4(x10)
    lb x19, 4(x10)       # x19 = 0x1

    li x20, 0xFF         # Store 255 in memory (unsigned max)
    sb x20, 5(x10)
    lb x21, 5(x10)       # x21 = 0xFFFFFFFF (-1 when sign-extended)

    li x22, 0xA5         # Store random value 0xA5
    sb x22, 6(x10)
    lb x23, 6(x10)       # x23 = 0xFFFFFFA5 (sign-extended value)

    # Testing boundary condition at address wrap-around
    li x24, 0x7FFFFFFC
    sb x25, 0(x24)       # Store 0 at high address
    lb x26, 0(x24)       # x26 = 0x0 (boundary check)

    # Testing with x0 as destination register (result discarded)
    li x27, 0x100
    sb x27, 7(x10)
    lb x0, 7(x10)        # x0 = 0 (Result discarded)

