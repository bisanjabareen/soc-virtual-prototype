#include "../core/iss.h"
#include <cstdio>

int main() {
    CPU cpu(0, 4096);   // start PC at 0, 4KB memory

    // Hand-encoded: add x3, x1, x2
    //   opcode=0x33, rd=3, funct3=0, rs1=1, rs2=2, funct7=0
    //   encoding: 0x002081B3
    uint32_t add_instr = 0x002081B3;

    // place it in memory at address 0 (little-endian: low byte first)
    cpu.write_mem(0, add_instr & 0xFF);
    cpu.write_mem(1, (add_instr >> 8)  & 0xFF);
    cpu.write_mem(2, (add_instr >> 16) & 0xFF);
    cpu.write_mem(3, (add_instr >> 24) & 0xFF);

    // set x1 = 10, x2 = 32  (you'll need a way to set registers)
    cpu.set_reg(1, 10);
    cpu.set_reg(2, 32);

    // run ONE instruction (you'll want a step() that does one fetch/decode/execute)
    cpu.step();

    printf("x3 = %d (expected 42)\n", cpu.get_reg(3));
    return 0;
}