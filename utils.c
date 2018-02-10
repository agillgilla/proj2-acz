#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* Sign extends the given field to a 32-bit integer where field is
 * interpreted an n-bit integer. */ 
int sign_extend_number( unsigned int field, unsigned int n) {
    unsigned int most_significant_bit = (x >> (n - 1)) & 1;
    int result = 0;
    result = result + field;
    int mask;
    if (most_significant_bit) { /* Most significant bit is 1. */
        mask = -1; /* All 1's */
    } else { /* Most significant bit is 0. */
        mask = 0 /* All 0's */
    }
    result = result | mask; /* Apply the mask (fill in empty bits with most significant bit) */
    return result;
}

/* Unpacks the 32-bit machine code instruction given into the correct
 * type within the instruction struct */ 
Instruction parse_instruction(uint32_t instruction_bits) {
    /* YOUR CODE HERE */
    Instruction instruction;
    return instruction;
}

/* Return the number of bytes (from the current PC) to the branch label using the given
 * branch instruction */
int get_branch_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    return 0; 
}

/* Returns the number of bytes (from the current PC) to the jump label using the given
 * jump instruction */
int get_jump_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    return 0;
}

int get_store_offset(Instruction instruction) {
    /* YOUR CODE HERE */
    return 0;
}

void handle_invalid_instruction(Instruction instruction) {
    printf("Invalid Instruction: 0x%08x\n", instruction.bits); 
}

void handle_invalid_read(Address address) {
    printf("Bad Read. Address: 0x%08x\n", address);
    exit(-1);
}

void handle_invalid_write(Address address) {
    printf("Bad Write. Address: 0x%08x\n", address);
    exit(-1);
}

