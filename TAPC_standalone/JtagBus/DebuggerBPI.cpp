#include "DebuggerBPI.h"

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T>
DebuggerBPI<T>::DebuggerBPI(sc_module_name nm , const unsigned int ID , sc_event& mBPIDatRdEv)
: sc_module            (nm)                 // init module name
, ID_r                 (ID)                 // ID register
, mBPIDatRdEv_e        (mBPIDatRdEv)        // bind to the reference?
, ssMaster1Socket("Debugger-Master1-Socket")
{
  SC_HAS_PROCESS(DebuggerBPI);
  cout << left << setw(30) << "ConstructorCPP - DebuggerBPI:    "
       << left << setw(36) << name()
       << left << setw(14) << " with the ID: "
       << left << setw(5)  << dec << ID_r
       << endl;
  SC_THREAD(initThread);
  SC_THREAD(cmdThread);
  
  
  // binds the fifo to the export
  tdi_i.bind(tdi_s);
  write_i.bind(write_s);
  address_i.bind(address_s);
}
/*-----------------------------------------------------------------------------
-- Methods
-----------------------------------------------------------------------------*/
template<class T>
void DebuggerBPI<T>::initThread(void)
{
  tlm::tlm_generic_payload *trans_p = new tlm::tlm_generic_payload;
  //sc_time delay = sc_time(6, SC_NS);
  sc_time delay = sc_time(SC_ZERO_TIME);
  while(true)
  {
    // ... here is the wait
    wait(cmd_e);

    // read cmd from export
    // if write: read data from export and write it to the payload
    // if read:  read data from bus - write to port (to functional block)
    if(write_s.read() == 1) // write; write=1
    {
      dataBfr = tdi_s.read();
    }
    tlm::tlm_command cmd = static_cast<tlm::tlm_command>(write_s.read());
    trans_p->set_command(cmd);
    trans_p->set_address(address_s.read());
    trans_p->set_data_ptr(reinterpret_cast<unsigned char*>(&dataBfr));
    trans_p->set_data_length(BYTESPERMEMLINE);
    trans_p->set_streaming_width(BYTESPERMEMLINE);
    trans_p->set_byte_enable_ptr(0); // 0 indicates unused
    trans_p->set_dmi_allowed(false); // Mandatory initial value
    trans_p->set_response_status(tlm::TLM_INCOMPLETE_RESPONSE);
    // send the payload on its journey
    ssMaster1Socket->b_transport(*trans_p, delay); // Blocking transport call

    if (trans_p->get_command() == tlm::TLM_READ_COMMAND) // read; cmd=0
    {
      // write the read data to the functional block
      tdo_o->write(dataBfr);
      // notify the functional block, that new data is available
      mBPIDatRdEv_e.notify(SC_ZERO_TIME);
    }
	   
    // Check back channel
    // Initiator obliged to check response status and delay
    backChannelProc(*trans_p);
    
    wait(delay);
    
      
  }//while
}

template<class T>
void DebuggerBPI<T>::backChannelProc(tlm::tlm_generic_payload &pl)
{
  if(pl.is_response_error())
  {
    char txt[100];
    sprintf(txt, "Master-BPI - Error from b_transport, response status = %s",
            pl.get_response_string().c_str());
    SC_REPORT_ERROR("TLM-2", txt);
  }
  else
  {
    // no error
    
  }
}

// Cmd, generated in MasterFunc, will be got here. For bus action.
template<class T>
void DebuggerBPI<T>::cmdThread(void)
{
  while(true)
  {
    // wait, until something happens on the bus ...
    wait(write_s.value_changed_event());

    // notify, that something happens on the bus; the init_thread waits for it
    cmd_e.notify(SC_ZERO_TIME);
  }
}
