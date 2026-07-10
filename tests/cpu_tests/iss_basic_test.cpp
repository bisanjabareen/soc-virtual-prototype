#include "../../core/iss.h"
#include <cstdio>


void test_srl() {
    CPU cpu(0, 4096); 
    cpu.write_mem(0, 0xB3);
    cpu.write_mem(1, 0xD1);
    cpu.write_mem(2, 0x20);
    cpu.write_mem(3, 0x00); 
    cpu.set_reg(1, -1);
    cpu.set_reg(2, 1);
    //cpu.write_mem(0, i); 
    cpu.step();
    if (cpu.get_reg(3) != 0x7FFFFFFF) {
        printf("SRL test failed: expected 0x7FFFFFFF, got %08X\n", cpu.get_reg(3));
    } else {
        printf("SRL test passed\n");
    }
}

void sra() {
    CPU cpu(0, 4096); 
    cpu.write_mem(0, 0xB3);
    cpu.write_mem(1, 0xD1);
    cpu.write_mem(2, 0x20);
    cpu.write_mem(3, 0x40); 
    cpu.set_reg(1, -1);
    cpu.set_reg(2, 1);
    cpu.step();
    if (cpu.get_reg(3) != -1) {
        printf("SRA test failed: expected -1, got %d\n", cpu.get_reg(3));
    } else {
        printf("SRA test passed\n");
    }
}

void sll() {
    CPU cpu(0, 4096); 
    //0x002091B3
    cpu.write_mem(0, 0xB3);
    cpu.write_mem(1, 0x91);
    cpu.write_mem(2, 0x20);
    cpu.write_mem(3, 0x00); 
    cpu.set_reg(1, 1);
    cpu.set_reg(2, 33);
    cpu.step();
    if (cpu.get_reg(3) != 2) {
        printf("SLL test failed: expected 2, got %d\n", cpu.get_reg(3));
    } else {
        printf("SLL test passed\n");
    }
}
void sltiu() {
    CPU cpu(0, 4096); 
    //0xFFF0B193
    cpu.write_mem(0, 0x93);
    cpu.write_mem(1, 0xB1);
    cpu.write_mem(2, 0xF0);
    cpu.write_mem(3, 0xFF); 
    cpu.set_reg(1, 1);
    cpu.step();
    if (cpu.get_reg(3) != 1) {
        printf("SLTIU test failed: expected 1, got %d\n", cpu.get_reg(3));
    } else {
        printf("SLTIU test passed\n");
    }
}


int main () {

    
test_srl();
    sra();
    sll();
    sltiu();
    return 0;
}