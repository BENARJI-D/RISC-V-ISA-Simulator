#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <map> 
#include <math.h>
#include <iostream>
#include <string.h>
#include <set>

using namespace std;

std::set<uint32_t> breakpoints;                 // Stores multiple breakpoint addresses
std::map<uint32_t, uint8_t> watchpoints_memory; // Maps watched memory addresses to their last known values
std::map<int, uint32_t> watchpoints_register;   // Maps watched registers to their last known values
map<uint32_t, uint8_t> memory; // Ordered map for memory
uint32_t registers[32]; // Simulated registers
uint32_t pc; // Program Counter initialized to 0


// Function to perform sign extension dynamically
uint32_t sign_extend(int value,int bit_count) {
    uint32_t temp = value;
    if (bit_count >= 32) {
        return value;
    }

    uint32_t sign_bit = value>>(bit_count-1);
    if (sign_bit) {
        value |= (0xFFFFFFFF << bit_count);
    }

    return value;
}

// Function to parse and store the memory image file
void parse_and_store(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[50]; // Buffer to hold each line
    uint32_t address, value;
    uint8_t temp_bytes[4] = {0}; // Temporary buffer to store bytes
    uint32_t temp_address = 0;   // Address where the first byte of an instruction is stored
    int bytes_stored = 0;        // Tracks how many bytes we have stored in temp_bytes

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%x: %x", &address, &value) == 2) {
            int num_bytes = (value <= 0xFF) ? 1 : (value <= 0xFFFF) ? 2 : 4; // Determine byte count

            // If bytes_stored is not 0 and we are continuing at the next contiguous address, store contiguously
            if (bytes_stored > 0 && address != temp_address + bytes_stored) {
                // Store the previous set of bytes in memory before processing the new address
                for (int i = 0; i < bytes_stored; i++) {
                    memory[temp_address + i] = temp_bytes[i];
                }
                bytes_stored = 0; // Reset buffer
            }

            // Set the base address for this chunk
            if (bytes_stored == 0) {
                temp_address = address;
            }

            // Store new bytes in little-endian format
            for (int i = 0; i < num_bytes; i++) {
                temp_bytes[bytes_stored] = (value >> (8 * i)) & 0xFF;
                bytes_stored++;
            }

            // If we have accumulated 4 bytes, store them in memory
            if (bytes_stored == 4) {
                for (int i = 0; i < 4; i++) { 
                    memory[temp_address + i] = temp_bytes[i];
                }
                bytes_stored = 0; // Reset buffer
            }
        }
    }

    // If there are leftover bytes (not forming a full 4-byte word), store them in memory
    if (bytes_stored > 0) {
        for (int i = 0; i < bytes_stored; i++) {
            memory[temp_address + i] = temp_bytes[i];
        }
    }

    fclose(file);
    printf("Memory image successfully loaded.\n");
}

// **Function to enforce registers[0] = 0 after every operation**
void enforce_register_zero() {
    registers[0] = 0; // Ensure x0 is always zero
}

// Read 1, 2, or 4 bytes from memory (little-endian format)
uint32_t read_memory(uint32_t address, int size = 4) {
    uint32_t value = 0;

    if (size == 1 || size == 2 || size == 4) {
        for (int i = 0; i < size; i++) {
            value |= (memory[address + i] << (8 * i));
        }
        return value;
    } else {
        printf("Error: Invalid memory read size %d. Only 1, 2, or 4 bytes allowed.\n", size);
        return 0;
    }
}


// Write 1, 2, or 4 bytes to memory (little-endian format)
void write_memory(uint32_t address, uint32_t value, int size = 4) {
    if (size != 1 && size != 2 && size != 4) {
        printf("Error: Invalid memory write size %d. Only 1, 2, or 4 bytes allowed.\n", size);
        return;
    }

    for (int i = 0; i < size; i++) {
        memory[address + i] = (value >> (8 * i)) & 0xFF;
    }
    enforce_register_zero();
}

// Fetch instruction from memory
uint32_t fetch() {
    return read_memory(pc);
}

// Check if a memory address is used
bool is_memory_used(uint32_t address) {
    return memory.find(address) != memory.end();
}

// Function to print the stack memory
void print_registers() {
    // RISC-V register names
    const char* reg_names[32] = {
        "zero", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
        "s0/fp", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
        "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
        "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
    };

    printf("\n=== Register State ===\n");
    for (int i = 0; i < 32; i++) {
        // Ensure register x0 (zero) is always 0
        if (i == 0) {
            registers[i] = 0;
        }
        printf("%-6s : 0x%08X\n", reg_names[i], registers[i]);

    }
    printf("======================\n");
}


