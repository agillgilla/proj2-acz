#include <stdio.h> // for stderr
#include <stdlib.h> // for exit()
#include "types.h"
#include "utils.h"
#include "riscv.h"

void execute_rtype(Instruction, Processor *);
void execute_itype_except_load(Instruction, Processor *);
void execute_branch(Instruction, Processor *);
void execute_jal(Instruction, Processor *);
void execute_load(Instruction, Processor *, Byte *);
void execute_store(Instruction, Processor *, Byte *);
void execute_ecall(Processor *, Byte *);
void execute_lui(Instruction, Processor *);

unsigned get_bit_range(unsigned, unsigned, unsigned);

void execute_instruction(uint32_t instruction_bits, Processor *processor,Byte *memory) {    
    Instruction instruction = parse_instruction(instruction_bits);
    switch(instruction.opcode) {
        case 0x33:
            execute_rtype(instruction, processor);
            break;
        case 0x13:
            execute_itype_except_load(instruction, processor);
            break;
        case 0x73:
            execute_ecall(processor, memory);
            break;
        case 0x63:
            execute_branch(instruction, processor);
            break;
        case 0x6F:
            execute_jal(instruction, processor);
            break;
        case 0x23:
            execute_store(instruction, processor, memory);
            break;
        case 0x03:
            execute_load(instruction, processor, memory);
            break;
        case 0x37:
            execute_lui(instruction, processor);
            break;
        default: // undefined opcode
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
}

void execute_rtype(Instruction instruction, Processor *processor) {
    switch (instruction.rtype.funct3){
        case 0x0:
            switch (instruction.rtype.funct7) {
                case 0x0:
                    // Add
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] + processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                case 0x1:
                    // Mul 
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] * processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                case 0x20:
                    // Sub
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] - processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                default:
                    handle_invalid_instruction(instruction);
                    exit(-1);
                    break;
            }
            break;
        case 0x1:
            switch (instruction.rtype.funct7) {
                case 0x0:
                    // SLL
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] << processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                case 0x1:
                    // MULH
                    /*IMPLEMENT THIS LATER! */
                /* TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT */
                /* TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT */
                /* TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT */
                /* TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT */
                /* TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT */
                /* TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT */
                /* TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT TODO: IMPLEMENT */
                    processor->PC += 4;
                    break;
            }
            break;
        case 0x2:
            // SLT
            processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] < processor->R[instruction.rtype.rs2];
            processor->PC += 4;
            break;
        case 0x4:
            switch (instruction.rtype.funct7) {
                case 0x0:
                    // XOR
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] ^ processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                case 0x1:
                    // DIV
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] / processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                default:
                    handle_invalid_instruction(instruction);
                    exit(-1);
                    break;
            }
            break;
        case 0x5:
            switch (instruction.rtype.funct7) {
                case 0x0:
                    // SRL
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] >> processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                case 0x20:
                    // SRA
                    processor->R[instruction.rtype.rd] = (int32_t) processor->R[instruction.rtype.rs1] >> processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                default:
                    handle_invalid_instruction(instruction);
                    exit(-1);
                break;
            }
            break;
        case 0x6:
            switch (instruction.rtype.funct7) {
                case 0x0:
                    // OR
                    processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] | processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                case 0x1:
                    // REM
                    processor->R[instruction.rtype.rd] = (int32_t) processor->R[instruction.rtype.rs1] % (int32_t) processor->R[instruction.rtype.rs2];
                    processor->PC += 4;
                    break;
                default:
                    handle_invalid_instruction(instruction);
                    exit(-1);
                    break;
            }
            break;
        case 0x7:
            // AND
            processor->R[instruction.rtype.rd] = processor->R[instruction.rtype.rs1] & processor->R[instruction.rtype.rs2];
            processor->PC += 4;
            break;
        default:
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
}

void execute_itype_except_load(Instruction instruction, Processor *processor) {
    int imm = sign_extend_number(processor->R[instruction.itype.imm], 12);
    switch (instruction.itype.funct3) {
        case 0x0:
            // ADDI
            processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] + imm;
            processor->PC += 4;
            break;
        case 0x1:
            // SLLI
            processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] << imm;
            processor->PC += 4;
            break;
        case 0x2:
            // SLTI
            processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] < imm;
            processor->PC += 4;
            break;
        case 0x4:
            // XORI
            processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] ^ imm;
            processor->PC += 4;
            break;
        case 0x5: ; //This is an empty statement
            // Shift Right (You must handle both logical and arithmetic)
            unsigned shamt = get_bit_range(instruction.itype.imm, 0, 4);
            unsigned funct = get_bit_range(instruction.itype.imm, 10, 10);
            if (funct) { //SRAI
                processor->R[instruction.itype.rd] = (unsigned) (sign_extend_number(processor->R[instruction.itype.rs1], 5) >> shamt);
            } else { //SRLI
                processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] >> shamt;
            }
            processor->PC += 4;
            break;
        case 0x6:
            // ORI
            processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] | imm;
            processor->PC += 4;
            break;
        case 0x7:
            // ANDI
            processor->R[instruction.itype.rd] = processor->R[instruction.itype.rs1] & imm;
            processor->PC += 4;
            break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }
}

