.global _start
_start:
ADDI x5, x0, 0x80  # x5 = 0x80000000 (-2147483648, Min signed 32-bit value) 
ADDI x6, x0, 0x00000001  # x6 = 0x00000001 (1) 
SUB  x7, x5, x6   # x7 = x5 - x6 (Expected: 0x7FFFFFFF, which is +2147483647 in signed 2's complement)      
 
