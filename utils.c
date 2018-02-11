#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

/* Sign extends the given field to a 32-bit integer where field is
 * interpreted an n-bit integer. */ 
int sign_extend_number( unsigned int field, unsigned int n) {
    unsigned int most_significant_bit = (field >> (n - 1)) & 1;
    int result = 0;
    result = result + field;
    int mask;
    if (most_significant_bit) { /* Most significant bit is 1. */
        mask = -1 - ((1 << n) - 1); /* All 1's in extra left spaces. */
        result = result | mask; /* Apply the mask (fill in empty bits with most significant bit) */
    } else { /* Most significant bit is 0. */
        mask = 0 + ((1 << n) - 1); /* All 0's in extra left spaces, 1's in used spaces. */
        result = result & mask; /* Apply the mask (fill in empty bits with most significant bit) */
    }
    
    return result;
}

/* Unpacks the 32-bit machine code instruction given into the correct
 * type within the instruction struct */ 
Instruction parse_instruction(uint32_t instruction_bits) {
    Instruction instruction;
    
    unsigned opcode = instruction_bits & ((1 << 7) - 1); /* Extract last 7 bits */

    switch(opcode) {
        case 0x33:
            /* R-Type */
            instruction.rtype.opcode = get_bit_range(instruction_bits, 0, 6);
            instruction.rtype.rd = get_bit_range(instruction_bits, 7, 11);
            instruction.rtype.funct3 = get_bit_range(instruction_bits, 12, 14);
            instruction.rtype.rs1 = get_bit_range(instruction_bits, 15, 19);
            instruction.rtype.rs2 = get_bit_range(instruction_bits, 20, 24);
            instruction.rtype.funct7 = get_bit_range(instruction_bits, 25, 31);

            break;
        case 0x13: case 0x3: case 0x73:
            /* I-Type */
            instruction.itype.opcode = get_bit_range(instruction_bits, 0, 6);
            instruction.itype.rd = get_bit_range(instruction_bits, 7, 11);
            instruction.itype.funct3 = get_bit_range(instruction_bits, 12, 14);
            instruction.itype.rs1 = get_bit_range(instruction_bits, 15, 19);
            instruction.itype.imm = get_bit_range(instruction_bits, 20, 31);
            break;
        case 0x23:
            /* S-Type */
            instruction.stype.opcode = get_bit_range(instruction_bits, 0, 6);
            instruction.stype.imm5 = get_bit_range(instruction_bits, 7, 11);
            instruction.stype.funct3 = get_bit_range(instruction_bits, 12, 14);
            instruction.stype.rs1 = get_bit_range(instruction_bits, 15, 19);
            instruction.stype.rs2 = get_bit_range(instruction_bits, 20, 24);
            instruction.stype.imm7 = get_bit_range(instruction_bits, 25, 31);
            break;
        case 0x63:
            /* SB-Type */
            instruction.sbtype.opcode = get_bit_range(instruction_bits, 0, 6);
            /* FIX */
            instruction.sbtype.imm5 = get_bit_range(instruction_bits, 7, 11);
            instruction.sbtype.funct3 = get_bit_range(instruction_bits, 12, 14);
            instruction.sbtype.rs1 = get_bit_range(instruction_bits, 15, 19);
            instruction.sbtype.rs2 = get_bit_range(instruction_bits, 20, 24);
            /* FIX */
            instruction.sbtype.imm7 = get_bit_range(instruction_bits, 25, 31);
            break;
        case 0x37:
            /* U-Type (LUI) */
            instruction.utype.opcode = get_bit_range(instruction_bits, 0, 6);
            instruction.utype.rd = get_bit_range(instruction_bits, 7, 11);
            instruction.utype.imm = get_bit_range(instruction_bits, 12, 31);
            break;
        case 0x6F:
            /* UJ-Type */
            instruction.ujtype.opcode = get_bit_range(instruction_bits, 0, 6);
            instruction.ujtype.rd = get_bit_range(instruction_bits, 7, 11);
            /* FIX */
            instruction.ujtype.imm = get_bit_range(instruction_bits, 12, 31);
            break;
        default: // undefined opcode
            fprintf(stderr, "%s %d", "Undefined opcode in instruction: ", instruction_bits);
            break;
    }
    
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


unsigned get_bit_range(unsigned input, unsigned lower, unsigned upper) {
    return (input >> lower) & ~(~0 << (upper - lower + 1));
    /* Right shift off the unused lower (right) bits, then
        mask off the unused upper bits by anding with all 
        1s in the wanted bits and 0s in the unwanted left
        bits. */ 
}

