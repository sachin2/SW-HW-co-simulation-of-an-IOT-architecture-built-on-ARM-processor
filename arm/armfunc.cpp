#include "armfunc.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*!----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/

template<class T, class U>
ARMFunc<T,U>::ARMFunc(sc_module_name nm, sc_event& mFuncDatRdEv)
             : sc_module(nm)
             , mFuncDatRdEv_e(mFuncDatRdEv)
      {
        SC_HAS_PROCESS(ARMFunc); // because no SC_CTOR()
        cout << left << setw(30) << "ConstructorCPP - ARMFunc:   "
             << left << setw(36) << name()
             << endl;
  
       SC_THREAD(getDatThread);
       SC_THREAD(armSequenceThread);
  
       //! binds the fifo to the export
        data_i.bind(dataIn_s);
     }
     
     
/*!----------------------------------------------------------------------------
-- Methods
-----------------------------------------------------------------------------*/

//! Definition: Gets read data from MasterBPI (and so from the memory).

template<class T, class U>
void ARMFunc<T,U>::getDatThread(void)
   {
      while(true)
          {
             // wait on the information, that the read data is available
             wait(mFuncDatRdEv_e);
             // notify, that that the data is available - captured by the algorithm
             readMemRdy_e.notify(SC_ZERO_TIME);
    
             if(MASTERFUNPRINTS)
             armFuncPrint01(dataIn_s.read());

         }
   }



/*!----------------------------------------------------------------------------
-- void armSequenceThread(void): This is the algorithm of the ARM.
-----------------------------------------------------------------------------*/


