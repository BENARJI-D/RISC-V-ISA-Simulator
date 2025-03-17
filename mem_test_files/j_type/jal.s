.global _start
_start:
    # --- Test Case 1: Valid Jump to Aligned Address (rd = x10) ---
    li x10, 0            # Initialize x10
    jal x10, valid_jump  # Jump to valid_jump, return address stored in x10
    li x11, 1            # This executes only if we return successfully

valid_jump:
    li x12, 2            # Indicate execution of valid_jump
    jalr x0, 0(x10)      # Return using x10

    # --- Test Case 2: Valid Jump to Another Aligned Label (rd = x13) ---
    jal x13, another_jump # Jump to another_jump, return address stored in x13
    li x14, 3            # This should execute only if return works

another_jump:
    li x15, 4            # Indicate execution of another_jump
    jalr x0, 0(x13)      # Return using x13

    # --- Test Case 3: Misaligned Jump (Handled Exception) ---
    # This should trigger an exception if misaligned jumps are not allowed
    jal x16, misaligned_jump  # x16 should store return address
    li x17, 5                 # Should execute only if exception is handled
    
    .align 2            # Ensure the label is misaligned (this should result in a misalignment)
misaligned_jump:
    li x18, 6            # Indicate execution of misaligned jump (if allowed)
    jalr x0, 0(x16)      # Return using x16




#Expected Result x10: Return address (valid_jump was executed)
#Expected Result x11: 1 (Return from valid_jump was succesfull)
#Expected Result x12: 2 (valid_jump was executed)
#Expected Result x13: Return address (another_jump was executed)
#Expected Result x14: 3 (Return from another_jump was succesfull)
#Expected Result x15: 4 (another_jump was executed)
#Expected Result x16: Return address (misaligned_jump attempted)
#Expected Result x17: 5 (if no crash) (Misaligned jump handled)
#Expected Result x18: 6 (if reached) (misaligned_jump was executed)


