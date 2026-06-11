#pragma once
#include <cstdint>
#include <vector>

enum class Operation {
    ADD,
    SUB,
    ADDI,
    LW,    
    SW,    
    BEQ,   
    LUI,   
};
struct decoded_instruction_t {
    Operation op;           
    uint8_t rd;                                 
    uint8_t rs1;                  
    uint8_t rs2;                 
    int32_t imm;
};

class CPU {
private:
    uint32_t PC;                    
    std::vector<uint8_t> memory;    
    uint32_t registerFile[32];      

public:
    CPU(uint32_t initial_pc, unint32_t memory_size);
    ~CPU();

    uint32_t fetch();                                
    decoded_instruction_t decode(uint32_t raw_instr);         
    void execute(const decoded_instruction_t& instr);   
    void run();                                      
    uint8_t read_mem(uint32_t addr);
    void write_mem(uint32_t addr, uint8_t value);
};