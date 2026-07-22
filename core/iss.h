#pragma once
#include <cstdint>
#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>

enum class Operation {
    NOP,
    ADD,
    SUB,
    ADDI,
    LW,
    LB,
    LH,
    LBU,
    LHU,
    SB,
    SH,
    SLTI,
    SLTIU,
    XORI,
    ANDI,
    SLTU,
    XOR,
    SRL,
    SRA,
    OR,
    AND,
    BNE,
    BLT,
    BGE,
    BLTU,
    BGEU,
    JAL,
    JALR,
    AUIPC,
    SLL,
    SW,    
    BEQ,   
    LUI,
    ORI,
    SLT
};
struct decoded_instruction_t {
    Operation op;           
    uint8_t rd;                                 
    uint8_t rs1;                  
    uint8_t rs2;                 
    int32_t imm;
};

struct CPU : sc_core::sc_module {
    tlm_utils::simple_initiator_socket<CPU> socket;
    uint32_t PC;     
    uint32_t last_PC;               
    //std::vector<uint8_t> memory;    
    int32_t registerFile[32];    
    CPU(sc_core::sc_module_name name, uint32_t initial_pc);
    ~CPU();

    uint32_t fetch();                                
    decoded_instruction_t decode(uint32_t raw_instr);         
    void execute(const decoded_instruction_t& instr);   
    void run();                                      
    uint32_t read_mem(uint32_t addr, uint32_t len = 1);
    void write_mem(uint32_t addr, uint32_t value, uint32_t len = 1);
    int32_t get_reg(uint8_t reg);
    void set_reg(uint8_t reg, int32_t value);
    void step();
};