#include <systemc>
#include <tlm>
#include <tlm_utils/simple_initiator_socket.h>
#include <tlm_utils/simple_target_socket.h>
#define UART_BASE 0x10000000
#define MEM_BASE 0x80000000

SC_MODULE(BUS)
{

    tlm_utils::simple_initiator_socket<BUS> initiator_socket_uart;
    tlm_utils::simple_target_socket<BUS> target_socket;
    tlm_utils::simple_initiator_socket<BUS> initiator_socket_mem;

    SC_CTOR(BUS) : initiator_socket_uart("initiator_socket_uart"), target_socket("target_socket"), initiator_socket_mem("initiator_socket_mem")
    {
        target_socket.register_b_transport(this, &BUS::b_transport);
    }

    void b_transport(tlm::tlm_generic_payload & trans, sc_core::sc_time & delay)
    {
        uint32_t addr = trans.get_address();

        if (addr >= MEM_BASE)
        {
            trans.set_address(addr - MEM_BASE);
            initiator_socket_mem->b_transport(trans, delay);
        }
        else if (addr >= UART_BASE && addr < UART_BASE + 0x1000)
        {
            trans.set_address(addr - UART_BASE);
            initiator_socket_uart->b_transport(trans, delay);
        }
        else
        {
            trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
        }
    }
};