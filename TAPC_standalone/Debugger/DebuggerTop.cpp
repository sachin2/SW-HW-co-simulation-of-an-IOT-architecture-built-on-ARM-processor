#include "DebuggerTop.h"

//#include "stim.cpp"
#include "DebuggerFunc.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*!----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T, class U>
DebuggerTop<T,U>::DebuggerTop(sc_module_name nm , const unsigned int ID) : sc_module(nm) , ssID (ID)
{
  cout << left << setw(30) << "ConstructorCPP - DebuggerTop:    "
       << left << setw(36) << name()
       << left << setw(14) << " with the ID: "
       << left << setw(5)  << dec << ssID
       << endl;
  
  //! Initialization: Master BPI.
  mbpi = new DebuggerBPI<T>("Debugger-BPI", ssID, masterDataReadEvent_e);
  //! Initialization: Masters function.
  mdeb = new DebuggerFunc<T,U>("Debugger-Func", masterDataReadEvent_e);
  
  //mstim = new ssStim("StimulationModule");

  //! Binding of the inputs from stimulation.
  //mstim->tms(mdeb->tms_s);
  //! Binding of the sc_export's.
  mdeb->write_o(mbpi->write_s);
  mdeb->address_o(mbpi->address_s);
  mdeb->tdi_o(mbpi->tdi_s);
  mbpi->tdo_o(mdeb->tdo_s);
}

