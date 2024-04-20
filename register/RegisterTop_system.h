#ifndef _REGISTERTOP_H_
#define _REGISTERTOP_H_

#include "ProcPack.h"
#include <systemc.h>
//#include "../bus_system/SystemRegisterBPI.h"
#include "RegisterFunc.h"

template<class T>
//SC_MODULE(RegisterTop)
class RegisterTop : public sc_module
{
  public:
    /*!------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    //! GPIO Input
    sc_in<T> gpio_i;
    //! GPIO Outout
    sc_out<T> gpio_o;
    //! GPIO Control: 0 for output, 1 for input
    sc_in<bool> gpio_dir_i;
    
    /*!------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(RegisterTop);
    RegisterTop(sc_module_name nm
      , const unsigned int      ID                 //!< target ID
      , const char              *memorySocket      //!< socket name
      , uint64                  memorySize         //!< memory size (bytes)
      , unsigned int            memoryWidth        //!< memory width(bytes)
      , unsigned int            baseAddress        //!< base address of the regs.
      , unsigned int            regReset           //!< Reset value
      , const sc_time           acceptDelay        //!< accept delay
      , const sc_time           readResponseDelay  //!< read response delay
      , const sc_time           writeResponseDelay //!< write response delay
      );
  
    
    //! Declaration: Slave BPI.
    SystemRegisterBPI<T>*      mbpi;
  
  private:
   
    sc_event slaveDataReadEvent_e;
  
   
    /*!------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    const unsigned int ID;                   //!< target ID
    uint64             MemorySize;           //!< memory size (bytes)
    unsigned int       MemoryWidth;          //!< word size (bytes)
    unsigned int       BaseAddress;          //!< base address of the register
    unsigned int       RegReset;             //!< 
    const sc_time      AcceptDelay;          //!< accept delay
    const sc_time      ReadResponseDelay;    //!< read response delay
    const sc_time      WriteResponseDelay;   //!< write response delays
  
    
    RegisterFunc<T>*      mfun;
};
#endif
