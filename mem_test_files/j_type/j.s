    .section .text
    .globl _start

_start:
    # Test JAL - Jump and Link
    jal x1, target1   # Jump to target1, store return address in x1
    nop               # This should be skipped if JAL works

    # Test JALR - Jump and Link Register
    la x2, target2    # Load address of target2 into x2
    jalr x3, 0(x2)    # Jump to target2, store return address in x3
    nop               # This should be skipped if JALR works

    # Test JALR with nonzero immediate
    la x6, target3    # Load address of target3 into x6
    jalr x7, 4(x6)    # Jump to target3 + 4, store return address in x7
    nop               

    # End of test
    j end_test

# Target addresses for jumps
target1:
    li x4, 1          # If we land here, set x4 to 1
    jalr x0, x1, 0    #go to la x2,  target2

target2:
    li x5, 2          # If we land here, set x5 to 2
    jalr x0, x3, 0    #return

target3:
    li x8, 3          # this instruction should be skipped
    jalr x0, x7, 0    # Return to caller

end_test:
    # End the test with an infinite loop
    nop
    j end_test

