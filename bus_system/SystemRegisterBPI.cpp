#include "SystemRegisterBPI.h"

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
SystemRegisterBPI<T>::SystemRegisterBPI(sc_module_name nm
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
, Slave1Socket       (memorySocket)       // init socket name
, ID                 (ID)                 // init target ID
, MemorySize         (memorySize)         // e.g., 1024 lines = 4*1024
, MemoryWidth        (memoryWidth)        // e.g., 4 bytes per line
, AcceptDelay        (acceptDelay)        // init accept delay
, ReadResponseDelay  (readResponseDelay)  // init read response delay
, WriteResponseDelay (writeResponseDelay) // init write response delay
, sBPIDatRdEv_e (sBPIDatRdEv)
{
  SC_HAS_PROCESS(SystemRegisterBPI);
  cout << left  << setw(30) << "ConstructorCPP - SystemRegisterBPI: "
       << left  << setw(36) << name()
       << left  << setw(15) << " with the ID: "
       << right << setw(5)  << dec << ID
       << left  << setw(8)  << ", Size: "
       << right << setw(5)  << dec << MemorySize
       << left  << setw(15) << " bytes, Width: "
       << right << setw(5)  << dec << MemoryWidth
       << " bytes"
       << endl;
  SC_THREAD(cmdThread);
  SC_THREAD(adrThread);
  SC_THREAD(dawThread);
  SC_THREAD(darThread);
  
  // binds the fifo to the export
  data_i.bind(dataRd_s);

  Slave1Socket.register_b_transport(this, &SystemRegisterBPI::b_transport);
}

template<class T>
void SystemRegisterBPI<T>::b_transport(tlm::tlm_generic_payload &trans,sc_time &delay)
{
  // get the information from the payload
  tlm::tlm_command cmd = trans.get_command();
  sc_dt::uint64    adr = trans.get_address() / BYTESPERMEMLINE;
  unsigned char*   ptr = trans.get_data_ptr();
  unsigned int     len = trans.get_data_length();
  unsigned char*   byt = trans.get_byte_enable_ptr();
  unsigned int     wid = trans.get_streaming_width();

  if(SLAVEBPIPRINTS)
    slaveBPIPrint01(adr);
  

  if (adr >= uint64(MemorySize/BYTESPERMEMLINE))
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
  if (len > BYTESPERMEMLINE || wid < len)
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
    *reinterpret_cast<T*>(ptr) = dataRd_s.read();
    
    if(SLAVEBPIPRINTS)
      slaveBPIPrint02(adr, dataRd_s.read());
      
    delay = AcceptDelay + ReadResponseDelay;
  }
  else if (cmd == tlm::TLM_WRITE_COMMAND) // cmd = 1
  {
    write_s.write(1);   // goes to the slave functional block
    adr_s.write(adr); // goes to the slave functional block
    daw_s.write(*reinterpret_cast<T*>(ptr));
    
    if(SLAVEBPIPRINTS)
      slaveBPIPrint03(adr, *reinterpret_cast<T*>(ptr));
      
    delay = AcceptDelay + WriteResponseDelay;
  }
  else if (cmd == tlm::TLM_IGNORE_COMMAND)
  {
    write_s.write(2);   // goes to the slave functional block
    
    if(SLAVEBPIPRINTS)
      slaveBPIPrint04(adr, *reinterpret_cast<T*>(ptr), cmd);
      
    delay = SC_ZERO_TIME;
  }

  // Set the ok response
  trans.set_response_status(tlm::TLM_OK_RESPONSE);
}
/*-----------------------------------------------------------------------------
-- void cmdThread(void): Delivers "Read" or "Write" to the sc_export.
-----------------------------------------------------------------------------*/
template<class T>
void SystemRegisterBPI<T>::cmdThread(void)
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
void SystemRegisterBPI<T>::adrThread(void)
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
void SystemRegisterBPI<T>::dawThread(void)
{
  T v;
  while(true)
  {
    wait(daw_s.value_changed_event());
    v = daw_s.read();
    data_o->write(v);
  }//while
}

/*-----------------------------------------------------------------------------
-- void darThread(void): Data read; data comes from the SlaveFunc and triggers
-- the bus transport.
-----------------------------------------------------------------------------*/
template<class T>
void SystemRegisterBPI<T>::darThread(void)
{
  T v;
  while(true)
  {
    // wait for the data comming from the functional block
    wait(sBPIDatRdEv_e);
    v = dataRd_s.read();
    
    if(SLAVEBPIPRINTS)
      slaveBPIPrint05(v);

    // give b_transport a note, that the data is now available and can be
    // placed on the bus
    dataRd_e.notify(SC_ZERO_TIME);
  }
}

/*-----------------------------------------------------------------------------
-- Helper functions
-----------------------------------------------------------------------------*/
// prints
template<class T>
void SystemRegisterBPI<T>::slaveBPIPrint01(unsigned int addr)
{
  cout << left  << setw(11) << "Slave-BPI "
       << left  << setw(20) << "- b_transport "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", wordAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7) << ", max "
       << right << hex << setw(9) << MemorySize
       << left  << setw(11) << ", ID "
       << dec << setw(4) << ID
       << endl;
}

template<class T>
void SystemRegisterBPI<T>::slaveBPIPrint02(unsigned int addr, T data)
{
  cout << left  << setw(11) << "Slave-BPI "
       << left  << setw(20) << "- read: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", wordAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       //<< hex << setw(9) << *reinterpret_cast<T*>(ptr)
       << right << hex << setw(9) << data
       << endl;
}

template<class T>
void SystemRegisterBPI<T>::slaveBPIPrint03(unsigned int addr, T data)
{
  cout << left  << setw(11) << "Slave-BPI "
       << left  << setw(20) << "- write: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", wordAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}

template<class T>
void SystemRegisterBPI<T>::slaveBPIPrint04(unsigned int addr, T data, tlm::tlm_command cmd)
{
  cout << left  << setw(11) << "Slave-BPI "
       << left  << setw(20) << "- ignor: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ", wordAdr."
       << right << hex << setw(9) << addr
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << left  << setw(7)  << ", cmd "
       << dec   << setw(4)  << cmd
       << endl;
}

template<class T>
void SystemRegisterBPI<T>::slaveBPIPrint05(T data)
{
  cout << left  << setw(11) << "Slave-BPI "
       << left  << setw(20) << "- rd func-BPI-Bus: "
       << left  << setw(3)  << " @ "
       << right << setw(9)  << sc_time_stamp()
       << left  << setw(10) << ""
       << right << setw(9)  << ""
       << left  << setw(7)  << ", data "
       << right << hex << setw(9) << data
       << endl;
}
