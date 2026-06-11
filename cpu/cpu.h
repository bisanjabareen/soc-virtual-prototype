#include <cstdint>
#include <vector>

enum class Op {
    ADD,
    SUB,
    ADDI,
    LW,    
    SW,    
    BEQ,   
    LUI,   
};
struct decoded_instruction_t {
    uint8_t opcode;               
    uint8_t rd;                   
    uint8_t funct3;               
    uint8_t rs1;                  
    uint8_t rs2;                  
    uint8_t funct7;
    uint32_t imm;
};

class CPU {
private:
    uint32_t PC;                    
    std::vector<uint8_t> memory;    
    uint32_t registerFile[32];      

public:
    CPU();
    ~CPU();

    uint32_t fetch();                                
    decoded_instruction_t decode(uint32_t raw);         
    void execute(const decoded_instruction_t& instr);   
    void run();                                      
    uint8_t read_mem(uint32_t addr);
    void write_mem(uint32_t addr, uint8_t value);
};