#pragma once
#ifndef _SYSTEMREGISTERBPI_H_
#define _SYSTEMREGISTERBPI_H_

#include "ProcPack.h"
#include "systemc.h"
#include <iomanip>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

template<class T>
//SC_MODULE(SystemRegisterBPI)
class SystemRegisterBPI : public sc_module
{
  public:
    /*-------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    
    sc_out<unsigned int>            write_o;
  
    sc_out<unsigned int>            address_o;
   
    sc_out<T>                       data_o;
  
    sc_export<sc_signal_in_if<T> >  data_i;
    /*-------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(SystemRegisterBPI);
    SystemRegisterBPI(sc_module_name   nm                 //!< SC module name
      , const unsigned int      ID                 //!< target ID
      , const char              *memorySocket      //!< socket name
      , uint64                  memorySize         //!< memory size (bytes)
      , unsigned int            memoryWidth        //!< memory width(bytes)
      , const sc_time           acceptDelay        //!< accept delay
      , const sc_time           readResponseDelay  //!< read response delay
      , const sc_time           writeResponseDelay //!< write response delay
      , sc_event&               sBPIDatRdEv        //!< event, that indicates
	                                           //!< that the read data is
                                                   //!< ready to be transmitted
                                                   //!< to the CPU
      );
  
    //sc_export connected signals
    sc_signal<T>            dataRd_s;
    // general signals
    sc_signal<unsigned int> write_s;
    sc_signal<unsigned int> adr_s;
    sc_signal<T>            daw_s;
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    tlm_utils::simple_target_socket<
    SystemRegisterBPI> Slave1Socket;//!< target socket
  //sc_event& sBPIDatRdEv_e;

  private:
    
    /*-------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    sc_event dataRd_e;
    /*-------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    virtual void b_transport(tlm::tlm_generic_payload &trans, sc_time &delay);
    void cmdThread(void);
    void adrThread(void);
    void dawThread(void);
    void darThread(void);
    void slaveBPIPrint01(unsigned int addr);
    void slaveBPIPrint02(unsigned int addr, T data);
    void slaveBPIPrint03(unsigned int addr, T data);
    void slaveBPIPrint04(unsigned int addr, T data, tlm::tlm_command cmd);
    void slaveBPIPrint05(T data);
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    const unsigned int ID;                   //!< target ID
    uint64             MemorySize;           //!< memory size (bytes)
    unsigned int       MemoryWidth;          //!< word size (bytes)
    const sc_time      AcceptDelay;          //!< accept delay
    const sc_time      ReadResponseDelay;    //!< read response delay
    const sc_time      WriteResponseDelay;   //!< write response delays
  public:
    sc_event& sBPIDatRdEv_e; // it is here because of a reorder error
};
#endif