template<class T, class U>
  void ARMFunc<T,U>::armSequenceThread(void)
  {       
    T d1,d2,d3,result,flag;

    while(true)
    {
      
      unsigned int slaveID = 0x0;   // chose the slave ID
      
      write_o->write(tlm::TLM_IGNORE_COMMAND); // set the bus inactive
      
      
      cout<<endl;
      cout<<endl;
      cout<<endl;
      cout<<endl;
      
      wait(1,SC_NS);
      
      cout << "------------------------------------------------------------------------- FIRST OPERATION --------------------------------------------------------------------------------------" << endl;
      
      wait(cpuSleep); // wait for wake-up
       
      /*-----------------------------------------------------------------------
      -- Write Section
       ----------------------------------------------------------------------*/
       
      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Write section Operand Data1-----------................" << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x0;  // Slave ID 0
      
      d1 = (T)0x80000001; // data for operand 2

      armWrAndPrint((slaveID << 28), d1); // calling write command to write operand 1 on slave ID0
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      
      cout << "------Write separator Operand Data1-------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      
      cout << "... writes to Operand Data2-----------------------...." << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      
      slaveID = 0x1; // Slave ID 1
      
      d2 = (T)0x80000002; // data for operand 1

      armWrAndPrint((slaveID << 28), d2); // calling write command to write operand 2 on slave ID 1
      
      cout<<endl;
      
      cout << "------Write separator Operand Data2-------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      wait(12,SC_NS);
      cout<<endl;
      
      
      cout << "... writes to Opcode------------------------------...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x2; // Slave ID 2 
        
      d3 = (T)0X00000006; // data for Opcode 
      
      cout << "------------Op code = "<<d3<<" ---------------------" << endl; // Print op_code	
      
      cout<<endl;
      
      armWrAndPrint((slaveID << 28), d3);
      
      cout<<endl;
      
      wait(2,SC_NS);
       
       
      cout << "------Write separator Opcode(Control Signal)----------" << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      cout<<endl;


      cout << "------------------------------------------------------" << endl;
      cout << "Read section ...." << endl;
      cout << "------------------------------------------------------" << endl;
      
	
      cout << "------------------------------------------------------" << endl;
      cout << "... reads from Result  ...." << endl;
      cout << "------------------------------------------------------" << endl;


      cout<<endl;
      
      slaveID = 0x3;  // Slave ID 3

      result = armRdAndPrint( (slaveID << 28) ); // read ALU result
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      
      cout << "Result in CPU Funcc: "<<result<<endl;
      cout << "------Read separator (Result)-------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
           
           
      cout<<endl;
      cout<<endl;
      
      
      cout << "... reads from Flag ------------------------------" << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x4; // chose the slave ID, Register
 
      flagPrints(flag = armRdAndPrint( (slaveID << 28))); // read ALU 

      cout<<endl;

      wait(2,SC_NS);
      
      
      cout << "... Flag in CPU FUNC...." <<flag<<"........................"<< endl;
      cout << "------Read separator (Flag )------------------" << endl;


      cout<<endl;
      cout<<endl;
      cout<<endl;
      cout<<endl;
      
 
      cout << "------------------------------------------------------------------------ SECOND OPERATION --------------------------------------------------------------------------------------" << endl;

      wait(cpuSleep);
       
      /*-----------------------------------------------------------------------
      -- Write Section
       ----------------------------------------------------------------------*/
       
      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Write section Operand Data1-----------................" << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x0; // Slave ID 0
      
      d1 = (T)0x80000004; // data for operand 1

      armWrAndPrint((slaveID << 28), d1); // calling write command to write operand 1 on slave ID 0
      
      cout<<endl;
      
      wait(2,SC_NS);
        
        
      cout << "------Write separator Operand Data1-------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      cout<<endl;
      cout<<endl;
	
	
      cout << "... writes to Operand Data2-----------------------...." << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      
      slaveID = 0x1; // Slave ID 1
	
      d2 = (T)0x80000003; // data for operand 2
      
      armWrAndPrint((slaveID << 28), d2); // calling write command to write operand 2 on slave ID 1
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      
      cout << "------Write separator Operand Data2-------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      wait(12,SC_NS);
      cout<<endl;
      
      
      cout << "... writes to Opcode------------------------------...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x2; // Slave ID 2 
      
      d3 = (T)0X00000001; // data for Opcode   
      
      cout << "------------Op code = "<<d3<<" ---------------------" << endl; // Print data3
      
      cout<<endl;
      
      armWrAndPrint((slaveID << 28), d3); // calling write command to write op code on slave ID 2
      
      cout<<endl;
      
      wait(2,SC_NS);
       
       
      cout << "------Write separator Opcode(Control Signal)----------" << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      cout<<endl;


      cout << "------------------------------------------------------" << endl;
      cout << "Read section ...." << endl;
      cout << "------------------------------------------------------" << endl;


      cout << "------------------------------------------------------" << endl;
      cout << "... reads from Result  ...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x3; // Slave ID 3

      result = armRdAndPrint( (slaveID << 28) ); // read ALU result
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      
      cout << "Result in CPU Funcc: "<<result<<endl;
      cout << "------Read separator (Result)-------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
           
           
      cout<<endl;
      cout<<endl;
      
      
      cout << "... reads from Flag ------------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      
      slaveID = 0x4; // Slave ID 4
      
      flagPrints(flag = armRdAndPrint( (slaveID << 28))); // read ALU flag 
      
      cout<<endl;

      wait(2,SC_NS);
      
      
      cout << "------Read separator (Flag )------------------" << endl;


      cout<<endl;
      cout<<endl;
      cout<<endl;
      cout<<endl;
      cout<<endl;
      
      
      cout << "------------------------------------------------------------------------------- THIRD OPERATION --------------------------------------------------------------------------------" << endl;
      
       wait(cpuSleep);
       
      /*-----------------------------------------------------------------------
      -- Write Section
       ----------------------------------------------------------------------*/
       
      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Write section Operand Data1-----------................" << endl;
      cout << "------------------------------------------------------" << endl;

     
      cout<<endl;
     
      slaveID = 0x0; // Slave ID 0
      
      d1 = (T)0x80000002; // data for operand 1
      
      armWrAndPrint((slaveID << 28), d1); // calling write command to write operand 1 on slave ID 0
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      cout << "------Write separator Operand Data1 -------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      
      
      cout << "... writes to Operand Data2-----------------------...." << endl;
      cout << "------------------------------------------------------" << endl;
          
      
      cout<<endl;
      
      slaveID = 0x1; // Slave ID 1	
      
      d2 = (T)0x80000003; // data for operand 1
      
      armWrAndPrint((slaveID << 28), d2); // calling write command to write operand 2 on slave ID 1
      
      //wait(2,SC_NS);
      
      wait(12,SC_NS);
      
      cout<<endl;
      
      cout << "------Write separator Operand Data2 -------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      
      
      cout << "... writes to Opcode------------------------------...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x2; // Slave ID 2 
       
      d3 = (T)0X00000100; // data for Opcode  
      
      cout << "------------Op code = "<<d3<<" ---------------------" << endl; // Print data3
      
      armWrAndPrint((slaveID << 28), d3); // calling write command to write op code on slave ID 1
      
      cout<<endl;
      
      wait(2,SC_NS);
       
       
      cout << "------Write separator Opcode(Control Signal)----------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      cout<<endl;


      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Read section ...." << endl;
      cout << "------------------------------------------------------" << endl;
	
	
      cout << "------------------------------------------------------" << endl;
      cout << "... reads from result  ...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x3; // Slave ID 3
      
      result = armRdAndPrint( (slaveID << 28) ); // read ALU result
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      cout << "Result in CPU Funcc: "<<result<<endl;     
      
      cout<<endl;
      
      
      cout << "------Write separator (Result) -------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      
      
      cout << "... reads from flag ------------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      
      slaveID = 0x4; // chose the slave ID, Register

      flagPrints(flag = armRdAndPrint( (slaveID << 28))); // read ALU flag
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      
      cout << "------Read separator (Flag )------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      cout<<endl;
      cout<<endl;
      cout<<endl;
      
      
       cout << "---------------------------------------------------------------------------- FOURTH OPERATION ---------------------------------------------------------------------------------" << endl;
      
       wait(cpuSleep);
       
      /*-----------------------------------------------------------------------
      -- Write Section
       ----------------------------------------------------------------------*/
       
      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Write section Operand Data1-----------................" << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x0; // Slave ID 0 
     
      d1 = (T)0x00000003;  // data for operand 1
      
      armWrAndPrint((slaveID << 28), d1); // calling write command to write operand 1 on slave ID 0
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      
      cout << "------Write separator Operand Data1 -------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      
      
      cout << "... writes to Operand Data2-----------------------...." << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      
      slaveID = 0x1; // Slave ID 1	
      
      d2 = (T)0x000000003; // data for operand 1
      
      armWrAndPrint((slaveID << 28), d2); // calling write command to write operand 2 on slave ID 1
      
      wait(2,SC_NS);
      
      wait(12,SC_NS);
      
      cout<<endl;
      
      
      cout << "------Write separator Operand Data2 -------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      
      
      cout << "... writes to Opcode------------------------------...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x2; // Slave ID 2 
        
      d3 = (T)0X00000100; // data for Opcode 
      
      cout << "------------Op code = "<<d3<<" ---------------------" << endl; // Print data3
      
      armWrAndPrint((slaveID << 28), d3);
      
      cout<<endl;
      
      wait(2,SC_NS);
       
       
      cout << "------Write separator Opcode(Control Signal)----------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      cout<<endl;


      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Read section ...." << endl;
      cout << "------------------------------------------------------" << endl;
	
	
      cout << "------------------------------------------------------" << endl;
      cout << "... reads from result  ...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x3; // Slave ID 3
      
      result = armRdAndPrint( (slaveID << 28) ); // read ALU result
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      cout << "Result in CPU Funcc: "<<result<<endl;    
      
      cout<<endl;
      
      cout << "------Write separator Opcode (Result) ----------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      
      
      cout << "... reads from flag ------------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      
      slaveID = 0x4; // chose the slave ID, Register

      flagPrints(flag = armRdAndPrint( (slaveID << 28))); // read flag 
      
      cout<<endl;

      wait(2,SC_NS);
      
      
      cout << "------Read separator (Flag )------------------" << endl;
      
      cout<<endl;
      cout<<endl;
      cout<<endl;
      cout<<endl;
      cout<<endl;
      
      
      cout << "---------------------------------------------------------------------------- FIFTH OPERATION -----------------------------------------------------------------------------------" << endl;
      
       wait(cpuSleep);
       
      /*-----------------------------------------------------------------------
      -- Write Section
       ----------------------------------------------------------------------*/
       
      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Write section Operand Data1-----------................" << endl;
      cout << "------------------------------------------------------" << endl;


      cout<<endl;
      
      slaveID = 0x0;  // Slave ID 0
      
      d1 = (T)0x80000002; // data for operand 1
      
      armWrAndPrint((slaveID << 28), d1); // calling write command to write operand 1 on slave ID 0
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      
      cout << "------Write separator Operand Data1 -------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      
      
      cout << "... writes to Operand Data2-----------------------...." << endl;
      cout << "------------------------------------------------------" << endl;


      cout<<endl;
      
      slaveID = 0x1;  // Slave ID 1
      
      d2 = (T)0x80000003; // data for operand 1
      
      armWrAndPrint((slaveID << 28), d2); // calling write command to write operand 2 on slave ID 1
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      wait(12,SC_NS);
      
      
      cout << "... writes to Opcode------------------------------...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x2; // Slave ID 2 
       
      d3 = (T)0X00000000; // data for Opcode  
      
      cout << "------------Op code = "<<d3<<" ---------------------" << endl; // Print data3
      
      armWrAndPrint((slaveID << 28), d3); // calling write command to write op code on slave ID 2
      
      cout<<endl;
      
      wait(2,SC_NS);
       
       
      cout << "------Write separator Opcode(Control Signal)----------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      cout<<endl;


      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Read section ...." << endl;
      cout << "------------------------------------------------------" << endl;
	
      cout << "------------------------------------------------------" << endl;
      cout << "... reads from result  ...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      cout<<endl;
      
      slaveID = 0x3; // Slave ID 3
      
      result = armRdAndPrint( (slaveID << 28) ); // read ALU result
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      cout << "Result in CPU Funcc: "<<result<<endl;    
       
      cout<<endl;
      
      cout << "------Write separator Opcode(Control Signal)----------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout<<endl;
      cout<<endl;
      
      
      cout << "... reads from flag ------------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      cout<<endl;
      
      slaveID = 0x4; // chose the slave ID, Register

      flagPrints(flag = armRdAndPrint( (slaveID << 28))); // read ALU flag 
      
      cout<<endl;
      
      wait(2,SC_NS);
      
      
      cout << "------Read separator (Flag )------------------" << endl;
      
      
      cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
      cout << "xx End of simulation! " << endl;
      cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
      
      sc_stop();
    }
    
}

