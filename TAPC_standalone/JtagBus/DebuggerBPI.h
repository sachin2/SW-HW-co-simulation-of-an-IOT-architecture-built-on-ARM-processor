#pragma once
#ifndef _DEBUGGERBPI_H_
#define _DEBUGGERBPI_H_

#include "../asProcPack.h"
#include "systemc.h"
#include <iomanip>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

template<class T>
class DebuggerBPI : public sc_module
{
  public:
    /*-------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    // write_i: indicates the read/write direction; 0 for a read, 1 for a write,
    // 2 for ign.; write it to the payload. Connected to the functional block.
    sc_export<sc_signal_in_if<unsigned int> >  write_i;
    // address_i: is the register address; write it to the payload.
    // Connected to the functional block.
    sc_export<sc_signal_in_if<unsigned int> >  address_i;
    //! data_o: is the data in case of a write; write it to the payload.
    // Connected to the BPI.
    sc_export<sc_signal_in_if<T> >             tdi_i;
    sc_out<T>          		   	       tdo_o;
    /*-------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(asDebuggerBPI);
    unsigned int ID_r; // ID register
    // event, that indicates that the read data is ready; notified here,
    // read in the functional block
    sc_event& mBPIDatRdEv_e;
    DebuggerBPI(sc_module_name nm , const unsigned int ID , sc_event& mBPIDatRdEv);
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    // Socket of this module
    tlm_utils::simple_initiator_socket<DebuggerBPI> ssMaster1Socket;
    // data register
    unsigned int dataBfr;
    //sc_export connected signals
    sc_signal<unsigned int> write_s;
    sc_signal<unsigned int> address_s;
    sc_signal<T>    	    tdi_s;
    // general signals
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
  private:
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    /*-------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    // indicates, that there is a new action on the bus
    sc_event cmd_e;
    /*-------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    //virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay);
    void cmdThread(void);
    void initThread (void);
    void backChannelProc(tlm::tlm_generic_payload &pl);
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    //const unsigned int asID;                   //!< target ID
  //public:
  //sc_event& mBPIDatRdEv_e;
};
#endif
