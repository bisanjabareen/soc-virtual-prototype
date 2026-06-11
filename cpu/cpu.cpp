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

void CPU::run() {
    while (true) {
        fetch();
        decode();
        execute();
    }
}

