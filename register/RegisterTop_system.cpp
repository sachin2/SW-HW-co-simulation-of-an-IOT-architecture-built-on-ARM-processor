#include "RegisterTop_system.h"
#include "RegisterFunc.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
RegisterTop<T>::RegisterTop(sc_module_name nm
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
, ID                 (ID)                 // init target ID
, MemorySize         (memorySize)         // e.g., 1024 lines = 4*1024
, MemoryWidth        (memoryWidth)        // e.g., 4 bytes per line
, BaseAddress        (baseAddress)
, RegReset           (regReset)
, AcceptDelay        (acceptDelay)        // init accept delay
, ReadResponseDelay  (readResponseDelay)  // init read response delay
, WriteResponseDelay (writeResponseDelay) // init write response delay 
{
  //SC_HAS_PROCESS(RegisterTop);
  cout << left  << setw(30) << "ConstructorCPP - RegisterTop:    "
       << left  << setw(36) << name()
       << left  << setw(15) << " with the ID: "
       << right << setw(5)  << dec << ID
       << left  << setw(8)  << ", Size: "
       << right << setw(5)  << dec << MemorySize
       << left  << setw(15) << " bytes, Width: "
       << right << setw(5)  << dec << MemoryWidth
       << " bytes"
       << left  << setw(16) << ", Reg. Address: "
       << right << setw(12)  << hex << BaseAddress
       << left  << setw(16) << ", Reg. Reset: "
       << right << setw(12)  << hex << RegReset
       << endl;

  //! Initialization: Slave BPI.
  mbpi = new SystemRegisterBPI<T>("Register-BPI",ID,
			  memorySocket,MemorySize,
			  MemoryWidth,
			  AcceptDelay,ReadResponseDelay,
			  WriteResponseDelay,slaveDataReadEvent_e);
  //! Initialization: Slave function.
  mfun = new RegisterFunc<T>("Register-Func",
			       MemorySize,BaseAddress,RegReset,
			       slaveDataReadEvent_e);

  //! Binding of the sc_export's.
  mbpi->write_o(mfun->write_s);
  mbpi->address_o(mfun->address_s);
  mbpi->data_o(mfun->dataIn_s);
  mfun->data_o(mbpi->dataRd_s);
  
  
  //! Connection of the ports.
  mfun->gpio_o(gpio_o);
  mfun->gpio_i(gpio_i);
  mfun->gpio_dir_i(gpio_dir_i);
}
