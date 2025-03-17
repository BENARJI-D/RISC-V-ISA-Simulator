.globl _start
_start:
    # 1. Store and load max byte (0x7F)
    li t0, 0x1004      # Memory address
    li t1, 0x7F        # Load max positive byte
    sb t1, 0(t0)       # Store byte
    lb t2, 0(t0)       # Load byte back

    # 2. Store and load min byte (0x80)
    li s0, 0x80        # Load min signed byte (-128)
    sb s0, 0(t0)       # Store byte
    lb s1, 0(t0)       # Load back

    # 3. Store and load zero
    li a3, 0x00        # Load 0
    sb a3, 0(t0)       # Store byte
    lb a4, 0(t0)       # Load back

    # 4. Store and load arbitrary value (0x42)
    li t3, 0x42        # Load arbitrary byte
    sb t3, 0(t0)       # Store byte
    lb t4, 0(t0)       # Load back

    # 5. Overwrite test
    li s2, 0xFF        # Load -1 (0xFF in byte)
    sb s2, 0(t0)       # Store byte
    lb s3, 0(t0)       # Load back

    # 6. Unaligned memory access test (this should work fine for SB)
    li t5, 0x1005      # Any address (SB works with unaligned addresses)
    li t6, 0xAB        # Some test value
    sb t6, 0(t5)       # Store byte
    lb a5, 0(t5)       # Load back


