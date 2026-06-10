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
    void fetch(uint32_t* instruction);
    void decode(uint32_t* instruction);
    void execute(uint32_t* instruction);
    void run(uint32_t* instruction);                     

    uint8_t read_mem(uint32_t addr);
    void write_mem(uint32_t addr, uint8_t value);
};