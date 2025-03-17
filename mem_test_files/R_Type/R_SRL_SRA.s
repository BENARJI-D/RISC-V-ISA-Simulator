.global _start
_start:


    # --- Test Case 1: SRL and SRA with Equal Shift Amounts (No Change in Value) ---
    li x2, 0x12345678  # Load value 0x12345678 into x2
    li x3, 4           # Load shift amount of 4 into x3
    srl x4, x2, x3     # Expect: x4 = 0x01234567
    sra x5, x2, x3     # Expect: x5 = 0x01234567 (same as SRL for positive value)

    # --- Test Case 2: SRL and SRA with Negative Value ---
    li x6, 0x80000000  # Load signed negative value 0x80000000 into x6
    li x7, 4           # Load shift amount of 4 into x7
    srl x8, x6, x7     # Expect: x8 = 0x08000000 (zero-fill on left)
    sra x9, x6, x7     # Expect: x9 = 0xF8000000 (sign-extend on left)

    # --- Test Case 3: SRL and SRA with Zero Value ---
    li x10, 0          # Load value 0 into x10
    li x11, 4          # Load shift amount of 4 into x11
    srl x12, x10, x11  # SRL: Expect: x12 = 0 (no change)
    sra x13, x10, x11  # SRA: Expect: x13 = 0 (no change)

    # --- Test Case 4: SRL and SRA with Shift Amount Exceeding 32 Bits (Shift by 32 or More) ---
    li x14, 0x12345678  # Load value 0x12345678 into x14
    li x15, 32          # Load shift amount of 32 into x15
    srl x16, x14, x15   # Expect: x16 = 0x12345678 (Take mod 32)
    sra x17, x14, x15   # Expect: x17 = 0x12345678 (Take mod 32)

    li x18, 0x12345678  # Load value 0x12345678 into x18
    li x19, 33          # Load shift amount of 33 into x19 (which is effectively 1 due to rs2 & 0x1F)
    srl x20, x18, x19   # Expect: x20 = 0x091A2B3C (shift by 1 bit)
    sra x21, x18, x19   # Expect: x21 = 0x091A2B3C (shift by 1 bit, same as SRL for positive)

    # --- Test Case 5: SRL and SRA with Maximum Positive Value ---
    li x22, 0x7FFFFFFF  # Load maximum positive value (signed) into x22
    li x23, 4           # Load shift amount of 4 into x23
    srl x24, x22, x23   # Expect: x24 = 0x07FFFFFF (no sign extension)
    sra x25, x22, x23   # Expect: x25 = 0x07FFFFFF (no sign extension)

    # --- Test Case 6: SRL and SRA with Larger Shift Amounts ---
    li x26, 0xFFFFFFFF  # Load value 0x0xFFFFFFFF into x30
    li x27, 30          # Load shift amount of 30 into x31
    srl x28, x26, x27   # Expect: x32 = 0x00000003 (shift by 30 bits)
    sra x29, x26, x27   # Expect: x33 = 0xFFFFFFFF (sign-extended left)


  