void execute_ecall(Processor *p, Byte *memory) {
    Register i;
    
    // syscall number is given by a0 (x10)
    // argument is given by a1
    switch(p->R[10]) {
        case 1: // print an integer
            printf("%d",p->R[11]);
            break;
        case 4: // print a string
            for(i=p->R[11];i<MEMORY_SPACE && load(memory,i,LENGTH_BYTE);i++) {
                printf("%c",load(memory,i,LENGTH_BYTE));
            }
            break;
        case 10: // exit
            printf("exiting the simulator\n");
            exit(0);
            break;
        case 11: // print a character
            printf("%c",p->R[11]);
            break;
        default: // undefined ecall
            printf("Illegal ecall number %d\n", p->R[10]);
            exit(-1);
            break;
    }
    p->PC += 4;
}

void execute_branch(Instruction instruction, Processor *processor) {
    switch (instruction.sbtype.funct3) {
        case 0x0:
            // BEQ
        if (processor->R[instruction.sbtype.rs1] == processor->R[instruction.sbtype.rs2]) {
            processor->PC += get_branch_offset(instruction);
        } else {
            processor->PC += 4;
        }
            break;
        case 0x1:
            // BNE
        if (processor->R[instruction.sbtype.rs1] != processor->R[instruction.sbtype.rs2]) {
            processor->PC += get_branch_offset(instruction);
        } else {
            processor->PC += 4;
        }
            break;
        default:
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
}

void execute_load(Instruction instruction, Processor *processor, Byte *memory) {
    switch (instruction.itype.funct3) {
        case 0x0:
            // LB
            processor->R[instruction.itype.rd] = (unsigned) (load(memory, processor->R[instruction.itype.rs1] + sign_extend_number(instruction.itype.imm, 12), LENGTH_BYTE));
            processor->PC += 4;
            break;
        case 0x1:
            // LH
            processor->R[instruction.itype.rd] = (unsigned) (load(memory, processor->R[instruction.itype.rs1] + sign_extend_number(instruction.itype.imm, 12), LENGTH_HALF_WORD));
            processor->PC += 4;
            break;
        case 0x2:
            // LW
            processor->R[instruction.itype.rd] = (unsigned) (load(memory, processor->R[instruction.itype.rs1] + sign_extend_number(instruction.itype.imm, 12), LENGTH_WORD));
            processor->PC += 4;
            break;
        default:
            handle_invalid_instruction(instruction);
            break;
    }
}

void execute_store(Instruction instruction, Processor *processor, Byte *memory) {
    switch (instruction.stype.funct3) {
        case 0x0:
            // SB
            store(memory, processor->R[instruction.itype.rs1] + get_store_offset(instruction), LENGTH_BYTE, processor->R[instruction.itype.rd]);
            processor->PC += 4;
            break;
        case 0x1:
            // SH
            store(memory, processor->R[instruction.itype.rs1] + get_store_offset(instruction), LENGTH_HALF_WORD, processor->R[instruction.itype.rd]);
            processor->PC += 4;
            break;
        case 0x2:
            // SW
            store(memory, processor->R[instruction.itype.rs1] + get_store_offset(instruction), LENGTH_WORD, processor->R[instruction.itype.rd]);
            processor->PC += 4;
            break;
        default:
            handle_invalid_instruction(instruction);
            exit(-1);
            break;
    }
}

void execute_jal(Instruction instruction, Processor *processor) {
    processor->R[instruction.ujtype.rd] = processor->PC + 4;
    processor->PC = get_jump_offset(instruction);
}

void execute_lui(Instruction instruction, Processor *processor) {
    processor->R[instruction.utype.rd] = instruction.utype.imm << 12;
    processor->PC += 4;
}

void store(Byte *memory, Address address, Alignment alignment, Word value) {
    fprintf(stderr, "%s", "CALLED STORE");
    if (alignment == LENGTH_WORD) {
        *(uint32_t*) (memory + address) = (uint32_t) value;
    } else if (alignment == LENGTH_HALF_WORD) {
        *(uint16_t*) (memory + address) = (uint16_t) value;
    } else if (alignment == LENGTH_BYTE) {
        *(uint8_t*) (memory + address) = (uint8_t) value;
    } else {
        fprintf(stderr, "%s", "ERROR: Unknown alignment type in store(...)");
        exit(-1);
    }
}

Word load(Byte *memory, Address address, Alignment alignment) {
    fprintf(stderr, "%s", "CALLED LOAD");
    if (alignment == LENGTH_WORD) {
        return *(uint32_t*) (memory + address);
    } else if (alignment == LENGTH_HALF_WORD) {
        return *(uint16_t*) (memory + address);
    } else if (alignment == LENGTH_BYTE) {
        return *(uint8_t*) (memory + address);
    }
    
    fprintf(stderr, "%s", "ERROR: Unknown alignment type in load(...)");
    exit(-1);
}


