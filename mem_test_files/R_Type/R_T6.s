.global _start
_start:
ADDI x5, x0, 0xFFFFFFFF  # x5 = 0xFFFFFFFF (-1 in two's complement)        
ADDI x6, x0, 4   # x6 = 4 (shift amount)        
# Perform Logical Shift Right (SRL) 
SRL  x7, x5, x6  # x7 = x5 >> x6 (Logical shift, fills with 0s)        
# Perform Arithmetic Shift Right (SRA) 
SRA  x8, x5, x6  # x8 = x5 >> x6 (Arithmetic shift, fills with sign bit)     

 