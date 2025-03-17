.globl _start
_start:
    # 1. Store and load -1 (All bits set)
    li t0, 0x1000       # Memory address
    li t1, 0xFFFFFFFF   # Load -1
    sw t1, 0(t0)        # Store word
    lw t2, 0(t0)        # Load word back into t2

    # 2. Store and load 0
    li s0, 0x00000000   # Load 0
    sw s0, 0(t0)        # Store word
    lw s1, 0(t0)        # Load back into s1

    # 3. Store and load max positive int (0x7FFFFFFF)
    li a3, 0x7FFFFFFF   # Load max positive int
    sw a3, 0(t0)        # Store word
    lw a4, 0(t0)        # Load back into a4

    # 4. Store and load min negative int (0x80000000)
    li t3, 0x80000000   # Load min negative int
    sw t3, 0(t0)        # Store word
    lw t4, 0(t0)        # Load back into t4

    # 5. Store and load a random value (0x12345678)
    li s2, 0x12345678   # Load arbitrary value
    sw s2, 0(t0)        # Store word
    lw s3, 0(t0)        # Load back into s3

    # 6. Overwrite test (write new value and read back)
    li a5, 0xDEADBEEF   # Load a new value
    sw a5, 0(t0)        # Store word
    lw a6, 0(t0)        # Load back into a6

    # 7. Unaligned memory access test (may cause exception)
    li t5, 0x1001       # Unaligned address (not allowed in strict RISC-V)
    li t6, 0xABCDEF12   # Some test value
    sw t6, 0(t5)        # Store word (might fail)
    lw s4, 0(t5)        # Attempt to load (might fail)