// Function to handle I-type instructions with signed immediate
void handle_i_type_instruction(uint32_t instruction) {
    int imm = (instruction >> 20) & 0xFFF; 
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x7;
    uint32_t opcode = instruction & 0x7F;
    uint32_t shamt = (instruction >> 20) & 0x1F;
        
        switch (opcode) {
        case 0x03: // Load Instructions
            switch (funct3) {
                case 0x0: registers[rd] = sign_extend(read_memory(registers[rs1] + sign_extend(imm,12), 1),8);pc += 4; break; // LB: Load Byte (signed)
                case 0x1: registers[rd] = sign_extend(read_memory(registers[rs1] + sign_extend(imm,12), 2),16); pc += 4;break; // LH: Load Halfword (signed)
                case 0x2: registers[rd] = read_memory(registers[rs1] + sign_extend(imm,12), 4); pc += 4;break; // LW: Load Word
                case 0x4: registers[rd] = read_memory(registers[rs1] + sign_extend(imm,12), 1);pc += 4; break; // LBU: Load Byte (unsigned)
                case 0x5: registers[rd] = read_memory(registers[rs1] + sign_extend(imm,12), 2); pc += 4;break; // LHU: Load Halfword (unsigned)
                default: printf("Unsupported Load instruction: 0x%X\n", funct3); break;
            }
            break;

        case 0x13: // Arithmetic and Shift Instructions
            switch (funct3) {
                case 0x0: registers[rd] = registers[rs1] + sign_extend(imm,12);pc += 4;break; // ADDI: Add Immediate
                case 0x2: registers[rd] = ((int32_t)registers[rs1] < (int32_t)sign_extend(imm, 12)) ? 1 : 0;pc += 4;break; // SLTI: Set Less Than Immediate (signed)
                case 0x3: registers[rd] = (registers[rs1] < (uint32_t)sign_extend(imm,12)) ? 1 : 0; pc += 4;break; // SLTIU: Set Less Than Immediate (unsigned)
                case 0x4: registers[rd] = registers[rs1] ^ sign_extend(imm,12);pc += 4;break; // XORI: XOR Immediate
                case 0x6: registers[rd] = registers[rs1] | sign_extend(imm,12);pc += 4;break; // ORI: OR Immediate
                case 0x7: registers[rd] = registers[rs1] & sign_extend(imm,12);pc += 4; break; // ANDI: AND Immediate
                case 0x1: registers[rd] = registers[rs1] << shamt;pc += 4; break; // SLLI: Shift Left Logical Immediate
                case 0x5: {
                    bool is_arithmetic = (instruction >> 30) & 1;
                    if (is_arithmetic) {
                        registers[rd] = sign_extend(registers[rs1] >> shamt,(32-shamt));pc += 4;// SRAI
                    } else {
                        registers[rd] = registers[rs1] >> shamt; pc += 4;// SRLI
                    }
                    break;
                }
                default: printf("Unsupported Arithmetic/Shift instruction: 0x%X\n", funct3); break;
            }
            break;

        case 0x67: // JALR: Jump And Link Register
            registers[rd] = pc + 4;
            pc = (registers[rs1] + sign_extend(imm,12)) & ~1;
            break;

        default:
            printf("Unsupported I-Type opcode: 0x%X\n", opcode);
            break;
    }
    enforce_register_zero();
}

 // Function to handle R-type instructions
