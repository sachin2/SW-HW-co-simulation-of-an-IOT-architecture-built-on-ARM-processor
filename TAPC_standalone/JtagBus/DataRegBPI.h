#pragma once
#ifndef _DATAREGBPI_H_
#define _DATAREGBPI_H_

#include "../asProcPack.h"
#include "systemc.h"
#include <iomanip>

using namespace sc_core;
using namespace sc_dt;
using namespace std;

#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"

template<class T>
//SC_MODULE(asSystemRegisterBPI)
class DataRegBPI : public sc_module
{
  public:
    /*-------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    // write_o: indicates the read/write direction; 0 for a read, 1 for a write,
    // 2 for ignore; read from the payload. Connected to the functional block.
    sc_out<unsigned int>            write_o;
    // address_o: is the memory or register address; read from the payload.
    // Connected to the functional block.
    sc_out<unsigned int>            address_o;
    // data_o: is the data in case of a write; read from the payload.
    // Connected to the functional block.
    sc_out<T>                       tdi_o;
    // data_i: is the data in case of a read; written to the payload.
    // Connected to the functional block.
    sc_export<sc_signal_in_if<T> >  tdo_i;
    /*-------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(asSystemRegisterBPI);
    DataRegBPI(sc_module_name   nm                 //!< SC module name
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
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    //sc_export connected signals
    sc_signal<T>            tdo_s;
    // general signals
    sc_signal<unsigned int> write_s;
    sc_signal<unsigned int> adr_s;
    sc_signal<T>            tdi_s;
    /*-------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    tlm_utils::simple_target_socket<DataRegBPI> ssSlave2Socket;//!< target socket
  //sc_event& sBPIDatRdEv_e;

  private:
    /*-------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
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
    const unsigned int ssID;                   //!< target ID
    uint64             ssMemorySize;           //!< memory size (bytes)
    unsigned int       ssMemoryWidth;          //!< word size (bytes)
    const sc_time      ssAcceptDelay;          //!< accept delay
    const sc_time      ssReadResponseDelay;    //!< read response delay
    const sc_time      ssWriteResponseDelay;   //!< write response delays
  public:
    sc_event& sBPIDatRdEv_e; // it is here because of a reorder error
};
#endif
