.global _start
_start:

    # Test Case : Simple Jump with Return Address in x1
    li x1, 0              # Initialize x1 to 0
    jal x1, target_label  # Jump to target_label, return address stored in x1
    li x2, 1              # This executes only if we return successfully

target_label:
    li x3, 2              # This executes after the jump
    jalr x0, 0(x1)        # Return using x1 (address stored in x1)




    # Test Case : Jump with Return Address in Register x5
    li x5, 0              # Initialize x5 to 0
    jal x5, label_a       # Jump to label_a, return address stored in x5
    li x6, 3              # This executes only if we return successfully

label_a:
    li x7, 4              # This executes after the jump
    jalr x0, 0(x5)        # Return using x5 (address stored in x5)





    # Test Case : Jump with Negative Offset (Negative Jump)
    li x1, 0              # Initialize x1 to 0
    jal x1, negative_jump # Jump to negative_jump, return address stored in x1
    li x2, 7              # This executes only if we return successfully

negative_jump:
    li x3, 8              # This executes after the jump
    jalr x0, 0(x1)        # Return using x1 (address stored in x1)





    # Test Case : Jump with Return Address in x10
    li x10, 0             # Initialize x10 to 0
    jal x10, label_b      # Jump to label_b, return address in x10
    li x11, 9             # This executes only if we return successfully

label_b:
    li x12, 10            # This executes after the jump
    jalr x0, 0(x10)       # Return using x10 (address stored in x10)






    # Test Case : Self-loop with JAL (Infinite Loop)
    li x1, 0              # Initialize x1 to 0
self_loop:
    jal x0, self_loop     # Infinite loop (repeated jump to self_loop)