void handle_r_type_instruction(uint32_t instruction) {
    // Extract fields from the instruction
    uint32_t rd = (instruction >> 7) & 0x1F;      // Bits [11:7] - Destination Register
    uint32_t funct3 = (instruction >> 12) & 0x7;  // Bits [14:12] - Function Code
    uint32_t rs1 = (instruction >> 15) & 0x1F;    // Bits [19:15] - Source Register 1
    uint32_t rs2 = (instruction >> 20) & 0x1F;    // Bits [24:20] - Source Register 2
    uint32_t funct7 = (instruction >> 25) & 0x7F; // Bits [31:25] - Function Code
    uint32_t opcode = instruction & 0x7F;         // Bits [6:0] - Opcode

    // Ensure it's an R-type instruction (opcode should be 0110011 in binary, i.e., 0x33)
    if (opcode != 0x33) {
        return; // Not an R-type instruction, return immediately
    }

    // Decode and execute the R-type instruction
    switch (funct3) {
        case 0x0: // ADD or SUB
            if (funct7 == 0x00) { // ADD
                registers[rd] = registers[rs1] + registers[rs2];pc += 4;
            } else if (funct7 == 0x20) { // SUB
                registers[rd] = registers[rs1] - registers[rs2];pc += 4;
            }
            break;
        case 0x1: // SLL (Shift Left Logical)
            registers[rd] = registers[rs1] << (registers[rs2] & 0x1F);pc += 4;
            break;
        case 0x2: // SLT (Set Less Than)
            registers[rd] = (int32_t)registers[rs1] < (int32_t)registers[rs2];pc += 4;
            break;
        case 0x3: // SLTU (Set Less Than Unsigned)
            registers[rd] = registers[rs1] < registers[rs2];pc += 4;
            break;
        case 0x4: // XOR
            registers[rd] = registers[rs1] ^ registers[rs2];pc += 4;
            break;
        case 0x5: // SRL (Shift Right Logical) or SRA (Shift Right Arithmetic)
            if (funct7 == 0x00) { // SRL
                registers[rd] = registers[rs1] >> (registers[rs2] & 0x1F);pc += 4;
            } else if (funct7 == 0x20) { // SRA
                registers[rd] = (int32_t)registers[rs1] >> (registers[rs2] & 0x1F);pc += 4;
            }
            break;
        case 0x6: // OR
            registers[rd] = registers[rs1] | registers[rs2];pc += 4;
            break;
        case 0x7: // AND
            registers[rd] = registers[rs1] & registers[rs2];pc += 4;
            break;
        default:
            break; // Unknown R-type instruction, do nothing
    }
    enforce_register_zero();
}

void handle_exception(const char* cause, uint32_t address) {
    std::cerr << "Exception: " << cause << ", Address: 0x" << std::hex << address << std::dec << std::endl;
}

void handle_j_type_instruction(uint32_t instruction) {
    uint32_t rd = (instruction >> 7) & 0x1F;       // Bits [11:7] - Destination Register
    int imm = ((instruction >> 12) & 0xFF) << 12 |  // Bits [19:12] - Immediate[19:12]
               ((instruction >> 20) & 1) << 11 |     // Bit  [20] - Immediate[11]
               ((instruction >> 21) & 0x3FF) << 1 |  // Bits [30:21] - Immediate[10:1]
               ((instruction >> 31) ? 0xFFE00000 : 0); // Bit [31] - Sign extension
    uint32_t opcode = instruction & 0x7F;          // Bits [6:0] - Opcode
    
    imm = sign_extend(imm, 20); // Perform sign extension
    uint32_t target_address = pc + 4 + imm;
    
    // Check for word alignment (RISC-V requires 4-byte alignment)
    if (target_address % 4 != 0) {
        handle_exception("Instruction address misaligned", target_address);
        return; // Do not perform the jump
    }
    
    registers[rd] = pc + 4; // Store return address
    pc = target_address; // Jump to target address
    
    enforce_register_zero();
}

// Function to hangle S-type instructions
void handle_s_type_instruction(uint32_t instruction){
    //extracting feilds from the instruction
    uint32_t opcode = instruction & 0x7F;
    int imm_4to0 = (instruction >> 7) & 0x1F;
    int imm_11to5 = (instruction >> 25) & 0x7F;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t rs2 =  (instruction >> 20) & 0x1F;
    uint32_t func3 = (instruction >> 12) & 0x7;
   
    int imm = (imm_11to5 << 5) | imm_4to0;
    imm = sign_extend(imm,12); // Sign extend the immediate value
       // Compute effective memory address
       uint32_t address = registers[rs1] + imm;
       int value = registers[rs2];
   
       // Execute the appropriate S-Type instruction
           switch (func3) {
               case 0x0: // SB (Store Byte)
                   write_memory(address, value & 0xFF, 1);pc += 4;
                   break;
               case 0x1: // SH (Store Halfword)
                   write_memory(address, value & 0xFFFF, 2);pc += 4;
                   break;
               case 0x2: // SW (Store Word)
                   write_memory(address, value, 4);pc += 4;
                   break;
               default:
                   printf("Unsupported S-Type instruction: opcode=0x%X, funct3=0x%X\n", opcode, func3);
                   break;
              }
       enforce_register_zero();
    }

