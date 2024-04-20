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
/*! \brief Reads and writes to registers.

writes dummy test data to the register with the ID=1 and writes dummy test data to the switch register 
with the ID=2. Then it reads the switch register and writes the values to the 
memory and finally the memory is read again.
@param[in]              (void)         Nothing
@param[out]             (void)         Nothing
@param dataIndex_v      (T)            The data which will written or read.
@param addr_v           (unsigned int) The address for reading or writing.
@param slaveID          (unsigned int) The ID of the slave. It is the upper 
                                       nibble of the address.
@param[out] write_o     (unsigned int) It defines the data direction: read or 
                                       write.
@param currentAddress_v (unsigned int) The address for reading or writing.
 */
template<class T, class U>
  void ARMFunc<T,U>::armSequenceThread(void)
  {       
    sc_uint<32> result;
    sc_uint<3> flag;

    while(true)
    {
      // chose the slave ID
         unsigned int slaveID = 0x0;   
      // set the bus inactive
         write_o->write(tlm::TLM_IGNORE_COMMAND);
      // wait for wake-up
        wait(cpuSleep);
      /*-----------------------------------------------------------------------
      -- Write Section
       ----------------------------------------------------------------------*/
      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Write section Operand Data1-----------................" << endl;
      cout << "------------------------------------------------------" << endl;

      // Slave ID 0
      slaveID = 0x0;   
      // data for operand 1
      sc_uint<32> data1 = data_op1_i.read();
      // calling write command to write operand 1 on slave ID 0
      armWrAndPrint((slaveID << 28), data1);
        
      cout << "------Write separator Operand Data1-------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout << "... writes to Operand Data2-----------------------...." << endl;
      cout << "------------------------------------------------------" << endl;

      
      // Slave ID 1
      slaveID = 0x1;
      // data for operand 1
      sc_uint<32> data2 = data_op2_i.read();
      // calling write command to write operand 2 on slave ID 1
      armWrAndPrint((slaveID << 28), data2); 
        
      cout << "------Write separator Operand Data2-------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      wait(1,SC_NS);
      cout << "... writes to Opcode------------------------------...." << endl;
      cout << "------------------------------------------------------" << endl;
      
      // Slave ID 2 
      slaveID = 0x2; 
      // data for Opcode   
      sc_uint<32> op_code = op_code_i.read(); 
      // write data3
      cout << "------------Op code "<<op_code<<"---------------------" << endl;
      unsigned int controlSignal = op_code;
      armWrAndPrint((slaveID << 28), controlSignal);
      
      cout << "------Write separator Opcode(Control Signal)----------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      
      cout << "------------------------------------------------------" << endl;
      
      // let the CPU sleep for a while
      wait(cpuSleep);
                   
      /*-----------------------------------------------------------------------
      -- Read Section
      ----------------------------------------------------------------------*/

      cout << " " << endl;
      cout << "------------------------------------------------------" << endl;
      cout << "Read section ...." << endl;
      cout << "------------------------------------------------------" << endl;

      cout << "------------------------------------------------------" << endl;
      cout << "... reads from Result  ...." << endl;
      cout << "------------------------------------------------------" << endl;

      // Result read
      // Slave ID 3
      slaveID = 0x3; 
      //writing a result from ALU to this reg
      //armWrAndPrint(( slaveID << 28));
      //wait(cpuSleep);
      result = armRdAndPrint( (slaveID << 28) ); // read ALU result
      cout << "Result in CPU Funcc: "<<result<<endl;
      cout << "------Read separator (Result)-------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      
      //wait(50,SC_NS);
      
      cout << "... reads from Flag ....------------------------------" << endl;
      cout << "------------------------------------------------------" << endl;
      // Flag read; 
      slaveID = 0x4; // chose the slave ID, Register
      //armWrAndPrint((slaveID << 28));
      flag = armRdAndPrint( (slaveID << 28) ); // read flag 
      switch(flag)
      {
        case ZERO:
              cout << "..1. Zero Flag...." <<flag<<"........................"<< endl;
              break;
        case CARRYIN:
              cout << "..2. Carry In....." <<flag<<"........................"<< endl;
              break;
        case CARRYOUT:
              cout << "..3. Carry Out...." <<flag<<"........................"<< endl;
              break;
        case CARRYOVERFLOW:
              cout << "..4. Carry Overflow...." <<flag<<"..................."<< endl;
              break;
        case NEGATIVE:
              cout << "..5. Negative.........." <<flag<<"..................."<< endl;
              break;
          default:
             break;
        
      } 
      cout << "... Flag in CPU FUNC...." <<flag<<"........................"<< endl;
      cout << "------Read separator (Flag )------------------" << endl;


      wait(100, SC_NS);

      cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
      cout << "xx End of simulation! " << endl;
      cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
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