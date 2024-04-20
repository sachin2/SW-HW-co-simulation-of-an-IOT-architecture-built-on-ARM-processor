#ifndef _DATAREGFUNC_H_
#define _DATAREGFUNC_H_

#include "../asProcPack.h"
#include "systemc.h"
#include <iomanip>

template<class T> //! The template specifies the data type: unsigned int or char
//SC_MODULE(InstRegFunc)
class DataRegFunc : public sc_module
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
    sc_export<sc_signal_in_if<T> >  	       tdi_i;
    sc_out<T>                      	    	tdo_o;
  
    /*------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    // sc_export connected signals.
    //! Bind to write_i.
    sc_signal<unsigned int> write_s;
    //! Bind to address_i.
    sc_signal<unsigned int> address_s;
	sc_signal<T>   			tdi_s;
  
    /*------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    DataRegFunc(sc_module_name nm,
				   uint64 memorySize,
				   unsigned int baseAddress,
				   unsigned int regReset,
				   sc_event& sFuncDatRdEv);
  
  private:
    /*------------------------------------------------------------------------
    -- FIFOs/Signals/Registers
    -------------------------------------------------------------------------*/
     //! The register.
    T DataRegister_s;
  
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
    uint64  drMemorySize_v;
    unsigned int drBaseAddress_v;
    unsigned int drRegReset_v;
    sc_event& sFuncDatRdEv_e;
};
#endif
