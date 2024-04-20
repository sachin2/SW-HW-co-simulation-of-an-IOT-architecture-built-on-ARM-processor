#include "DataRegTop.h"
#include "DataRegFunc.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
DataRegTop<T>::DataRegTop(sc_module_name nm
, const unsigned int   ID                   // target ID
, const char           *memorySocket        // socket name
, uint64               memorySize           // memory size (bytes)
, unsigned int         memoryWidth          // memory width (bytes)
, unsigned int         baseAddress
, unsigned int         regReset
, const sc_time        acceptDelay          // accept delay
, const sc_time        readResponseDelay    // read response delay
, const sc_time        writeResponseDelay   // write response delay
)
  : sc_module(nm)
, drID                 (ID)                 // init target ID
, drMemorySize         (memorySize)         // e.g., 1024 lines = 4*1024
, drMemoryWidth        (memoryWidth)        // e.g., 4 bytes per line
, drBaseAddress        (baseAddress)
, drRegReset           (regReset)
, drAcceptDelay        (acceptDelay)        // init accept delay
, drReadResponseDelay  (readResponseDelay)  // init read response delay
, drWriteResponseDelay (writeResponseDelay) // init write response delay 
{
  //SC_Hdr_PROCESS(drRegisterTop);
  cout << left  << setw(30) << "ConstructorCPP - DataRegTop:    "
       << left  << setw(36) << name()
       << left  << setw(15) << " with the ID: "
       << right << setw(5)  << dec << drID
       << endl;

  //! Initialization: Slave BPI.
  mbpiDR = new DataRegBPI<T>("DebuggerTAP-DataRegister-BPI",drID,
			  memorySocket,drMemorySize,
			  drMemoryWidth,
			  drAcceptDelay,drReadResponseDelay,
			  drWriteResponseDelay,drDataReadEvent_e);
  //! Initialization: Slave function.
  mfunDR = new DataRegFunc<T>("DebuggerTAP-DataRegister-Func",
			       drMemorySize,drBaseAddress,drRegReset,
			       drDataReadEvent_e);

  //! Binding of the sc_export's.
  mbpiDR->write_o(mfunDR->write_s);
  mbpiDR->address_o(mfunDR->address_s);
  mbpiDR->tdi_o(mfunDR->tdi_s);
  mfunDR->tdo_o(mbpiDR->tdo_s);
}
