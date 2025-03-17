.globl _start

_start:

addi x5, x0, 0x10            # Load immediate 0x10  into register x5 
addi x6, x0, 0x10            # Load immediate 0x10  into register x6 
beq  x5, x6, branch          #  If x5 == x6, jump to label branch 
addi x7, x0, 1               # If branch is not taken, x7 is set to 1 (this should be skipped)   
branch: ADDI x7, x0, 2       # If branch is taken, x7 is set to 2     

