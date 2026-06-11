#include "cpu.h"

CPU::CPU(uint32_t initial_pc, uint32_t memory_size) {
    PC = initial_pc;
    memory = std::vector<uint8_t>(memory_size, 0);
    for (int i = 0; i < 32; ++i) {
        registerFile[i] = 0;
    }
}

CPU::~CPU() {
}

uint32_t CPU::fetch() {
    uint32_t instr = read_mem(PC);
    uint32_t instr2 = read_mem(PC + 1);
    uint32_t instr3 = read_mem(PC + 2);
    uint32_t instr4 = read_mem(PC + 3);
    return (instr4 << 24) | (instr3 << 16) | (instr2 << 8) | instr;
}

decoded_instruction_t CPU::decode(uint32_t raw_instr) {
    /***
     * Major RISC-V opcodes:
     * 0000011: 0x03- load
     * 0100011: 0x23 - store
     * 0010011: 0x13- operations with immediates
     * 0110011: 0x33 - operations with registers
     * 1100011: 0x63 - branches
     * 1101111: 0x6F - JAL
     * 1100111: 0x67 - JALR
     * 0110111: 0x37 - LUI
     */
    decoded_instruction_t decoded;
    uint8_t opcode = raw_instr & 0x7F; // Extract the opcode (7 bits)
    uint8_t funct3 = (raw_instr >> 12) & 0x7; // Extract funct3 (3 bits)
    uint8_t funct7 = (raw_instr >> 25) & 0x7F; // Extract funct7 (7 bits)
    switch (decoded.op)
    {
    case 0x03: // load
        switch (funct3) {
            case 0x0: // LB
                decoded.op = Operation::LB;
                decoded.rd = (raw_instr >> 7) & 0x1F; // Extract rd (5 bits)
                decoded.rs1 = (raw_instr >> 15) & 0x1F;
                decoded.imm = (raw_instr >> 20) & 0xFFF; // Extract imm (12 bits)
                break;
            case 0x1: // LH
                decoded.op = Operation::LH;
                break;
            case 0x2: // LW
                decoed.op = Operation::LW;
                break;
            case 0x4: // LBU
                decoded.op = Operation::LBU;
                break;
            case 0x5: // LHU
                decoded.op = Operation::LHU;
                break;
            default:
                break;
        }
        break;
    case 0x23: // store
        /* code */
        break; 
    case 0x13: // operations with immediates
        /* code */
        break;
    case 0x33: // operations with registers
        /* code */ 

        break;
    case 0x23: // store
        /* code */
        break; 
    case 0x13: // operations with immediates
        /* code */
        break;
    case 0x33: // operations with registers
        /* code */ 
        break;
    case 0x63: // branches
        /* code */
        break; 
    case 0x6F: // JAL
        /* code */
        break; 
    case 0x67: // JALR
        /* code */  
        break;
    case 0x37: // LUI
        /* code */
        break;
    
    default:
        break;
    }
}

void CPU::execute(const decoded_instruction_t& instr) {
}                               
uint8_t CPU::read_mem(uint32_t addr) {

}
void CPU::write_mem(uint32_t addr, uint8_t value) {

}

void CPU::run() {
    while (true) {
        uint32_t raw_instruction = fetch();
        decoded_instruction_t instruction = decode(raw_instruction);
        execute(instruction);
    }
}

