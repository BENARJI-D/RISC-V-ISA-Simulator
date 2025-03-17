.global _start
_start:


addi x2, x0, 50         # x2 = 50
addi x3, x0, 20         # x3 = 20
sub  x10, x2, x3        # x10 = x2 - x3 = 50 - 20 = 30 (0x1E)


sub  x11, x2, x2        # x11 = x2 - x2 = 50 - 50 = 0 (Zero)


li   x4, 0x80000000     # x4 = -2147483648 (Smallest 32-bit signed integer)
addi x5, x0, 1          # x5 = 1
sub  x12, x4, x5        # Expected: 0x7FFFFFFF (2147483647, wraps around due to underflow)


li   x6, 0x7FFFFFFF     # x6 = 2147483647 (Largest 32-bit signed integer)
li   x7, -1             # x7 = -1 (0xFFFFFFFF)
sub  x13, x6, x7        # Expected: 0x80000000 (-2147483648, overflow case)

    
li   x8, 0x12345678     # x8 = 0x12345678
sub  x14, x8, x0        # x14 = x8 - 0 = 0x12345678 (Should remain unchanged)


sub  x15, x0, x8        # x15 = 0 - x8 = -0x12345678 (Negative of the value)

    
li   x9, -100           # x9 = -100
li   x16, -200          # x16 = -200
sub  x17, x9, x16       # x17 = -100 - (-200) = -100 + 200 = 100 (0x64)

sub x18, x2, x9         # x18 = -50(0xFFFFFFCE)
 