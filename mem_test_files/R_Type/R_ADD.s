.global _start
_start:

addi x2, x0, 0x10    # x2 = 0x10 
addi x3, x0, 0x5    # x3 = 0x5 
add  x11, x2, x3   #x11 = 0x15

li x4, 0x7FFFFFFF  # x4 =  2147483647 (Max positive value)
addi x5, x0, 0x1          
add  x12, x4, x5  #Expected:0x80000000  (Overflow)

add  x0, x4, x5   #x0 = 0x0


li x6, 0x80000000  # x6 =  (smallest 32-bit signed integer) 
addi x7, x0, -1         
add  x13, x6, x7  #Expected:0x7FFFFFFF  (underflow)

li x8, -157
add x14, x4, x8   # x13 = 2147483497 = 0x7FFFFF6D

li x9, -2147483648
li x10, -1
add x15, x9, x10  # Expected: 0x7FFFFFFF (overflow case)






