#include "DataRegFunc.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
DataRegFunc<T>::DataRegFunc(sc_module_name nm
		            , uint64       memorySize
			    , unsigned int baseAddress
			    , unsigned int regReset
			    , sc_event&    sFuncDatRdEv)
  : sc_module        (nm)
  , drMemorySize_v   (memorySize)
  , drBaseAddress_v  (baseAddress)
  , drRegReset_v     (regReset)
  , sFuncDatRdEv_e   (sFuncDatRdEv)
{
  //! SC_HAS_PROCESS is needed here because SC_CTOR() is not used.
  SC_HAS_PROCESS(DataRegFunc);
  
  cout << left  << setw(30) <<  "ConstructorCPP - DataRegFunc: "
       << left  << setw(56) << name()
       << endl;

  SC_THREAD(getCmdThread);
  SC_THREAD(putDatThread);
  SC_THREAD(wrtDatThread);
  
  //! Binding of the sc_signal write_s to the sc_export write_i.
  write_i.bind(write_s);
  //! Binding of the sc_signal address_s to the sc_export address_i.
  address_i.bind(address_s);
  //! Binding of the sc_signal dataIn_s to the sc_export data_i.
  tdi_i.bind(tdi_s);

  //! Reset of register register01_s.
  DataRegister_s = drRegReset_v;
}

template<class T>
void DataRegFunc<T>::getCmdThread(void)
{
  unsigned int v;
  while(true)
  {
    //! Wait on a transaction on the bus.
    wait(write_s.value_changed_event());

    // This is only needed for possible printouts.
    v = write_s.read();
    
	registerFuncPrint01(v);

    //! Fire the event when a transaction on the bus.
    gotCmd_e.notify(SC_ZERO_TIME);
  }//while
}

template<class T>
void DataRegFunc<T>::wrtDatThread(void)
{
  while(true)
  {
   wait(gotCmd_e);
	  //! Read the cmd and see if it is "WRITE". It is a write from the bus.
    if(write_s.read() == 1)
    {
		registerFuncPrint02(address_s.read(), tdi_s.read());

        //! Write the data from the bus to the register.
		DataRegister_s = tdi_s.read();
    }
  }//while
}

template<class T>
void DataRegFunc<T>::putDatThread(void)
{
  while(true)
  {
   wait(gotCmd_e);
     //! Read the cmd and see if it is "READ". It is a read from the bus.
    if(write_s.read() == 0)
    {
		registerFuncPrint03(address_s.read(), DataRegister_s);

        //! Write the data, read from the register, to the BPI.
        tdo_o ->write(DataRegister_s);
    }
      //! Give a notice to the BPI that the read data is ready.
      sFuncDatRdEv_e.notify(SC_ZERO_TIME);
  }//while
}

/*----------------------------------------------------------------------------
-- Helper functions
-----------------------------------------------------------------------------*/
// prints
template<class T>
void DataRegFunc<T>::registerFuncPrint01(unsigned int cmd)
{
  cout << left  << setw(11) << "Slave-Func "
       << left  << setw(20) << "- Got R/W "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(11) << ", cmd "
       << dec << setw(4) << cmd
       << endl;
}

template<class T>
void DataRegFunc<T>::registerFuncPrint02(unsigned int addr, T data)
{
  sc_bv<32> binVal(data);
  cout << left  << setw(11) << "Slave-Func "
       << left  << setw(20) << "- write it: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << right << setw(5)  << binVal.to_string(SC_BIN)
       << endl;
}

template<class T>
void DataRegFunc<T>::registerFuncPrint03(unsigned int addr, T data)
{
  sc_bv<32> binVal(data);
  cout << left  << setw(11) << "Slave-Func "
       << left  << setw(20) << "- read it: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << right << setw(10)  << binVal.to_string(SC_BIN)
       << endl;
}
