#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unordered_map>

using namespace std;

unordered_map<uint32_t, uint8_t> memory; // Memory using associative array
uint32_t registers[32]; // Simulated registers
uint32_t pc = 0; // Program Counter initialized to 0

// Function to parse and store the memory image file
void parse_and_store(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[50]; // Buffer to hold each line
    uint32_t address, value;
    uint8_t temp_bytes[4] = {0}; // Temporary storage for assembling 4-byte words
    uint32_t temp_address = 0;   // Stores the address of the first byte in an instruction
    int bytes_stored = 0;        // Tracks number of bytes stored for the current word

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%x: %x", &address, &value) == 2) {
            int num_bytes = (value <= 0xFF) ? 1 : (value <= 0xFFFF) ? 2 : 4; // Determine byte count

            // If new address is discontinuous, reset tracking
            if (bytes_stored > 0 && address != temp_address + bytes_stored) {
                printf("Warning: Non-contiguous memory detected at 0x%X\n", address);
                bytes_stored = 0;
            }

            // Store bytes in little-endian format into temp buffer
            for (int i = 0; i < num_bytes; i++) {
                temp_bytes[bytes_stored] = (value >> (8 * i)) & 0xFF;
                bytes_stored++;
            }

            // Set the base address when we start a new instruction
            if (bytes_stored == num_bytes) {
                temp_address = address;
            }

            // When we reach 4 bytes, store them in memory
            if (bytes_stored == 4) {
                for (int i = 0; i < 4; i++) {
                    memory[temp_address + i] = temp_bytes[i];
                }
                bytes_stored = 0; // Reset for next instruction
            }
        }
    }

    fclose(file);
    printf("Memory image successfully loaded.\n");
}

// Function to print stored memory content for verification
void print_memory(uint32_t start_address, uint32_t num_instructions) {
    printf("\n--- Loaded Memory Content (Little-Endian Order) ---\n");
    for (uint32_t addr = start_address; addr < start_address + (num_instructions * 4); addr += 4) {
        uint32_t instruction = (memory[addr]) |
                               (memory[addr + 1] << 8) |
                               (memory[addr + 2] << 16) |
                               (memory[addr + 3] << 24);
        printf("0x%08X: 0x%08X\n", addr, instruction);
    }
    printf("-------------------------------------------------\n");
}

// Read 1, 2, or 4 bytes from memory (little-endian format)
uint32_t read_memory(uint32_t address, int size) {
    uint32_t value = 0;

    // Ensure size is either 1, 2, or 4 bytes
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
    // Ensure size is either 1, 2, or 4 bytes
    if (size != 1 && size != 2 && size != 4) {
        printf("Error: Invalid memory write size %d. Only 1, 2, or 4 bytes allowed.\n", size);
        return;
    }

    // Store each byte in little-endian order
    for (int i = 0; i < size; i++) {
        memory[address + i] = (value >> (8 * i)) & 0xFF;
    }

    printf("Wrote %d-byte value 0x%X to address 0x%X\n", size, value, address);
}

// Fetch instruction from memory
uint32_t fetch() {
    return read_memory(pc, 4); // Fetch 4-byte instruction at PC
}

// Decode and execute instruction (simple placeholder)
void decode_and_execute(uint32_t instruction) {
    printf("Executing instruction 0x%08X at PC 0x%X\n", instruction, pc);

    if (instruction == 0x00000000) {
        printf("Encountered invalid instruction (0x00000000), stopping execution.\n");
        exit(0);
    }

    // Placeholder for actual instruction handling
}

// Main loop: Fetch-Decode-Execute cycle
void run_simulation() {
    while (1) {
        uint32_t instruction = fetch();
        decode_and_execute(instruction);
        pc += 4; // Increment PC by 4 after executing an instruction
    }
}

int main() {
    parse_and_store("test.mem"); // Load memory image from file
    print_memory(0, 10); // Print first 10 instructions for verification

    // Demonstrate read and write memory functions
    write_memory(0x1000, 0xAB, 1);
    read_memory(0x1000, 1);
    
    write_memory(0x2000, 0x1234, 2);
    read_memory(0x2000, 2);
    
    write_memory(0x3000, 0xDEADBEEF, 4);
    read_memory(0x3000, 4);

    printf("\nStarting simulation...\n");
    run_simulation(); // Start executing instructions from memory

    return 0;
}
