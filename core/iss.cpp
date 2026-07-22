#include "iss.h"
#include <cstdio>

CPU::CPU(sc_core::sc_module_name name, uint32_t initial_pc) : 
                    sc_core::sc_module(name){
    PC = initial_pc;
    //memory = std::vector<uint8_t>(memory_size, 0);
    for (int i = 0; i < 32; ++i) {
        registerFile[i] = 0;
    }
    last_PC = 0;
    SC_THREAD(run);
}

CPU::~CPU() {
}

int32_t sign_extend(uint32_t value, int bits) {
    int32_t m = 1 << (bits - 1);
    return (value ^ m) - m;
}

int32_t CPU::get_reg(uint8_t reg) {
    if (reg < 32) {
        printf("Getting register %d: %d\n", reg, registerFile[reg]);
        return registerFile[reg];
    } else {
        return 0; 
    }
}

void CPU::set_reg(uint8_t reg, int32_t value) {
    if (reg < 32) {
        registerFile[reg] = value;
    }
}



uint32_t CPU::fetch() {
    uint32_t instr = read_mem(PC, 4);
    return instr;
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
        decoded.imm = sign_extend(decoded.imm, 12);
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
        decoded.imm = sign_extend(decoded.imm, 12);
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
        decoded.imm = sign_extend(decoded.imm, 12);
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
        decoded.imm = ((raw_instr >> 7) & 0x1E) | ((raw_instr >> 25) & 0x3F) << 5 | ((raw_instr >> 7) & 0x1) << 11 | ((raw_instr >> 31) & 0x1) << 12;
        decoded.imm = sign_extend(decoded.imm, 13);
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
        decoded.imm = ((raw_instr >> 20) & 0x7FE) | ((raw_instr >> 20) & 0x1) << 11 | ((raw_instr >> 12) & 0xFF) << 12 | ((raw_instr >> 31) & 0x1) << 20;
        decoded.imm = sign_extend(decoded.imm, 21);
        decoded.op = Operation::JAL;
        break; 
    case 0x67: // JALR
        decoded.rd = (raw_instr >> 7) & 0x1F;
        decoded.rs1 = (raw_instr >> 15) & 0x1F;
        decoded.imm = (raw_instr >> 20) & 0xFFF;
        decoded.imm = sign_extend(decoded.imm, 12);
        decoded.op = Operation::JALR;
        break;
    case 0x37: // LUI
        decoded.rd = (raw_instr >> 7) & 0x1F;
        // no need for sign extension here since LUI uses the upper 20 bits directly
        decoded.imm = raw_instr & 0xFFFFF000;
        //decoded.immm = sign_extend(decoded.imm, 32);
        decoded.op = Operation::LUI;
        break;
    case 0x17: // AUIPC
        decoded.rd = (raw_instr >> 7) & 0x1F;
        // no need for sign extension here since AUIPC uses the upper 20 bits directly
        decoded.imm = raw_instr & 0xFFFFF000;
        decoded.op = Operation::AUIPC;
        break;
    default:
        decoded.op = Operation::NOP; // Unknown instruction, treat as NOP
        break;
    }
    return decoded;
}

