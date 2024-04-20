#ifndef _DATAREGTOP_H_
#define _DATAREGTOP_H_

#include "../asProcPack.h"
#include <systemc.h>
#include "DataRegFunc.h"

template<class T>
class DataRegTop : public sc_module
{
  public:
	
    /*!------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
	DataRegTop(sc_module_name nm
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
    /*!------------------------------------------------------------------------
    -- Hierarchy: Public in case of needed traces.
    -------------------------------------------------------------------------*/
    //! Declaration: Slave BPI.
    DataRegBPI<T>*      mbpiDR;
  
  private:
    /*!------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    /*!------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    //! Declaration: The event is fired when the data to be read will be
    // available in the functional block. The BPI waits for it to forward the
    // data to the bus.
    sc_event drDataReadEvent_e;
  
    /*!------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    /*!------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    const unsigned int drID;                   //!< target ID
    uint64             drMemorySize;           //!< memory size (bytes)
    unsigned int       drMemoryWidth;          //!< word size (bytes)
    unsigned int       drBaseAddress;          //!< base address of the register
    unsigned int       drRegReset;             //!< 
    const sc_time      drAcceptDelay;          //!< accept delay
    const sc_time      drReadResponseDelay;    //!< read response delay
    const sc_time      drWriteResponseDelay;   //!< write response delays
  
    /*!------------------------------------------------------------------------
    -- Hierarchy: Private, if no traces needed.
    -------------------------------------------------------------------------*/
    //! Declaration: Slaves function.
    DataRegFunc<T>*      mfunDR;
};
#endif
