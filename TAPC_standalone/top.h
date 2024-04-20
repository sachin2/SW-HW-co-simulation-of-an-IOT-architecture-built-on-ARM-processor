//! \brief Simple TLM bus model - the top module (h).
/*! A simple stub for a TLM bus model with one master and two slaves - the top module.
*/
#ifndef _TOP_H_
#define _TOP_H_

#include <systemc.h>
#include "tlm.h"
#include "asProcPack.h"
#include <iomanip>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

#include "JtagBus/DataRegBPI.h"
#include "DR/DataRegTop.h"
#include "JtagBus/JtagBus.h"
#include "JtagBus/DebuggerBPI.h"
#include "Debugger/DebuggerTop.h"

template<class T, class U>
//SC_MODULE(top)
class top : public sc_module
{
  public:
    /*!-----------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/   
    /*!------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    SC_CTOR(top);
    
    /*!------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    /*!------------------------------------------------------------------------
    -- Hierarchy: Public in case of needed traces.
    -------------------------------------------------------------------------*/
    JtagBus<1, 2>          ssJtagBus;
    DebuggerTop<T, U>*     ssDebugger;
    DataRegTop<T>*    	   ssInstrReg;
    DataRegTop<T>*         ssDataReg;
  private:
    /*!------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    /*!------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    /*!------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/  
    /*!------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    /*!------------------------------------------------------------------------
    -- Hierarchy: Private, if no traces needed.
    -------------------------------------------------------------------------*/
};
#endif
