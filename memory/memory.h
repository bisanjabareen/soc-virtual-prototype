#include <systemc>
#include <tlm>
#include <tlm_utils/simple_target_socket.h>
#include <vector>
#include <algorithm>
using namespace sc_core;


SC_MODULE(MEMORY) {
    tlm_utils::simple_target_socket<MEMORY> socket;

    SC_CTOR(MEMORY) : socket("socket"), mem(4096, 0) {
    socket.register_b_transport(this, &MEMORY::do_transaction);
    }
    void load(uint32_t addr, uint32_t value, uint32_t len) {
    for (uint32_t i = 0; i < len; ++i)
        mem[addr + i] = (value >> (8 * i)) & 0xFF;
    
    }

private:
    std::vector<uint8_t> mem;    // (2) bytes — byte-addressable, you got this


    void do_transaction(tlm::tlm_generic_payload& t, sc_time& delay) {
        auto command = t.get_command();
        auto addr = t.get_address();
        auto data_ptr = t.get_data_ptr();
        auto len = t.get_data_length();

        if (addr + len > mem.size()) {
            t.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
            return;
        }
        
        if(command == tlm::TLM_WRITE_COMMAND) {
            //mem = std::vector<uint8_t>(data_ptr, data_ptr + len);
            for (unsigned int i = 0; i < len; ++i) {
                mem[addr + i] = *(data_ptr + i);
            }
        } else if(command == tlm::TLM_READ_COMMAND) {
            for (unsigned int i = 0; i < len; ++i) {
                *(data_ptr + i) = mem[addr + i];
            }
        } else {
            t.set_response_status(tlm::TLM_COMMAND_ERROR_RESPONSE);
            return;
        }

        t.set_response_status(tlm::TLM_OK_RESPONSE); 
    }
};