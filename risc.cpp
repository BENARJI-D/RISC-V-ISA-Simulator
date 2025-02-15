#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unordered_map>

using namespace std;

unordered_map<uint32_t, uint8_t> memory; // Memory using associative array
uint32_t registers[32]; // Simulate registers

void parse_and_store(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[50]; // Buffer to hold each line
    uint32_t address, value;

    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%x: %x", &address, &value) == 2) {
            // Store in little-endian format
            memory[address]     = (value & 0xFF);
            memory[address + 1] = (value >> 8) & 0xFF;
            memory[address + 2] = (value >> 16) & 0xFF;
            memory[address + 3] = (value >> 24) & 0xFF;
        }
    }

    fclose(file);
    printf("Memory image successfully loaded.\n");
}

// Read 1, 2, or 4 bytes from memory
uint32_t read_memory(uint32_t address, int size) {
    uint32_t value = 0;
    for (int i = 0; i < size; i++) {
        value |= memory[address + i] << (8 * i);
    }
    printf("Read %d-byte value 0x%X from address 0x%X\n", size, value, address);
    return value;
}

// Write 1, 2, or 4 bytes to memory
void write_memory(uint32_t address, uint32_t value, int size) {
    for (int i = 0; i < size; i++) {
        memory[address + i] = (value >> (8 * i)) & 0xFF;
    }
    printf("Wrote %d-byte value 0x%X to address 0x%X\n", size, value, address);
}

int main() {
    parse_and_store("ven.mem");

    // Demonstration of reading and writing
    write_memory(0x1000, 0xDEADBEEF, 4);
    read_memory(0x1000, 4);
    
    write_memory(0x2000, 0xABCD, 2);
    read_memory(0x2000, 2);

    write_memory(0x3000, 0x12, 1);
    read_memory(0x3000, 1);

    return 0;
}
