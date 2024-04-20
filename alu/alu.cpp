#include "alu.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

//Constructor
template<class T>
ALU<T>::ALU(sc_module_name nm)
  : sc_module(nm)
{
	cout << "Inside ALU Constructor" << endl;
        SC_THREAD(controlMethod);
}

template<class T>        
void ALU<T>::controlMethod() 
{

while(true)
{

//control signal capture
control_decode = control_sig_i.read();

//control signal masking
control_decode = ((control_decode.range(8,8) << 3) | (control_decode.range(2,0)));

//control decoding and ALU operations
switch(control_decode)
{
    	case ADD:
    		
    		Sum_copy=data_op1_i.read() + data_op2_i.read();			//ADD
    		break;
    	case SLL:
    		Sum_copy=data_op1_i.read() << data_op2_i.read();		//SLL
    		break;
    	case SLTU: 
    		Sum_copy = (data_op1_i.read() < data_op2_i.read()) ? 1: 0;  	//SLTU, BGE, BLT
    		break;
    	case SLT:
    		Sum_copy = (static_cast<int>(data_op1_i.read()) < static_cast<int>(data_op2_i.read())) ? 1: 0;  //SLT
    		break;
    	case XOR:	
    		Sum_copy=data_op1_i.read() xor data_op2_i.read();		//XOR
    		break;
    	case SRL:
    		Sum_copy=data_op1_i.read() >> data_op2_i.read();		//SRL
    		break;
    	case OR:	    		
    		Sum_copy=data_op1_i.read() | data_op2_i.read();			//OR	    		
    		break;
    	case AND:	    		
    		Sum_copy=data_op1_i.read() & data_op2_i.read();			//AND
    		break;
    	case SUB:	    		
    		Sum_copy=data_op1_i.read() - data_op2_i.read();			//SUB	    		
    		break;
    	default:Sum_copy = 0;
    		cout << "Invalid Control Signal" << endl;
    		break;
}

//Setting flags
if(control_decode)
{ 
	
	//Zero Flag
	if(Sum_copy == 0)
		flag = 1;
		
	//Carry Flag
	else if(control_decode == ADD || control_decode == OR || control_decode == AND)
	{
		if(data_op1_i.read() >= 0x80000000 and data_op2_i.read() >= 0x80000000)
			flag = 2;
		else
			flag = 0;
	}
	
	//negative flag
	else if(control_decode == SUB)
	{
		if(data_op1_i.read()  < data_op2_i.read())
			flag = 3;
		else
			flag=0;
	}
		
	//No Flag Set
	else 
		flag = 0;
		
}

//flag and sum write
result_o.write(Sum_copy);
wait(10,SC_NS);
flag_o.write(4);
wait(10,SC_NS);


flag_o.write(flag);

}
}   




