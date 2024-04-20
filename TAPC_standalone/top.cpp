//! \brief Simple TLM bus model - the top module (cpp).
/*! A simple stub for a TLM bus model with one master and two slaves - the top module.
*/
#include "top.h"

#include "JtagBus/DataRegBPI.cpp"
#include "DR/DataRegTop.cpp"
#include "JtagBus/JtagBus.cpp"
#include "JtagBus/DebuggerBPI.cpp"
#include "Debugger/DebuggerTop.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*!----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T, class U>
top<T,U>::top(sc_module_name nm)
  : sc_module(nm)
  , ssJtagBus("DebuggerTAP-JTAG-Bus")
{
  cout << left << setw(30) << "ConstructorCPP - top:         "
       << left << setw(35) << name()
       << endl;

  // Construct the CPUs
  ssDebugger   = new DebuggerTop<T, U>("DebuggerTAP", 218);

  // Construct the periperals
  ssInstrReg   = new DataRegTop<T>("InstructionRegister", 210, "InstructionRegisterSocket1",
                              REG0BYTES, REGIWIDTH, REG0ADDRE, REG0RESET,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
  
  ssDataReg    = new DataRegTop<T>("DataRegister", 211, "DataRegisterSocket1",
                              REG1BYTES, REGIWIDTH, REG1ADDRE, REG1RESET,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);

  /*---------------------------------------------------------------------------
  -- Connect blocks
  ---------------------------------------------------------------------------*/
  // Connect the bus; bind TLM2 initiators to TLM2 target sockets on SimpleBus
  // CPU master socket to bus slave socket
  ssDebugger->mbpi->ssMaster1Socket(ssJtagBus.slaveSocket[0]);
  // Bus master socket to Instruction register slave socket
  ssJtagBus.masterSocket[0](ssInstrReg->mbpiDR->ssSlave2Socket);
  // Bus master socket to Data register slave socket
  ssJtagBus.masterSocket[1](ssDataReg->mbpiDR->ssSlave2Socket); 
}
