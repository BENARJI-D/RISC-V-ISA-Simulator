.global _start
_start:
ADDI x5, x0, 0x7FF  # x5 =  2147483647 (Max positive value) 
ADDI x6, x0, 0x1          
ADD  x7, x5, x6  #Expected:0x800          

