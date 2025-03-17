.global _start

_start:
    # Test AUIPC with zero immediate
    auipc t0, 0x0          # t0 = PC + 0x00000000

    # Test AUIPC with maximum 20-bit immediate
    auipc t1, 0xFFFFF      # t1 = PC + 0xFFFFF000

    # Test AUIPC with a mid-range value
    auipc t2, 0x12345      # t2 = PC + 0x12345000

    # Test AUIPC with small value
    auipc t3, 0x1          # t3 = PC + 0x00001000

    # Exit program
    li a7, 10


