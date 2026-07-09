#include <systemc.h>


SC_MODULE(NAME) {
    SC_CTOR(NAME) {
        // Constructor code here
        cout << "Constructor called" << endl;
    }
};

int sc_main(int argc, char* argv[]) {
    NAME name("name_instance");
    sc_start();
    return 0;
}