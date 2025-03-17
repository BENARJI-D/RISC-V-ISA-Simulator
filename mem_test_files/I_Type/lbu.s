.globl _start

_start:
    # Store values in memory
    li x10, 0x1000      # Base address
    sb x20, 0(x10)      # Store 0x20 at address 0x1000 (positive value)
    li x21, -1          # Load -1 (0xFF in 8-bit) into x21
    sb x21, 1(x10)      # Store 0xFF at address 0x1001 (interpreted as 255)
    li x22, 0           # Load 0 (boundary test)
    sb x22, 2(x10)      # Store 0x00 at address 0x1002
    li x23, 0x7F        # Load maximum positive 8-bit value
    sb x23, 3(x10)      # Store 0x7F at address 0x1003

    # Load Byte Unsigned Tests
    lbu x24, 0(x10)     # x24 = 0x20 (Positive value test)
    lbu x25, 1(x10)     # x25 = 0xFF (Unsigned interpretation of -1)
    lbu x26, 2(x10)     # x26 = 0x00 (Zero value test)
    lbu x27, 3(x10)     # x27 = 0x7F (Maximum positive value in 8-bit range)

    # Negative Offset Test
    addi x28, x10, 4    # Point to 0x1004
    sb x21, -1(x28)     # Store 0xFF at address 0x1003 (using negative offset)
    lbu x29, -1(x28)    # x29 = 0xFF (Unsigned interpretation of -1)
