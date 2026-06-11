#include "cpu.h"

CPU::CPU() {

}

CPU::~CPU() {
}

uint32_t CPU::fetch() {

}

decoded_instruction_t CPU::decode(uint32_t raw_instr) {

}

void CPU::execute(const decoded_instruction_t& instr) {
}                               
uint8_t CPU::read_mem(uint32_t addr) {

}
void CPU::write_mem(uint32_t addr, uint8_t value) {

}

void CPU::run() {
    while (true) {
        uint32_t instruction = fetch();
        decode();
        execute();
    }
}

