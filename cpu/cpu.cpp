#include "cpu.h"

CPU::CPU() {

}

CPU::~CPU() {

}

void CPU::fetch() {

}

void CPU::decode() {

}

void CPU::execute() {

}

void CPU::memory() {
  
}

void CPU::run() {
    while (true) {
        fetch();
        decode();
        execute();
        // Add any necessary cycle counting or timing here
    }
}

