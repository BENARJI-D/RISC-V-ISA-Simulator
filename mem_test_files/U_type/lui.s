.global _start

_start:
    # Test LUI with zero immediate
    lui t0, 0x0            # t0 = 0x00000000

    # Test LUI with maximum 20-bit immediate
    lui t1, 0xFFFFF        # t1 = 0xFFFFF000 (largest 20-bit value shifted)

    # Test LUI with a mid-range value
    lui t2, 0x12345        # t2 = 0x12345000

    # Test LUI with small value
    lui t3, 0x1            # t3 = 0x00001000

    # Exit program
    li a7, 10
   

