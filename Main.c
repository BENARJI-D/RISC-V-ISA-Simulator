#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MEMORY_SIZE  64 * 1024  // Define a sufficient memory size
#define NUMBER_OF_REGISTERS 32  // Define the number of registers

uint8_t memory[MEMORY_SIZE]; // Simulated memory storage
uint32_t registers [NUMBER_OF_REGISTERS]; // simulated registers 

void parse_and_store(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[50]; // Buffer to hold each line
    uint32_t address, value;

    while (fgets(line, sizeof(line), file)) {
        // Parse address and 32-bit hex value
        if (sscanf(line, "%x: %x", &address, &value) == 2) {
            // Store in little-endian format
            memory[address]     = (value & 0xFF);        // Least significant byte
            memory[address + 1] = (value >> 8) & 0xFF;
            memory[address + 2] = (value >> 16) & 0xFF;
            memory[address + 3] = (value >> 24) & 0xFF;  // Most significant byte
        }
    }

    fclose(file);
}




int main() {
    parse_and_store("input.txt"); // Replace with actual filename

    // Print memory contents for verification

    return 0;
}
