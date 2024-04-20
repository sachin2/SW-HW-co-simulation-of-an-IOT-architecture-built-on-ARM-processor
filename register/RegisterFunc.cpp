#include "RegisterFunc.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
RegisterFunc<T>::RegisterFunc(sc_module_name nm
		            , uint64       memorySize
			    , unsigned int baseAddress
			    , unsigned int regReset
			    , sc_event&    sFuncDatRdEv)
  : sc_module        (nm)
  , MemorySize_v   (memorySize)
  , BaseAddress_v  (baseAddress)
  , RegReset_v     (regReset)
  , sFuncDatRdEv_e   (sFuncDatRdEv)
{
  //! SC_HAS_PROCESS is needed here because SC_CTOR() is not used.
  SC_HAS_PROCESS(RegisterFunc);
  
  cout << left  << setw(30) <<  "ConstructorCPP - RegisterFunc: "
       << left  << setw(56) << name()
       << left  << setw(8)  << ", Size: "
       << right << setw(5)  << dec << MemorySize_v
       << endl;

  //! Connection of the thread getCmdThread to the simulation kernel.
  SC_THREAD(getCmdThread);
  //! Connection of the thread putDatThread to the simulation kernel.
  SC_THREAD(putDatThread);
  //! Connection of the thread wrtDatThread to the simulation kernel.
  SC_THREAD(wrtDatThread);
  
  //! Binding of the sc_signal write_s to the sc_export write_i.
  write_i.bind(write_s);
  //! Binding of the sc_signal address_s to the sc_export address_i.
  address_i.bind(address_s);
  //! Binding of the sc_signal dataIn_s to the sc_export data_i.
  data_i.bind(dataIn_s);

  //! Reset of register register01_s.
  register01_s = RegReset_v;
}

template<class T>
void RegisterFunc<T>::getCmdThread(void)
{
  unsigned int v;
  while(true)
  {
    //! Wait on a transaction on the bus or at the GPIOs.
    wait( (write_s.value_changed_event()) | (gpio_dir_i.value_changed_event())
	                                  | (gpio_i.value_changed_event()) );
    
    // This is only needed for possible printouts.
    v = write_s.read();
    
    if(SLAVEFUNPRINTS)
      registerFuncPrint01(v);

    //! Fire the event when a transaction on the bus or at the GPIOs happened.
    gotCmd_e.notify(SC_ZERO_TIME);
  }//while
}


template<class T>
void RegisterFunc<T>::wrtDatThread(void)
{
  while(true)
  {
    //! If the GPIO is programmed beeing an input, the register will be written directly.
    if( gpio_dir_i.read() == 1) // The GPIO is input.
      register01_s = gpio_i.read();
    
    //! Wait on a bus/GPIO transaction.
    wait(gotCmd_e);
    
    //! If the GPIO is programmed beeing an input, the register will be written directly.
    if( gpio_dir_i.read() == 1) // The GPIO is input.
      register01_s = gpio_i.read();
    
    //! If the GPIO is programmed beeing an output, the content of the register
    //  will be witten to the GPIO.
    if( gpio_dir_i.read() == 0) // The GPIO is output.
      gpio_o.write(register01_s);
    
    //! Read the cmd and see if it is "WRITE". It is a write from the bus.
    if(write_s.read() == 1)
    {
      if(SLAVEFUNPRINTS)
	registerFuncPrint02(address_s.read(), dataIn_s.read());

      //! Write the data from the bus to the register.
      if(address_s.read()==BaseAddress_v/REGIWIDTH) // word address
      {
        register01_s = dataIn_s.read();
	//cout << "Register write: address = " << BaseAddress_v << " data = " << register01_s << endl;
      }
    }
  }//while
}


template<class T>
void RegisterFunc<T>::putDatThread(void)
{
  while(true)
  {
    //! If the GPIO is programmed beeing an input, the register will be written directly.
    if( gpio_dir_i.read() == 1) //! The GPIO is input.
      register01_s = gpio_i.read();
    
    //! Wait on a bus/GPIO transaction.
    wait(gotCmd_e);

    //! If the GPIO is programmed beeing an input, the register will be written directly.
    if( gpio_dir_i.read() == 1) //! The GPIO is input.
      register01_s = gpio_i.read();
    
    //! Read the cmd and see if it is "READ". It is a read from the bus.
    if(write_s.read() == 0)
    {
      if(SLAVEFUNPRINTS)
	registerFuncPrint03(address_s.read(), register01_s);

      //! Write the data, read from the register, to the BPI.
      if(address_s.read()==BaseAddress_v/REGIWIDTH)
      {
        data_o->write(register01_s);
	//cout << "Register read: address = " << BaseAddress_v << " data = " << register01_s << endl;
      }

      //! Give a notice to the BPI that the read data is ready.
      sFuncDatRdEv_e.notify(SC_ZERO_TIME);
    }
  }//while
}

/*----------------------------------------------------------------------------
-- Helper functions
-----------------------------------------------------------------------------*/
// prints
template<class T>
void RegisterFunc<T>::registerFuncPrint01(unsigned int cmd)
{
  cout << left  << setw(11) << "Slave-Func "
       << left  << setw(20) << "- Got R/W "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ""
       << right << hex << setw(9) << ""
       << left  << setw(7) << ""
       << right << hex << setw(9) << ""
       << left  << setw(11) << ", cmd "
       << dec << setw(4) << cmd
       << endl;
}

template<class T>
void RegisterFunc<T>::registerFuncPrint02(unsigned int addr, T data)
{
  cout << left  << setw(11) << "Slave-Func "
       << left  << setw(20) << "- write it: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", wordAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}

template<class T>
void RegisterFunc<T>::registerFuncPrint03(unsigned int addr, T data)
{
  cout << left  << setw(11) << "Slave-Func "
       << left  << setw(20) << "- read it: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", wordAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}
