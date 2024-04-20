
#ifndef _REGISTERFUNC_H_
#define _REGISTERFUNC_H_

#include "ProcPack.h"
#include "systemc.h"
#include <iomanip>

template<class T> //! The template specifies the data type: unsigned int or char
//SC_MODULE(RegisterFunc)
class RegisterFunc : public sc_module
{
  public:
    /*------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    //! Write/read cmd.
    sc_export<sc_signal_in_if<unsigned int> >  write_i;
    //! Bus address.
    sc_export<sc_signal_in_if<unsigned int> >  address_i;
    //! Write data.
    sc_export<sc_signal_in_if<T> >  data_i;
    //! Read data.
    sc_out<T>                       data_o;
    //! GPIO input.
    sc_in<T>                        gpio_i;
    //! GPIO output.
    sc_out<T>                       gpio_o;
    //! GPIO control: 0 for output, 1 for input
    sc_in<bool> gpio_dir_i;
  
    /*------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    // sc_export connected signals.
    //! Bind to write_i.
    sc_signal<unsigned int> write_s;
    //! Bind to address_i.
    sc_signal<unsigned int> address_s;
    //! Bind to data_i.
    sc_signal<T>            dataIn_s;
  
    
    RegisterFunc(sc_module_name nm,
		   uint64 memorySize,
		   unsigned int baseAddress,
		   unsigned int regReset,
		   sc_event& sFuncDatRdEv);
  
  private:
    /*------------------------------------------------------------------------
    -- FIFOs/Signals/Registers
    -------------------------------------------------------------------------*/
     //! The register.
    T register01_s;
  
    /*------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    //! The event, which indicates that something happened on the bus.
    sc_event gotCmd_e;
  
    /*------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    void getCmdThread(void);
    void putDatThread(void);
    void wrtDatThread(void);
    void registerFuncPrint01(unsigned int cmd);
    void registerFuncPrint02(unsigned int addr, T data);
    void registerFuncPrint03(unsigned int addr, T data);
  
    /*------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    //! The size of the memory in bytes (taken from the constructor).
    uint64  MemorySize_v;
    //! The base address ot the register (taken from the constructor).
    unsigned int BaseAddress_v;
    //! The reset value of the register (taken from the constructor).
    unsigned int RegReset_v;
    /*! The event fired when the data to be read will be available in the
    functional block. The BPI waits for it to forward the data to the bus.*/
    sc_event& sFuncDatRdEv_e;
};
#endif
