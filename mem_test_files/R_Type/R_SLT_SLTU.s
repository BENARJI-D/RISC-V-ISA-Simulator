.global _start
_start:


    # --- Test Case 1: SLT and SLTU with Equal Values ---
    li x2, 0x12345678  # Load value 0x12345678 into x2
    li x3, 0x12345678  # Load value 0x12345678 into x3
    slt x4, x2, x3     # Expect: x4 = 0 (equal)
    sltu x5, x2, x3    # Expect: x5 = 0 (equal)

    # --- Test Case 2: SLT and SLTU with Negative and Positive Values ---
    li x6, 0x80000000  # Load negative value (0x80000000) into x6 (signed value)
    li x7, 0x12345678  # Load positive value into x7
    slt x8, x6, x7     # Expect: x8 = 1 (since 0x80000000 < 0x12345678)
    sltu x9, x6, x7    # Expect: x9 = 0 (since 0x80000000 is unsigned larger)

    # --- Test Case 3: SLT and SLTU with Positive and Negative Values ---
    li x10, 0x12345678  # Load value 0x12345678 into x10 (positive value)
    li x11, 0x80000000  # Load negative value (0x80000000) into x11
    slt x12, x10, x11   # Expect: x12 = 0 (since 0x12345678 > 0x80000000)
    sltu x13, x10, x11  # Expect: x13 = 1 (since 0x12345678 < 0x80000000 as unsigned)

    # --- Test Case 4: SLT and SLTU with 0 and Positive Number ---
    li x14, 0           # Load 0 into x14
    li x15, 0x12345678  # Load positive value into x15
    slt x16, x14, x15   # Expect: x16 = 1 (since 0 < 0x12345678)
    sltu x17, x14, x15  # Expect: x17 = 1 (since 0 < 0x12345678)

    # --- Test Case 5: SLT and SLTU with 0 and Negative Number ---
    li x18, 0           # Load 0 into x18
    li x19, 0x80000000  # Load negative value into x19
    slt x20, x18, x19   # Expect: x20 = 0 (since 0 > 0x80000000 in signed)
    sltu x21, x18, x19  # Expect: x21 = 1 (since 0 < 0x80000000 as unsigned)


    
    
