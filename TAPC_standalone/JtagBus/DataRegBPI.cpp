#include "DataRegBPI.h"

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
DataRegBPI<T>::DataRegBPI(sc_module_name nm
, const unsigned int   ID                   // target ID
, const char           *memorySocket        // socket name
, uint64               memorySize           // memory size (bytes)
, unsigned int         memoryWidth          // memory width (bytes)
, const sc_time        acceptDelay          // accept delay
, const sc_time        readResponseDelay    // read response delay
, const sc_time        writeResponseDelay   // write response delay
, sc_event&            sBPIDatRdEv
)
: sc_module            (nm)               // init module name
, ssSlave2Socket       (memorySocket)       // init socket name
, ssID                 (ID)                 // init target ID
, ssMemorySize         (memorySize)         // e.g., 1024 lines = 4*1024
, ssMemoryWidth        (memoryWidth)        // e.g., 4 bytes per line
, ssAcceptDelay        (acceptDelay)        // init accept delay
, ssReadResponseDelay  (readResponseDelay)  // init read response delay
, ssWriteResponseDelay (writeResponseDelay) // init write response delay
, sBPIDatRdEv_e (sBPIDatRdEv)
{
  SC_HAS_PROCESS(DataRegBPI);
  cout << left  << setw(30) << "ConstructorCPP - DataRegBPI: "
       << left  << setw(36) << name()
       << left  << setw(15) << " with the ID: "
       << right << setw(5)  << dec << ssID
       << " bytes"
       << endl;
  SC_THREAD(cmdThread);
  SC_THREAD(adrThread);
  SC_THREAD(dawThread);
  SC_THREAD(darThread);
  
  // binds the fifo to the export
  tdo_i.bind(tdo_s);

  ssSlave2Socket.register_b_transport(this, &DataRegBPI::b_transport);
}

template<class T>
void DataRegBPI<T>::b_transport(tlm::tlm_generic_payload &trans,sc_time &delay)
{
  // get the information from the payload
  tlm::tlm_command cmd = trans.get_command();
  sc_dt::uint64    adr = trans.get_address() / 4;
  unsigned char*   ptr = trans.get_data_ptr();
  unsigned int     len = trans.get_data_length();
  unsigned char*   byt = trans.get_byte_enable_ptr();
  unsigned int     wid = trans.get_streaming_width();
  
  // Check the error possibilities ...
  // ... address range
  if (adr >= uint64(ssMemorySize/4))
  {
    trans.set_response_status(tlm::TLM_ADDRESS_ERROR_RESPONSE);
    return;
  }
  // ... byte enable
  if (byt != 0)
  {
    trans.set_response_status(tlm::TLM_BYTE_ENABLE_ERROR_RESPONSE);
    return;
  }
  // ... burst
  if (len > 4 || wid < len)
  {
    trans.set_response_status(tlm::TLM_BURST_ERROR_RESPONSE);
    return;
  }
  
  // Do the read and write.
  if(cmd == tlm::TLM_READ_COMMAND) // cmd = 0
  {
    write_s.write(0);   // goes to the slave functional block
    adr_s.write(adr); // goes to the slave functional block
    wait(dataRd_e);   // need to wait on the data from the slave functional block
    // set the data pointer
    *reinterpret_cast<T*>(ptr) = tdo_s.read();
      
    delay = ssAcceptDelay + ssReadResponseDelay;
  }
  else if (cmd == tlm::TLM_WRITE_COMMAND) // cmd = 1
  {
    write_s.write(1);   // goes to the slave functional block
    adr_s.write(adr); // goes to the slave functional block
    tdi_s.write(*reinterpret_cast<T*>(ptr));
      
    delay = ssAcceptDelay + ssWriteResponseDelay;
  }
  else if (cmd == tlm::TLM_IGNORE_COMMAND)
  {
    write_s.write(2);   // goes to the slave functional block
    
      
    delay = SC_ZERO_TIME;
  }

  // Set the ok response
  trans.set_response_status(tlm::TLM_OK_RESPONSE);
}
/*-----------------------------------------------------------------------------
-- void cmdThread(void): Delivers "Read" or "Write" to the sc_export.
-----------------------------------------------------------------------------*/
template<class T>
void DataRegBPI<T>::cmdThread(void)
{
  unsigned int v;
  while(true)
  {
    wait(write_s.value_changed_event());
    v = write_s.read();
    write_o->write(v);
  }//while
}

/*-----------------------------------------------------------------------------
-- void adrThread(void): Delivers/writes "Address" to the sc_export.
-----------------------------------------------------------------------------*/
template<class T>
void DataRegBPI<T>::adrThread(void)
{
  unsigned int v;
  while(true)
  {
    wait(adr_s.value_changed_event());
    v = adr_s.read();
    address_o->write(v);
  }//while
}

/*-----------------------------------------------------------------------------
-- void dawThread(void): Delivers/writes "Data" to the sc_export.
-----------------------------------------------------------------------------*/
template<class T>
void DataRegBPI<T>::dawThread(void)
{
  T v;
  while(true)
  {
    wait(tdi_s.value_changed_event());
    v = tdi_s.read();
    tdi_o->write(v);
  }//while
}

/*-----------------------------------------------------------------------------
-- void darThread(void): Data read; data comes from the SlaveFunc and triggers
-- the bus transport.
-----------------------------------------------------------------------------*/
template<class T>
void DataRegBPI<T>::darThread(void)
{
  while(true)
  {
    // wait for the data comming from the functional block
    wait(sBPIDatRdEv_e);

    // give b_transport a note, that the data is now available and can be
    // placed on the bus
    dataRd_e.notify(SC_ZERO_TIME);
  }
}
