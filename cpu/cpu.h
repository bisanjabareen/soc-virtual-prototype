#include <cstdint>
#include <vector>
class CPU {
private:
    // CPU registers
    uint8_t A;  // Accumulator
    uint8_t X;  // X index register
    uint8_t Y;  // Y index register
    uint8_t SP; // Stack pointer
    uint16_t PC; // Program counter
    uint8_t P;  // Processor status
    std::vector<uint8_t> memory; // Memory
    uint32_t registerFile[32]; // Cycle count
public:
    CPU();
    ~CPU();
    void fetch();
    void decode();
    void execute();
    void memory();
    void run();
}