// Function to hangle U-type instructions
void handle_u_type_instruction(uint32_t instruction) {
    uint32_t opcode = instruction & 0x7F;     // Extract opcode (bits 6:0)
    uint32_t rd = (instruction >> 7) & 0x1F;  // Extract rd (bits 11:7)
    int imm = (instruction >> 12) & 0xFFFFF;   // Extract immediate (bits 31:12) and align it
    switch (opcode) {
        case 0x37: // LUI
        // Simulated operation: Load the upper immediate into the register
            registers[rd] = imm<<12;pc += 4;
        break;
    
        case 0x17: // AUIPC
        // Simulated operation: Add immediate to PC and store in rd
            registers[rd] = pc + (imm<<12);pc += 4;
        break;
    
        default:
             printf("Unsupported U-Type instruction: opcode=0x%X", opcode);
        break;
        }
        enforce_register_zero();
    }

//function to handle B-type instructions
void handle_b_type_instruction(uint32_t instruction) {
    int imm = ((instruction & 0x80000000) >> 20 ) |    //bit[31]] =imm[12] 
    ((instruction & 0x80) <<3) |                            //bit[7]  = imm[11]
    ((instruction & 0x7E000000) >> 21) |                   //bit[25:30] =imm[10:5]
    ((instruction & 0xF00) >> 8);                          //bit[8:11] =imm[4:1]
    imm = sign_extend(imm,13); // Sign extend the immediate value
    uint32_t rs1 = (instruction >> 15 ) & 0x1F;
    uint32_t rs2 = (instruction >> 20) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x7;
    uint32_t opcode = instruction & 0x7F;
    switch (funct3) {
        case 0x0: if (registers[rs1] == registers[rs2])    //BEQ (Branch if equal)
                   pc = pc + (imm<<1);
                   break;          
        case 0x1: if (registers[rs1] != registers[rs2])    //BNE  (Branch if not equal)
                   pc = pc + (imm<<1);
                   break;          
        case 0x2:  if (registers[rs1] < registers[rs2])    //BLT (Branch if less than)
                   pc = pc + (imm<<1);
                   break;   
        case 0x3: if(registers[rs1] >= registers[rs2])    //BGE (Branch if greater than or equal)
                   pc = pc + (imm<<1);   
                   break;
        case 0x4: if((unsigned)registers[rs1] < (unsigned)registers[rs2])  //BLTU(Branch if less than unsigned)
                   pc = pc + (imm<<1);
                   break;   
        case 0x5: if((unsigned)registers[rs1] >= (unsigned)registers[rs2])  //BGEU(Branch if greater than or equal unsigned)
                   pc = pc + (imm<<1);
                   break;    
        default: printf("unsupported B-Type instruction: 0x%X\n",funct3);
                break;
    }
    enforce_register_zero();
}
    
// Decode and execute instruction
void decode_and_execute(uint32_t instruction) {
    #ifdef NORMAL
    printf("PC = 0x%08X | Instruction = 0x%08X\n", pc, instruction);
    #endif
    if (instruction == 0x00000000) {
        printf("Encountered invalid instruction (0x00000000), stopping execution.\n");
        return; }

    uint32_t opcode = instruction & 0x7F; // Extract opcode (bits 6:0)

    switch (opcode) {
        case 0x33:handle_r_type_instruction(instruction);break; // R-Type Instructions (ADD, SUB, XOR, AND, OR, etc.) 
        case 0x03: handle_i_type_instruction(instruction);break;// Load I-Type Instructions (LB, LH, LW, LBU, LHU)
        case 0x13: handle_i_type_instruction(instruction);break;// Arithmetic & Shift I-Type Instructions (ADDI, SLTI, XORI, etc.)
        case 0x67: handle_i_type_instruction(instruction);break;// JALR (Jump and Link Register)
        case 0x6F: handle_j_type_instruction(instruction);break;// J-Type Instruction (JAL)
        case 0x23: handle_s_type_instruction(instruction);break;// S-Type Instruction (SB, SH, SW - Store instructions)               
        case 0x37: handle_u_type_instruction(instruction);break;// U-Type Instruction (LUI - Load Upper Immediate)
        case 0x17: handle_u_type_instruction(instruction);break;// U-Type Instruction (AUIPC - Add Upper Immediate to PC)
        case 0x63: handle_b_type_instruction(instruction);break;// B-Type Instruction
        default:
        if (instruction == 0x00000000) {
            printf("Encountered invalid instruction (0x00000000), stopping execution.\n");
            break;}
    }
    enforce_register_zero();

    #ifdef NORMAL
    print_registers();
    #endif
}

void print_register(int reg) {
    if (reg >= 0 && reg < 32) {
        printf("x%d = 0x%08X\n", reg, registers[reg]);
    } else {
        printf("Invalid register number.\n");
    }
}

