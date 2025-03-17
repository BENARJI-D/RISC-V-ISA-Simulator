.global _start
_start:

# Test Case 1: Shift by 0 
li x2, 0x12345678  # Load value 0x12345678 into x2
li x3, 0           # Load 0 into x3 (shift by 0)
sll x4, x2, x3     # Expect: x4 = 0x12345678 (no shift)
    
# Test Case 2: Shift by 1 
li x5, 0x12345678  # Load value 0x12345678 into x5
li x6, 1           # Load 1 into x6 (shift by 1)
sll x7, x5, x6     # Expect: x7 = 0x2468ACF0 (shift left by 1)

# Test Case 3: Shift by 31 (Max shift for 32-bit value)
li x8, 0x1         # Load 0x1 into x8 (binary 00000000000000000000000000000001)
li x9, 31          # Load 31 into x9 (shift by 31)
sll x10, x8, x9    # Expect: x10 = 0x80000000 (shift left by 31, MSB set)

# Test Case 4: Shift by 32 (Shift by 0 since rs2 && 0x1F)
li x11, 0x12345678 # Load value 0x12345678 into x11
li x12, 32         # Load 32 into x12 (shift by 32 = shift by 0)
sll x13, x11, x12  # Expect: x13 = 0x12345678 (we take mod 32 of rs2 value)

# Test Case 5: Shift with the most significant bit set
li x14, 0x80000000 # Load value 0x80000000 into x14 (MSB set)
li x15, 4          # Load 4 into x15 (shift by 4)
sll x16, x14, x15  # Expect: x16 = 0x00000000 (MSB shifted out, result is 0)

# Test Case 6: Shift with the value 0 (No change expected)
li x17, 0          # Load 0 into x17
li x18, 10         # Load 10 into x18 (shift by 10)
sll x19, x17, x18  # Expect: x19 = 0 (no shift since value is 0)

# Test Case 7: Shifting a large number (overflow check)
li x20, 0xFFFFFFFF # Load value 0xFFFFFFFF into x20
li x21, 1          # Load 1 into x21 (shift by 1)
sll x22, x20, x21  # Expect: x22 = 0xFFFFFFFE (shift left by 1)

# Test Case 8: Shift with non-zero value where high bits are shifted out
li x23, 0x80000001 # Load value 0x80000001 into x23 (MSB set)
li x24, 31         # Load 31 into x24 (shift by 31)
sll x25, x23, x24  # Expect: x25 = 0x80000000 (only MSB remains, others shifted out)

# Test Case 9: Shift a value larger than the width of the register (should result in shift by 1)
li x26, 0x12345678 # Load value 0x12345678 into x26
li x27, 33         # Load 33 into x27 (shift by 33, greater than 32-bit width so take mod 32)
sll x28, x26, x27  # Expect: x7 = 0x2468ACF0 (shift left by 1)

# Test Case 10: Shift using maximum value in rs2 (shift by 31)
li x29, 0x7FFFFFFF # Load value 0x7FFFFFFF into x29
li x30, 31         # Load 31 into x30 (maximum shift for 32-bit)
sll x31, x29, x30  # x31 = x29 << 31, Expect: x31 = 0x80000000 (shift left by 31)
    
