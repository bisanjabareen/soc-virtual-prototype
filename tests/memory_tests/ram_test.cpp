#include "../../memory/memory.h"
#include <tlm_utils/simple_initiator_socket.h>
#include <iostream>


SC_MODULE(TB) {
    tlm_utils::simple_initiator_socket<TB> socket;
    int failures = 0;

    SC_CTOR(TB) : socket("socket") {
        SC_THREAD(run);
    }

    void run() {
        tlm::tlm_generic_payload trans;
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;

        test_write_read();
        test_out_of_bounds_write();

        std::cout << (failures ? "FAIL" : "PASS") << std::endl;
    }
    void test_write_read() {
        // Implement write and read test
        tlm::tlm_generic_payload trans;
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;


        uint32_t out_data = 0xDEADBEEF;

        trans.set_command(tlm::TLM_WRITE_COMMAND);
        trans.set_address(0);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(&out_data));
        trans.set_data_length(4);
        trans.set_streaming_width(4);
        trans.set_byte_enable_ptr(nullptr);
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        socket->b_transport(trans, delay);

        if (trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
        std::cerr << "Write transaction failed with status: " << trans.get_response_status() << std::endl;
        failures++;
        }

        uint32_t in_data = 0;

        trans.set_command(tlm::TLM_READ_COMMAND);
        trans.set_address(0);
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(&in_data));
        trans.set_data_length(4);
        trans.set_streaming_width(4);
        trans.set_byte_enable_ptr(nullptr);
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        socket->b_transport(trans, delay);

        if (trans.get_response_status() != tlm::TLM_OK_RESPONSE) {
            std::cerr << "Read transaction failed with status: " << trans.get_response_status() << std::endl;
            failures++;
        }

        if (in_data != 0xDEADBEEF) {
            std::cerr << "Data mismatch: expected 0xDEADBEEF, got 0x" << std::hex << in_data << std::dec << std::endl;
            failures++;
        }
    }
    void test_out_of_bounds_write() {
        // Implement out-of-bounds write test
        uint32_t out_data = 0xDEADBEEF;
        tlm::tlm_generic_payload trans;
        sc_core::sc_time delay = sc_core::SC_ZERO_TIME;
        trans.set_command(tlm::TLM_WRITE_COMMAND);
        trans.set_address(4096); 
        trans.set_data_ptr(reinterpret_cast<unsigned char*>(&out_data));
        trans.set_data_length(4);
        trans.set_streaming_width(4);
        trans.set_byte_enable_ptr(nullptr);
        trans.set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);

        socket->b_transport(trans, delay);

        if (trans.get_response_status() != tlm::TLM_ADDRESS_ERROR_RESPONSE) {
            std::cerr << "Out-of-bounds write transaction did not fail as expected." << std::endl;
        failures++;
        }   

    }

};


int sc_main(int argc, char* argv[]) {
    MEMORY mem("mem");
    TB tb("tb");
    tb.socket.bind(mem.socket);   
    sc_core::sc_start();
    return tb.failures;

}