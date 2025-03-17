.globl _start

_start:
    # Store known data for testing
    li x10, 0x1000       # Base address for memory

    li x11, 0x1234       # Positive halfword value
    sh x11, 0(x10)       # Store 0x1234 at address 0x1000

    li x12, -0x1234      # Negative halfword value (sign extension test)
    sh x12, 2(x10)       # Store -0x1234 at address 0x1002

    li x13, 0x7FFF       # Maximum positive halfword
    sh x13, 4(x10)       # Store 0x7FFF at address 0x1004

    li x14, -0x8000      # Minimum negative halfword
    sh x14, 6(x10)       # Store -0x8000 at address 0x1006

    # LH Tests
    lh x15, 0(x10)       # x15 = 0x1234
    lh x16, 2(x10)       # x16 = 0xFFFFEDCC (sign-extended -0x1234)
    lh x17, 4(x10)       # x17 = 0x7FFF
    lh x18, 6(x10)       # x18 = 0xFFFF8000 (sign-extended -0x8000)

    # Zero register discard test
    lh x0, 0(x10)        # x0 = 0 (Result discarded)

