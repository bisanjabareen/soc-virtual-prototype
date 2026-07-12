#include <systemc>
#include "bus/bus.h"
#include "memory/memory.h"
#include "core/iss.h"
using namespace sc_core;

int sc_main(int argc, char* argv[]) {
    BUS bus("bus");
    MEMORY memory("memory");
    CPU cpu("cpu", MEM_BASE); 
    cpu.socket.bind(bus.target_socket);
    bus.initiator_socket.bind(memory.socket);

    // small test: set x1 and x2 to 1 and 2 and then run add x1,x2,x3
    cpu.set_reg(1, 40);
    cpu.set_reg(2, 2);
    memory.load(0, 0x002081B3, 4);

    sc_start(15, SC_NS); 

    int result = cpu.get_reg(3);
    if (result == 42) {
        printf("PASS: x3 = %d\n", result);
    } 
    else {
        printf("FAIL: x3 = %d\n", result);
    }              
    return 0;
}