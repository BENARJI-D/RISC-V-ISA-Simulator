.globl _start

_start:

    li x10, 0xA5A5A5A5       # (0xA5A5A5A5)

    andi x11, x10, 0xFF      # x11 = 0xA5 (Lower 8 bits masked)
    andi x12, x10, 0         # x12 = 0 (AND with zero)
    andi x13, x10, 0x6FF     # x13 = 0xA5A (Lower 12 bits retained)
    andi x14, x10, 0x555     # x14 = 0x505 (Pattern match)
    andi x15, x10, -1        # x15 = 0xA5A5A5A5 (AND with all ones)
    andi x16, x10, 0x700     # x16 = 0x800 (MSB mask in 12-bit range)
    andi x17, x10, 0xF       # x17 = 0x5 (Lowest 4 bits masked)

    li x10, -100             # (0xFFFFFF9C)

    andi x18, x10, 0x7F      # x18 = 0x1C (Lower 7 bits masked)
    andi x19, x10, 0x7FF     # x19 = 0x39C (Max positive immediate)
    andi x20, x10, -2048     # x20 = 0xFFFFF800 (Min negative immediate)

    andi x0, x10, 0xAA       # x0 = 0 (Result discarded)

