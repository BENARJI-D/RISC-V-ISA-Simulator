.globl _start
_start:
    # 1. Store and load max halfword (0x7FFF)
    li t0, 0x1002       # Memory address (aligned)
    li t1, 0x7FFF       # Load max positive halfword
    sh t1, 0(t0)        # Store halfword
    lh t2, 0(t0)        # Load halfword back

    # 2. Store and load min halfword (0x8000)
    li s0, 0x8000       # Load min negative halfword
    sh s0, 0(t0)        # Store halfword
    lh s1, 0(t0)        # Load back

    # 3. Store and load zero
    li a3, 0x0000       # Load 0
    sh a3, 0(t0)        # Store halfword
    lh a4, 0(t0)        # Load back

    # 4. Store and load arbitrary value (0x1234)
    li t3, 0x1234       # Load arbitrary halfword
    sh t3, 0(t0)        # Store halfword
    lh t4, 0(t0)        # Load back

    # 5. Overwrite test
    li s2, 0xABCD       # Load a new halfword
    sh s2, 0(t0)        # Store halfword
    lh s3, 0(t0)        # Load back

    # 6. Unaligned memory access test (may cause exception)
    li t5, 0x1001       # Unaligned address
    li t6, 0x5678       # Some test value
    sh t6, 0(t5)        # Store halfword (might fail)
    lh s4, 0(t5)        # Attempt to load (might fail)