void CPU::execute(const decoded_instruction_t& instr) {
    switch(instr.op){
        case Operation::LB: {
            uint32_t addr = registerFile[instr.rs1] + instr.imm;
            int8_t value = static_cast<int8_t>(read_mem(addr, 1));
            registerFile[instr.rd] = static_cast<int32_t>(value);
            break;
        }
        case Operation::LH: {
            uint32_t addr = registerFile[instr.rs1] + instr.imm;
            int16_t value = static_cast<int16_t>(read_mem(addr, 2));
            registerFile[instr.rd] = static_cast<int32_t>(value);
            break;
        }
        case Operation::LBU: {
            uint32_t addr = registerFile[instr.rs1] + instr.imm;
            uint8_t value = read_mem(addr, 1);
            registerFile[instr.rd] = static_cast<int32_t>(value);
            break;
        }
        case Operation::LHU: {
            uint32_t addr = registerFile[instr.rs1] + instr.imm;
            uint16_t value = read_mem(addr, 2);
            registerFile[instr.rd] = static_cast<int32_t>(value);
            break;
        }
        case Operation::ADD:
            registerFile[instr.rd] = registerFile[instr.rs1] + registerFile[instr.rs2];
            break;
        case Operation::SUB:
            registerFile[instr.rd] = registerFile[instr.rs1] - registerFile[instr.rs2];
            break;
        case Operation::SLL:
            registerFile[instr.rd] = (uint32_t)registerFile[instr.rs1] << (registerFile[instr.rs2] & 0X1F);
            break;
        case Operation::SLT:
            registerFile[instr.rd] = registerFile[instr.rs1] < registerFile[instr.rs2];
            break;
        case Operation::SLTU:
            registerFile[instr.rd] = (uint32_t)registerFile[instr.rs1] < (uint32_t)registerFile[instr.rs2];
            break;
        case Operation::XOR:
            registerFile[instr.rd] = registerFile[instr.rs1] ^ registerFile[instr.rs2];
            break;
        case Operation::SRL:
            registerFile[instr.rd] = (uint32_t)registerFile[instr.rs1] >> (registerFile[instr.rs2] & 0X1F);
            break;
        case Operation::SRA:
            registerFile[instr.rd] = (int32_t)registerFile[instr.rs1] >> (registerFile[instr.rs2] & 0X1F);
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
            if (registerFile[instr.rs1] < registerFile[instr.rs2]) {
                PC = last_PC + instr.imm;
            }
            break;
        case Operation::BGE:
            if (registerFile[instr.rs1] >= registerFile[instr.rs2]) {
                PC = last_PC + instr.imm;
            }
            break;
        case Operation::BLTU:
            if ((uint32_t)registerFile[instr.rs1] < (uint32_t)registerFile[instr.rs2]) {
                PC = last_PC + instr.imm;
            }
            break;
        case Operation::BGEU:
            if ((uint32_t)registerFile[instr.rs1] >= (uint32_t)registerFile[instr.rs2]) {
                PC = last_PC + instr.imm;
            }
            break;
        case Operation::ADDI:
            registerFile[instr.rd] = registerFile[instr.rs1] + instr.imm;
            break;
        case Operation::SLTI:
            registerFile[instr.rd] = registerFile[instr.rs1] < instr.imm;
            break;
        case Operation::SLTIU:
            registerFile[instr.rd] = (uint32_t)registerFile[instr.rs1] < (uint32_t)instr.imm;
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
            uint32_t value = read_mem(addr, 4);
            registerFile[instr.rd] = value;
            break;
        }
        case Operation::SW: {
            uint32_t addr = registerFile[instr.rs1] + instr.imm;
            uint32_t value = registerFile[instr.rs2];
            write_mem(addr, value, 4);
            //write_mem(addr + 1, (value >> 8) & 0xFF);
            //write_mem(addr + 2, (value >> 16) & 0xFF);
            //write_mem(addr + 3, (value >> 24) & 0xFF);
            break;
        }
        case Operation::SH: {
            uint32_t addr = registerFile[instr.rs1] + instr.imm;
            uint16_t value = registerFile[instr.rs2] & 0xFFFF;
            write_mem(addr, value, 2);
            //write_mem(addr + 1, (value >> 8) & 0xFF, 1);
            break;
        }
        case Operation::SB: {
            uint32_t addr = registerFile[instr.rs1] + instr.imm;
            uint8_t value = registerFile[instr.rs2] & 0xFF;
            write_mem(addr, value, 1);
            break;
        }
        case Operation::LUI:
            registerFile[instr.rd] = instr.imm;
            break;
        case Operation::AUIPC:
            registerFile[instr.rd] = last_PC + instr.imm;
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
uint32_t CPU::read_mem(uint32_t addr, uint32_t len) {
    tlm::tlm_generic_payload trans;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
    uint32_t data = 0;

    trans.set_command(tlm::TLM_READ_COMMAND);
    trans.set_address(addr);
    trans.set_data_ptr(reinterpret_cast<unsigned char*>(&data));
    trans.set_data_length(len);
    trans.set_streaming_width(len);
    trans.set_byte_enable_ptr(nullptr);
    trans.set_dmi_allowed(false);
    trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    socket->b_transport(trans, delay);

    if (trans.is_response_error()) {
        SC_REPORT_ERROR("CPU", "read transaction failed");
    }
        
    return data;
}
void CPU::write_mem(uint32_t addr, uint32_t value, uint32_t len) {
    tlm::tlm_generic_payload trans;
    sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

    trans.set_command(tlm::TLM_WRITE_COMMAND);
    trans.set_address(addr);
    trans.set_data_ptr(reinterpret_cast<unsigned char*>(&value));
    trans.set_data_length(len);
    trans.set_streaming_width(len);
    trans.set_byte_enable_ptr(nullptr);
    trans.set_dmi_allowed(false);
    trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

    socket->b_transport(trans, delay);

    if (trans.is_response_error()) {
        SC_REPORT_ERROR("CPU", "write transaction failed");
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
        wait(10, sc_core::SC_NS);
    }
}

void CPU::step() {
    last_PC = PC;
    uint32_t raw_instruction = fetch();
    PC += 4;
    decoded_instruction_t instruction = decode(raw_instruction);
    execute(instruction);
    registerFile[0] = 0;
    
}

