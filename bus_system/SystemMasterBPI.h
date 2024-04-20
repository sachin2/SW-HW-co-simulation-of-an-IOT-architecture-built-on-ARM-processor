#pragma once
#ifndef _SYSTEMMASTERBPI_H_
#define _SYSTEMMASTERBPI_H_

#include "ProcPack.h"
#include "systemc.h"
#include <iomanip>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"

#include "tlm_utils/simple_initiator_socket.h"


template<class T>
class SystemMasterBPI : public sc_module
{
  public:
    /*-------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    
    sc_export<sc_signal_in_if<unsigned int> >  write_i;
    
    sc_export<sc_signal_in_if<unsigned int> >  address_i;
    
    sc_export<sc_signal_in_if<T> >  data_i;
    
    sc_out<T>                       data_o;
    /*-------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(SystemMasterBPI);
    unsigned int ID_r; // ID register
   
    sc_event& mBPIDatRdEv_e;
    SystemMasterBPI(sc_module_name nm                 //!< SC module name
           , const unsigned int      ID             //!< target ID
           , sc_event&               mBPIDatRdEv
      );
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    // Socket of this module
    tlm_utils::simple_initiator_socket<SystemMasterBPI> Master1Socket;
    // data register
    unsigned int data_r;
    //sc_export connected signals
    sc_signal<unsigned int> write_s;
    sc_signal<unsigned int> address_s;
    sc_signal<T> dataWr_s;
    
  private:
   
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
    void masterBPIPrint01(void);
    void masterBPIPrint02(void);
    void masterBPIPrint03(unsigned int addr, T data, tlm::tlm_command cmd);
    void masterBPIPrint04(void);
    void masterBPIPrint05(T data);
    void masterBPIPrint06(unsigned int cmd);
  
};
#endif
