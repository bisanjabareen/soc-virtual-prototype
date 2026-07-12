#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <systemc>
#include <cstdio>
using namespace sc_core;
#define DATA_REG 0x00
#define STATUS_REG 0x04

SC_MODULE(UART) {
    
    tlm_utils::simple_target_socket<UART> socket;

    SC_CTOR(UART) : socket("socket") {
        socket.register_b_transport(this, &UART::do_transaction);
    }

    void do_transaction(tlm::tlm_generic_payload& t, sc_time& delay) {
        auto command = t.get_command();
        auto addr = t.get_address();
        auto data_ptr = t.get_data_ptr();
        auto len = t.get_data_length();
        if (addr == DATA_REG || addr == STATUS_REG) {
            if (len != 1) {
                t.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
                return;
            }
        }
        if (command == tlm::TLM_WRITE_COMMAND && addr == DATA_REG) {
            putchar(*data_ptr);
            fflush(stdout);
        } else if (command == tlm::TLM_READ_COMMAND && addr == STATUS_REG) {
            *data_ptr = 0x01; // ready for now, i'll come back to this later
        } else {
            t.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
            return;
        }

        t.set_response_status(tlm::TLM_OK_RESPONSE); 
    }
};