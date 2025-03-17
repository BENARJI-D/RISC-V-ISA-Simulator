.globl _start

_start:

    # Store values in memory
    li x10, 0x1000       # Base address
    li x11, 0x1234       # Positive 16-bit value
    sh x11, 0(x10)       # Store 0x1234 at address 0x1000

    li x12, 0xFFFF       # Max 16-bit value
    sh x12, 2(x10)       # Store 0xFFFF at address 0x1002

    li x13, 0x0          # Min 16-bit value
    sh x13, 4(x10)       # Store 0x0000 at address 0x1004

    li x14, 0x8000       # Negative interpreted value (two's complement)
    sh x14, 6(x10)       # Store 0x8000 at address 0x1006

    li x15, 0x00FF       # Upper byte zero, lower byte non-zero
    sh x15, 8(x10)       # Store 0x00FF at address 0x1008

    li x16, 0xFF00       # Upper byte non-zero, lower byte zero
    sh x16, 10(x10)      # Store 0xFF00 at address 0x100A

    li x17, 0xFFFE       # All bits set except LSB
    sh x17, 12(x10)      # Store 0xFFFE at address 0x100C

    li x18, 0x0001       # All bits zero except LSB
    sh x18, 14(x10)      # Store 0x0001 at address 0x100E

    # Load Halfword Unsigned Tests
    lhu x19, 0(x10)       # x19 = 0x1234
    lhu x20, 2(x10)       # x20 = 0xFFFF
    lhu x21, 4(x10)       # x21 = 0x0000
    lhu x22, 6(x10)       # x22 = 0x8000 (interpreted as unsigned)
    lhu x23, 8(x10)       # x23 = 0x00FF
    lhu x24, 10(x10)      # x24 = 0xFF00
    lhu x25, 12(x10)      # x25 = 0xFFFE
    lhu x26, 14(x10)      # x26 = 0x0001
