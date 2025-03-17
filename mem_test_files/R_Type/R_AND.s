.global _start
_start:


# Case 1:  with all ones (should return the second operand)
li x2, 0xFFFFFFFF     # x2 = 0xFFFFFFFF
li x3, 0x12345678     # x3 = 0x12345678
and x4, x2, x3        # Expected: x4 = 0x12345678

# Case 2:  with all zeros (should return zero)
li x5, 0x00000000     # x5 = 0x00000000
li x6, 0x89ABCDEF     # x6 = 0x89ABCDEF
and x7, x5, x6        # Expected: x7 = 0x00000000

# Case 3:  with alternating bits
li x8, 0xAAAAAAAA     # x8 = 0xAAAAAAAA
li x9, 0x55555555     # x9 = 0x55555555
and x10, x8, x9       # Expected: x10 = 0x00000000

# Case 4:  with itself (should return the same number)
li x11, 0xDEADBEEF    # x11 = 0xDEADBEEF
and x12, x11, x11     # Expected: x12 = 0xDEADBEEF

# Case 5:  with negative numbers
li x13, 0x80000000    # x13 = -2147483648 (Min signed int)
li x14, 0x7FFFFFFF    # x14 = 2147483647 (Max signed int)
and x15, x13, x14     # Expected: x15 = 0x00000000 (No bits in common)

# Case 6:  two negative numbers
li x16, 0xFFFFFFF0    # x16 = -16 (0xFFFFFFF0)
li x17, 0xFFFFFF0F    # x17 = -241 (0xFFFFFF0F)
and x18, x16, x17     # Expected: x18 = 0xFFFFFF00

# Case 7:  with a single bit set
li x19, 0x00000001    # x19 = 1
li x20, 0xFFFFFFFE    # x20 = -2 (All bits except LSB set)
and x21, x19, x20     # Expected: x21 = 0x00000000


  