template<class T, class U>
void ARMFunc<T,U>::armWr1Print(unsigned int addr, T data)
{
  cout << left  << setw(11)  << "ARM "
       << left  << setw(20) << "- writes: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", byteAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}

template<class T, class U>
void ARMFunc<T,U>::armWrCmd(unsigned int addr, T data)
{
  //! Writes the generated data to the BPI - to send it to the slave.
  data_o->write(data);
  //! Write the corresponding address.
  address_o->write(addr);
  //! send the WRITE command to the BPI.
  write_o->write(tlm::TLM_WRITE_COMMAND);
  //! Wait for the write delay.
  wait(rwAccess_t + rwWriteDelay_t);
  //! Remove the WRITE command.
  write_o->write(tlm::TLM_IGNORE_COMMAND);
}

template<class T, class U>
void ARMFunc<T,U>::armWrAndPrint(unsigned int addr, T data)
{
  if(CPUPRINTS)
    armWr1Print(addr, data);
  armWrCmd(addr, data);
  wait(cpuOp2Op);
}

template<class T, class U>
T ARMFunc<T,U>::armRdCmd(unsigned int addr)
{
  // write the address to the BPI for the data to be read
  address_o->write(addr);
  // send the READ command to the BPI
  write_o->write(tlm::TLM_READ_COMMAND);
  // wait for data from BPI - Bus - Memory
  wait(readMemRdy_e);
  // wait for the read delay
  wait(rwAccess_t + rwReadDelay_t);
  // remove the READ command
  write_o->write(tlm::TLM_IGNORE_COMMAND);
  // return the read data to the algorithm
  return dataIn_s.read();
}

