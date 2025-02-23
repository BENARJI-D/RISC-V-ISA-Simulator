#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <map> 

using namespace std;

map<uint32_t, uint8_t> memory; // Ordered mapi for memory
uint32_t registers[32]; // Simulated registers
uint32_t pc = 0; // Program Counter initialized to 0

// Function to perform sign extension dynamically
uint32_t sign_extend(uint32_t value) {
    int bit_count = 0;
    uint32_t temp = value;

    while (temp) { // Count how many bits are in use
        temp >>= 1;
        bit_count++;
    }

    if (bit_count >= 32) {
        return value;
    }

    uint32_t sign_bit = 1 << (bit_count - 1);
    if (value & sign_bit) {
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


// Read 1, 2, or 4 bytes from memory (little-endian format)
uint32_t read_memory(uint32_t address, int size = 4) {
    if ((int32_t)address < 0) {
        printf("Error: Negative address 0x%X is not valid.\n", address);
        exit(1);
    }
    
    uint32_t value = 0;

    if (size == 1 || size == 2 || size == 4) {
        for (int i = 0; i < size; i++) {
            value |= (memory[address + i] << (8 * i));
        }
        printf("Read %d-byte value 0x%X from address 0x%X\n", size, value, address);
        return value;
    } else {
        printf("Error: Invalid memory read size %d. Only 1, 2, or 4 bytes allowed.\n", size);
        return 0;
    }
}


// Write 1, 2, or 4 bytes to memory (little-endian format)
void write_memory(uint32_t address, uint32_t value, int size = 4) {
    if ((int32_t)address < 0) {
        printf("Error: Negative address 0x%X is not valid.\n", address);
        exit(1);
    }

    if (size != 1 && size != 2 && size != 4) {
        printf("Error: Invalid memory write size %d. Only 1, 2, or 4 bytes allowed.\n", size);
        return;
    }

    for (int i = 0; i < size; i++) {
        memory[address + i] = (value >> (8 * i)) & 0xFF;
    }

    printf("Wrote %d-byte value 0x%X to address 0x%X\n", size, value, address);
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
void print_stack(uint32_t start_address, uint32_t end_address) {
    printf("Stack Memory (0x%X to 0x%X):\n", start_address, end_address);
    for (uint32_t address = start_address; address <= end_address; address += 4) {
        if (is_memory_used(address)) {
            printf("0x%X: 0x%X\n", address, read_memory(address, 4));
        } else {
            printf("0x%X: [unused]\n", address);
        }
    }
}

// Function to handle I-type instructions with signed immediate
void handle_i_type_instruction(uint32_t instruction) {
    int32_t imm = instruction >> 20; // Use sign_extend only for loads
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x7;
    uint32_t opcode = instruction & 0x7F;

    switch (opcode) {
        case 0x03: // Load Instructions
            imm = sign_extend(imm); // Sign-extend only for loads
            switch (funct3) {
                case 0x0: registers[rd] = read_memory(registers[rs1] + imm, 1); break; // LB: Load Byte (signed)
                case 0x1: registers[rd] = read_memory(registers[rs1] + imm, 2); break; // LH: Load Halfword (signed)
                case 0x2: registers[rd] = read_memory(registers[rs1] + imm, 4); break; // LW: Load Word
                case 0x4: registers[rd] = read_memory(registers[rs1] + imm, 1); break; // LBU: Load Byte (unsigned)
                case 0x5: registers[rd] = read_memory(registers[rs1] + imm, 2); break; // LHU: Load Halfword (unsigned)
                default: printf("Unsupported Load instruction: 0x%X\n", funct3); break;
            }
            break;

        case 0x13: // Arithmetic and Shift Instructions
            switch (funct3) {
                case 0x0: registers[rd] = registers[rs1] + imm; break; // ADDI: Add Immediate
                case 0x2: registers[rd] = (registers[rs1] < (uint32_t)imm) ? 1 : 0; break; // SLTI: Set Less Than Immediate (signed)
                case 0x3: registers[rd] = ((uint32_t)registers[rs1] < (uint32_t)imm) ? 1 : 0; break; // SLTIU: Set Less Than Immediate (unsigned)
                case 0x4: registers[rd] = registers[rs1] ^ imm; break; // XORI: XOR Immediate
                case 0x6: registers[rd] = registers[rs1] | imm; break; // ORI: OR Immediate
                case 0x7: registers[rd] = registers[rs1] & imm; break; // ANDI: AND Immediate
                case 0x1: registers[rd] = registers[rs1] << imm; break; // SLLI: Shift Left Logical Immediate
                case 0x5: {
                    bool is_arithmetic = (instruction >> 30) & 1;
                    if (is_arithmetic) {
                        registers[rd] = sign_extend(sign_extend(registers[rs1]) >> imm); // SRAI
                    } else {
                        registers[rd] = registers[rs1] >> imm; // SRLI
                    }
                    break;
                }
                default: printf("Unsupported Arithmetic/Shift instruction: 0x%X\n", funct3); break;
            }
            break;

        case 0x67: // JALR: Jump And Link Register
            registers[rd] = pc + 4;
            pc = (registers[rs1] + imm) & ~1;
            break;

        default:
            printf("Unsupported I-Type opcode: 0x%X\n", opcode);
            break;
    }
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
                registers[rd] = registers[rs1] + registers[rs2];
            } else if (funct7 == 0x20) { // SUB
                registers[rd] = registers[rs1] - registers[rs2];
            }
            break;
        case 0x1: // SLL (Shift Left Logical)
            registers[rd] = registers[rs1] << (registers[rs2] & 0x1F);
            break;
        case 0x2: // SLT (Set Less Than)
            registers[rd] = (int32_t)registers[rs1] < (int32_t)registers[rs2];
            break;
        case 0x3: // SLTU (Set Less Than Unsigned)
            registers[rd] = registers[rs1] < registers[rs2];
            break;
        case 0x4: // XOR
            registers[rd] = registers[rs1] ^ registers[rs2];
            break;
        case 0x5: // SRL (Shift Right Logical) or SRA (Shift Right Arithmetic)
            if (funct7 == 0x00) { // SRL
                registers[rd] = registers[rs1] >> (registers[rs2] & 0x1F);
            } else if (funct7 == 0x20) { // SRA
                registers[rd] = (int32_t)registers[rs1] >> (registers[rs2] & 0x1F);
            }
            break;
        case 0x6: // OR
            registers[rd] = registers[rs1] | registers[rs2];
            break;
        case 0x7: // AND
            registers[rd] = registers[rs1] & registers[rs2];
            break;
        default:
            break; // Unknown R-type instruction, do nothing
    }
}


// Decode and execute instruction

void decode_and_execute(uint32_t instruction) {
    printf("Executing instruction 0x%08X at PC 0x%X\n", instruction, pc);
    if (instruction == 0x00000000) exit(0);
    uint32_t opcode = instruction & 0x7F;
    if (opcode == 0x33) {
        handle_r_type_instruction(instruction);
    } else if (opcode == 0x03 || opcode == 0x13 || opcode == 0x67) {
        handle_i_type_instruction(instruction);
    } else {
        printf("Unsupported instruction opcode: 0x%X\n", opcode);
    }
}

// Main loop: Fetch-Decode-Execute cycle
void run_simulation() {
    while (1) {
        uint32_t instruction = fetch();
        decode_and_execute(instruction);
        pc += 4;
    }
}

int main() {
    parse_and_store("test.mem");
    printf("\nStarting simulation...\n");
    run_simulation();
    return 0;
}
