.global _start
_start:

# Test Case 1: Zero XOR (Zero Property)
li x2, 5          # Load 5 into x2 
li x3, 0          # Load 0 into x3 
xor x4, x2, x3    # Expect: x4 = 5 (5 XOR 0 = 5)

# Test Case 2: Self XOR (Self Property)
li x5, 7          # Load 7 into x5 
xor x6, x5, x5    # Expect: x6 = 0 (7 XOR 7 = 0)

# Test Case 3: XOR of Different Non-Zero Values
li x7, 7          # Load 7 into x7 
li x8, 3          # Load 3 into x8 
xor x9, x7, x8    # Expect: x9 = 4 (7 XOR 3 = 4)

# Test Case 4: XOR with Maximum Value (Edge Case)
li x10, 0xFFFFFFFF  # Load max 32-bit value into x10 
li x11, 0xAAAAAAAA  # Load alternating bits into x11 
xor x12, x10, x11   # Expect: x12 = 0x55555555

# Test Case 5: XOR with Negative Values (Two's complement)
li x13, -5         # Load -5 into x13 
li x14, -3         # Load -3 into x14 
xor x15, x13, x14  # Expect: x15 = 6 (-5 XOR -3 = 6)

# Test Case 6: XOR with All Zeroes 
li x16, 0          # Load 0 into x16 
li x17, 0          # Load 0 into x17 
xor x18, x16, x17  # Expect: x18 = 0 (0 XOR 0 = 0)

# Test Case 7: XOR with Random Large Numbers
li x19, 0xABCDEF01  # Load large value into x19 
li x20, 0x12345678  # Load another large value into x20 
xor x21, x19, x20   # Expect: x21 = 0xB9800969

 # Test Case 8: XOR with Inverted Bits
li x22, 0x0FFFFFFF  # Load value with lower 28 bits set into x22 
li x23, 0xF0000000  # Load an inverted value into x23 
xor x24, x22, x23   # Expect: x24 = 0FFFFFFFF

# Test Case 9: XOR of a Value and Its Mask
li x25, 0xA5        # Load 0xA5 into x25 
li x26, 0xFF        # Load 0xFF into x26 
xor x27, x25, x26   # Expect: x27 = 0x5A (A5 XOR FF = 5A)

# Test Case 10: Commutativity Test (Testing commutative property)
li x28, 0x1F        # Load 0x1F into x28 
li x29, 0xF0        # Load 0xF0 into x29 
xor x30, x28, x29   # 
xor x31, x29, x28   # Expect: x30 == x31 (EF)


  