template<class T, class U>
T ARMFunc<T,U>::armRdAndPrint(unsigned int addr)
{
  T data;

  data = armRdCmd(addr);
  if(CPUPRINTS)
    armRd1Print(addr, data);
  wait(cpuOp2Op);
  
  return data;
}

template<class T, class U>
void ARMFunc<T,U>::armRd1Print(unsigned int addr, T data)
{
  cout << left  << setw(11) << "ARM "
       << left  << setw(20) << "- reads: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", byteAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data << " (" << dec << data << ")"
       << endl;
}

template<class T, class U>
void ARMFunc<T,U>::flagPrints(unsigned int flag)
{

switch(flag)
{
        case 0:
              cout << endl << "....No Flag Set...." <<flag<< endl;
              break;
        case 1:
              cout << endl << "....Zero Flag Set...." <<flag<< endl;
              break;
        case 2:
              cout << endl << "....Carry Flag Set...." <<flag<< endl;
              break;
        case 3:
              cout << endl << "....Negative Flag Set...." <<flag<< endl;
              break;
          default:
             break;
        
} 
}

/*-----------------------------------------------------------------------------
-- Not needed things, just printouts
-----------------------------------------------------------------------------*/
template<class T, class U>
void ARMFunc<T,U>::armFuncPrint01(T data)
{
  cout << left  << setw(11) << "Master-Func"
       << left  << setw(20) << "- Data read: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(19) << ""
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}
