.global _start
_start:
ADDI x5, x0, 0x10    # x5 = 0x10 
ADDI x6, x0, 0x5    # x6 = 0x5 
SLL  x7, x6, x5   # x7 = x6 << (x5 & 0x1F)  (Logical shift left) 