void print_memory(uint32_t address) {
    if (memory.count(address)) {
        printf("Memory at 0x%08X = 0x%02X\n", address, memory[address]);
    } else {
        printf("No data at address 0x%08X\n", address);
    }
}

void interactive_mode() {
    char command[100];
    unsigned int address;
    int reg;

    while (true) {
        printf("(debugger) > ");
        fgets(command, sizeof(command), stdin);

        if (strcmp(command, "ni\n") == 0) break;

        if (sscanf(command, "print r%d", &reg) == 1 && reg >= 0 && reg < 32) {
            print_register(reg);
        } 
        else if (sscanf(command, "print m[%x]", &address) == 1) {
            print_memory(address);
        } 
        else if (sscanf(command, "bp +%x", &address) == 1) {
            breakpoints.insert(pc + address);  // Calculate upcoming PC breakpoint
            printf("Breakpoint set at 0x%08X (PC + %X)\n", pc + address, address);
        } 
        else if (sscanf(command, "bp %x", &address) == 1) {
            breakpoints.insert(address);  // Standard address breakpoint
            printf("Breakpoint set at 0x%08X\n", address);
        } 
        else if (sscanf(command, "watch m[%x]", &address) == 1) {
            watchpoints_memory[address] = memory[address];
            printf("Memory watchpoint set at 0x%08X\n", address);
        } 
        else if (sscanf(command, "watch r%d", &reg) == 1 && reg >= 0 && reg < 32) {
            watchpoints_register[reg] = registers[reg];
            printf("Register watchpoint set for x%d\n", reg);
        } 
        else if (strcmp(command, "exit\n") == 0) {
            printf("Exiting debugger.\n");
            exit(0);
        } 
        else {
            printf("Unknown command. Use:\n"
                   "  'ni' - Continue execution\n"
                   "  'print rX' - Display register\n"
                   "  'print m[ADDR]' - Display memory\n"
                   "  'bp 0xADDR' - Set breakpoint at absolute address\n"
                   "  'bp +X' - Set breakpoint X instructions ahead\n"
                   "  'watch m[ADDR]' - Set memory watchpoint\n"
                   "  'watch rX' - Set register watchpoint\n"
                   "  'exit' - Exit debugger\n");
        }
    }
}

bool watchpoint_triggered = false;

void check_memory_watchpoints() {
    for (auto& wp : watchpoints_memory) {
        if (memory[wp.first] != wp.second) {
            printf("** Memory watchpoint triggered at 0x%08X (Value changed from 0x%02X to 0x%02X) **\n",
                   wp.first, wp.second, memory[wp.first]);
            wp.second = memory[wp.first]; // Update tracked value
            watchpoint_triggered = true;  // Flag the trigger
        }
    }
}

void check_register_watchpoints() {
    for (auto& wp : watchpoints_register) {
        if (registers[wp.first] != wp.second) {
            printf("** Register watchpoint triggered for x%d (Value changed from 0x%08X to 0x%08X) **\n",
                   wp.first, wp.second, registers[wp.first]);
            wp.second = registers[wp.first]; // Update tracked value
            watchpoint_triggered = true;     // Flag the trigger
        }
    }
}


void run_simulation() {
    printf("=== Simulation Started ===\n");

    while (1) {
        uint32_t instruction = fetch();
        if (instruction == 0x00000000) {
            printf("Encountered invalid instruction (0x00000000), stopping execution.\n");
            break;
        }

        #ifdef VERBOSE
        printf("PC = 0x%08X | Instruction = 0x%08X\n", pc, instruction);
        #endif
     
        #ifdef INTERACTIVE
        
        // Breakpoint Check
        if (breakpoints.count(pc)) {
            printf("** Breakpoint hit at 0x%08X **\n", pc);
            interactive_mode(); // Pause for interactive control
        }

        // Watchpoint Checks
        check_memory_watchpoints();
        check_register_watchpoints();
        if (watchpoint_triggered) {
            interactive_mode();
        }

        interactive_mode();
        #endif

        decode_and_execute(instruction);
    }
        #ifdef SILENT
        printf("Final PC = 0x%08X\n", pc);
        print_registers();
        #endif    
        printf("=== Simulation Ended ===\n");
    }


int main() {
    registers[2]= 65536;
    pc= 0;
    parse_and_store("addi.mem");
    printf("\nStarting simulation...\n");
    run_simulation();
    return 0;
}
