#include "cpu.h"

CPU::CPU(uint32_t initial_pc, uint32_t memory_size) {
    PC = initial_pc;
    memory = std::vector<uint8_t>(memory_size, 0);
    for (int i = 0; i < 32; ++i) {
        registerFile[i] = 0;
    }
    last_PC = 0;
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
     * 0010111: 0x17 - AUIPC
     */
    decoded_instruction_t decoded;
    uint8_t opcode = raw_instr & 0x7F; 
    uint8_t funct3 = (raw_instr >> 12) & 0x7; 
    uint8_t funct7 = (raw_instr >> 25) & 0x7F; 
    switch (opcode)
    {
    case 0x03: // load
        decoded.rd = (raw_instr >> 7) & 0x1F; 
        decoded.rs1 = (raw_instr >> 15) & 0x1F;
        decoded.imm = (raw_instr >> 20) & 0xFFF; 
        switch (funct3) {
            case 0x0: // LB
                decoded.op = Operation::LB;
                break;
            case 0x1: // LH
                decoded.op = Operation::LH;

                break;
            case 0x2: // LW
                decoded.op = Operation::LW;
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
        decoded.rs1 = (raw_instr >> 15) & 0x1F; 
        decoded.rs2 = (raw_instr >> 20) & 0x1F;
        decoded.imm = ((raw_instr >> 7) & 0x1F) | ((raw_instr >> 25) & 0x7F) << 5;
        switch (funct3) {
            case 0x0: // SB
                decoded.op = Operation::SB;
                break;
            case 0x1: // SH
                decoded.op = Operation::SH;
                break;
            case 0x2: // SW
                decoded.op = Operation::SW;
                break;
            default:
                break;
        }
        break; 
    case 0x13: // operations with immediates
        decoded.rd = (raw_instr >> 7) & 0x1F; 
        decoded.rs1 = (raw_instr >> 15) & 0x1F;
        decoded.imm = (raw_instr >> 20) & 0xFFF; 
        switch (funct3) {
            case 0x0: // ADDI
                decoded.op = Operation::ADDI;
                break;
            case 0x2: // SLTI
                decoded.op = Operation::SLTI;
                break;
            case 0x3: // SLTIU
                decoded.op = Operation::SLTIU;
                break;
            case 0x4: // XORI
                decoded.op = Operation::XORI;
                break;
            case 0x6: // ORI
                decoded.op = Operation::ORI;
                break;
            case 0x7: // ANDI
                decoded.op = Operation::ANDI;
                break;
            default:
                break;
        }
        break;
    case 0x33: // operations with registers
        decoded.rd = (raw_instr >> 7) & 0x1F; 
        decoded.rs1 = (raw_instr >> 15) & 0x1F;
        decoded.rs2 = (raw_instr >> 20) & 0x1F;
        switch (funct3) {
            case 0x0: // ADD/SUB
                if (funct7 == 0x00) {
                    decoded.op = Operation::ADD;
                } else if (funct7 == 0x20) {
                    decoded.op = Operation::SUB;
                }
                break;
            case 0x1: // SLL
                decoded.op = Operation::SLL;
                break;
            case 0x2: // SLT
                decoded.op = Operation::SLT;
                break;
            case 0x3: // SLTU
                decoded.op = Operation::SLTU;
                break;
            case 0x4: // XOR
                decoded.op = Operation::XOR;
                break;
            case 0x5: // SRL/SRA
                if (funct7 == 0x00) {
                    decoded.op = Operation::SRL;
                } else if (funct7 == 0x20) {
                    decoded.op = Operation::SRA;
                }
                break;
            case 0x6: // OR
                decoded.op = Operation::OR;
                break;
            case 0x7: // AND
                decoded.op = Operation::AND;
                break;
            default:
                break;
        }
        break;
    case 0x63: // branches
        decoded.rs1 = (raw_instr >> 15) & 0x1F;
        decoded.rs2 = (raw_instr >> 20) & 0x1F;
        decoded.imm = ((raw_instr >> 7) & 0x1E) | ((raw_instr >> 25) & 0x3F) << 5 | ((raw_instr >> 8) & 0x1) << 11 | ((raw_instr >> 31) & 0x1) << 12;
        switch (funct3) {
            case 0x0: // BEQ
                decoded.op = Operation::BEQ;
                break;
            case 0x1: // BNE
                decoded.op = Operation::BNE;
                break;
            case 0x4: // BLT
                decoded.op = Operation::BLT;
                break;
            case 0x5: // BGE
                decoded.op = Operation::BGE;
                break;
            case 0x6: // BLTU
                decoded.op = Operation::BLTU;
                break;
            case 0x7: // BGEU
                decoded.op = Operation::BGEU;
                break;
            default:
                break;
        }
        break; 
    case 0x6F: // JAL
        decoded.rd = (raw_instr >> 7) & 0x1F;
        decoded.imm = ((raw_instr >> 21) & 0x3FF) | ((raw_instr >> 20) & 0x1) << 10 | ((raw_instr >> 12) & 0xFF) << 11 | ((raw_instr >> 31) & 0x1) << 19;
        decoded.op = Operation::JAL;
        break; 
    case 0x67: // JALR
        decoded.rd = (raw_instr >> 7) & 0x1F;
        decoded.rs1 = (raw_instr >> 15) & 0x1F;
        decoded.imm = (raw_instr >> 20) & 0xFFF;
        decoded.op = Operation::JALR;
        break;
    case 0x37: // LUI
        decoded.rd = (raw_instr >> 7) & 0x1F;
        decoded.imm = raw_instr & 0xFFFFF000;
        decoded.op = Operation::LUI;
        break;
    case 0x17: // AUIPC
        decoded.rd = (raw_instr >> 7) & 0x1F;
        decoded.imm = raw_instr & 0xFFFFF000;
        decoded.op = Operation::AUIPC;
        break;
    default:
        break;
    }
    return decoded;
}

void CPU::execute(const decoded_instruction_t& instr) {
    switch(instr.op){
        case Operation::ADD:
            registerFile[instr.rd] = registerFile[instr.rs1] + registerFile[instr.rs2];
            break;
        case Operation::SUB:
            registerFile[instr.rd] = registerFile[instr.rs1] - registerFile[instr.rs2];
            break;
        case Operation::SLL:
            registerFile[instr.rd] = registerFile[instr.rs1] << registerFile[instr.rs2];
            break;
        case Operation::SLT:
            registerFile[instr.rd] = (int32_t)registerFile[instr.rs1] < (int32_t)registerFile[instr.rs2];
            break;
        case Operation::SLTU:
            registerFile[instr.rd] = registerFile[instr.rs1] < registerFile[instr.rs2];
            break;
        case Operation::XOR:
            registerFile[instr.rd] = registerFile[instr.rs1] ^ registerFile[instr.rs2];
            break;
        case Operation::SRL:
            registerFile[instr.rd] = registerFile[instr.rs1] >> registerFile[instr.rs2];
            break;
        case Operation::SRA:
            registerFile[instr.rd] = (int32_t)registerFile[instr.rs1] >> registerFile[instr.rs2];
            break;
        case Operation::OR:
            registerFile[instr.rd] = registerFile[instr.rs1] | registerFile[instr.rs2];
            break;
        case Operation::AND:
            registerFile[instr.rd] = registerFile[instr.rs1] & registerFile[instr.rs2];
            break;
        case Operation::BEQ:
            if (registerFile[instr.rs1] == registerFile[instr.rs2]) {
                PC = last_PC + instr.imm;
            }
            break;
        case Operation::BNE:
            if (registerFile[instr.rs1] != registerFile[instr.rs2]) {
                PC = last_PC + instr.imm;
            }
            break;
        case Operation::BLT:
            if ((int32_t)registerFile[instr.rs1] < (int32_t)registerFile[instr.rs2]) {
                PC = last_PC + instr.imm;
            }
            break;
        case Operation::BGE:
            if (registerFile[instr.rs1] >= registerFile[instr.rs2]) {
                PC = last_PC + instr.imm;
            }
            break;
        case Operation::BLTU:
            if (registerFile[instr.rs1] < registerFile[instr.rs2]) {
                PC += instr.imm;
            }
            break;
        case Operation::BGEU:
            if (registerFile[instr.rs1] >= registerFile[instr.rs2]) {
                PC += instr.imm;
           }
            break;
        case Operation::ADDI:
            registerFile[instr.rd] = registerFile[instr.rs1] + instr.imm;
            break;
        case Operation::SLTI:
            registerFile[instr.rd] = registerFile[instr.rs1] < instr.imm;
            break;
        case Operation::SLTIU:
            registerFile[instr.rd] = registerFile[instr.rs1] < instr.imm;
            break;
        case Operation::XORI:
            registerFile[instr.rd] = registerFile[instr.rs1] ^ instr.imm;
            break;
        case Operation::ORI:
            registerFile[instr.rd] = registerFile[instr.rs1] | instr.imm;
            break;
        case Operation::ANDI:
            registerFile[instr.rd] = registerFile[instr.rs1] & instr.imm;
            break;
        case Operation::LW: {
            uint32_t addr = registerFile[instr.rs1] + instr.imm;
            uint32_t value = read_mem(addr) | (read_mem(addr + 1) << 8) | (read_mem(addr + 2) << 16) | (read_mem(addr + 3) << 24);
            registerFile[instr.rd] = value;
            break;
        }
        case Operation::SW: {
            uint32_t addr = registerFile[instr.rs1] + instr.imm;
            uint32_t value = registerFile[instr.rs2];
            write_mem(addr, value & 0xFF);
            write_mem(addr + 1, (value >> 8) & 0xFF);
            write_mem(addr + 2, (value >> 16) & 0xFF);
            write_mem(addr + 3, (value >> 24) & 0xFF);
            break;
        }
        case Operation::LUI:
            registerFile[instr.rd] = instr.imm;
            break;
        case Operation::AUIPC:
            registerFile[instr.rd] = PC + instr.imm;
            break; 
        case Operation::JAL: {
            uint32_t return_addr = last_PC + 4; 
            PC = last_PC + instr.imm; 
            registerFile[instr.rd] = return_addr; 
            break;
        }
        case Operation::JALR: {
            uint32_t return_addr = last_PC + 4; 
            PC = (registerFile[instr.rs1] + instr.imm) & ~1; 
            registerFile[instr.rd] = return_addr; 
            break;
        }
        default:
            break;
    }

}                               
uint8_t CPU::read_mem(uint32_t addr) {
    if (addr < memory.size()) {
        return memory[addr];
    } else {
        return 0; 
    }
}
void CPU::write_mem(uint32_t addr, uint8_t value) {
    if (addr < memory.size()) {
        memory[addr] = value;
    }
}

void CPU::run() {
    while (true) {
        last_PC = PC;
        uint32_t raw_instruction = fetch();
        PC += 4;
        decoded_instruction_t instruction = decode(raw_instruction);
        execute(instruction);
        registerFile[0] = 0;
    }
}

