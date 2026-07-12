#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>

#define MEM_BASE 0x80000000 

SC_MODULE(BUS) {
    
    tlm_utils::simple_initiator_socket<BUS> initiator_socket;
    tlm_utils::simple_target_socket<BUS> target_socket;

    SC_CTOR(BUS) : initiator_socket("initiator_socket"), target_socket("target_socket") {
        //target_socket.bind(initiator_socket); (done in sc_main)
        target_socket.register_b_transport(this, &BUS::b_transport);
    }

    void b_transport(tlm::tlm_generic_payload& trans, sc_core::sc_time& delay) {
        if (trans.get_address() >= MEM_BASE) {
            trans.set_address(trans.get_address() - MEM_BASE); 
            initiator_socket->b_transport(trans, delay);
        }
        
    }
};