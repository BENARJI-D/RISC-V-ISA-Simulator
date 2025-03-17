.global _start
_start:
ADDI x5, x0, -1 # x5 = 0xFFFFFFFF (-1 in signed, large positive in unsigned)       
ADDI x6, x0, 1  # x6 = 0x00000001 (1 in both signed and unsigned)    
SLT  x7, x5, x6 # Signed comparison: (-1 < 1) -> x7 = 1      
SLTU x8, x5, x6 # Unsigned comparison: (0xFFFFFFFF < 0x00000001) -> x8 = 0     

 

