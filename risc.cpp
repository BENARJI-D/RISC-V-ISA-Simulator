#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <map> // Change from unordered_map to map

using namespace std;

map<uint32_t, uint8_t> memory; // Ordered map for memory
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
    uint32_t value = 0;

    if (size != 1 && size != 2 && size != 4) {
        printf("Error: Invalid memory read size %d. Only 1, 2, or 4 bytes allowed.\n", size);
        return 0;
    }

    for (int i = 0; i < size; i++) {
        value |= (memory[address + i] << (8 * i));
    }

    printf("Read %d-byte value 0x%X from address 0x%X\n", size, value, address);
    return value;
}

// Write 1, 2, or 4 bytes to memory (little-endian format)
void write_memory(uint32_t address, uint32_t value, int size) {
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

// Function to handle I-type instructions
void handle_i_type_instruction(uint32_t instruction) {
    int32_t imm = (instruction >> 20) & 0xFFF;
    uint32_t rd = (instruction >> 7) & 0x1F;
    uint32_t rs1 = (instruction >> 15) & 0x1F;
    uint32_t funct3 = (instruction >> 12) & 0x7;
    uint32_t opcode = instruction & 0x7F;

    switch (opcode) {
        case 0x03: // Load Instructions
            switch (funct3) {
                case 0x0: registers[rd] = (int8_t)read_memory(registers[rs1] + imm, 1); break;
                case 0x1: registers[rd] = (int16_t)read_memory(registers[rs1] + imm, 2); break;
                case 0x2: registers[rd] = read_memory(registers[rs1] + imm, 4); break;
                case 0x4: registers[rd] = (uint8_t)read_memory(registers[rs1] + imm, 1); break;
                case 0x5: registers[rd] = (uint16_t)read_memory(registers[rs1] + imm, 2); break;
                default: printf("Unsupported Load instruction: 0x%X\n", funct3); break;
            }
            break;

        case 0x13: // Arithmetic and Shift Instructions
            switch (funct3) {
                case 0x0: registers[rd] = registers[rs1] + imm; break;
                case 0x2: registers[rd] = (registers[rs1] < imm) ? 1 : 0; break;
                case 0x3: registers[rd] = ((uint32_t)registers[rs1] < (uint32_t)imm) ? 1 : 0; break;
                case 0x4: registers[rd] = registers[rs1] ^ imm; break;
                case 0x6: registers[rd] = registers[rs1] | imm; break;
                case 0x7: registers[rd] = registers[rs1] & imm; break;
                case 0x1: registers[rd] = registers[rs1] << imm; break;
                case 0x5: 
                    if ((instruction >> 30) & 1) registers[rd] = (int32_t)registers[rs1] >> imm; 
                    else registers[rd] = (uint32_t)registers[rs1] >> imm;
                    break;
                default: printf("Unsupported Arithmetic/Shift instruction: 0x%X\n", funct3); break;
            }
            break;

        case 0x67: // JALR
            registers[rd] = pc + 4;
            pc = (registers[rs1] + imm) & ~1;
            break;

        default:
            printf("Unsupported I-Type opcode: 0x%X\n", opcode);
            break;
    }
}

// Decode and execute instruction
void decode_and_execute(uint32_t instruction) {
    printf("Executing instruction 0x%08X at PC 0x%X\n", instruction, pc);
    if (instruction == 0x00000000) exit(0);
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
