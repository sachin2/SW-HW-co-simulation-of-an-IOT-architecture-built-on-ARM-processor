#ifndef _ALU_H_
#define _ALU_H_
#include <systemc.h>

template<class T>
SC_MODULE(ALU) {
    
    public:
    
    //internal ports
    T Sum_copy;
    T flag=0;
    
    //control signal masking ports
    sc_uint<10> control_decode;
    
    //top level ports
    sc_in<T> data_op1_i, data_op2_i, control_sig_i;
    sc_out<T> flag_o, result_o;
    
    
    //Constructor
    SC_CTOR(ALU);
    
    //ALU operations
    enum ALUOperations{
    	ADD = 0,
    	SLL = 1,
    	SLTU = 2,
    	SLT = 3,
    	XOR = 4,
    	SRL = 5,
    	OR = 6,
    	AND = 7,
    	SUB = 8
    };
    
    private:
    void controlMethod(void);
    
};

#endif

