
#pragma once
#ifndef __SYSTEMBUS_H__
#define __SYSTEMBUS_H__

#include <systemc>
#include <iomanip>
#include "ProcPack.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;


template <unsigned int nrMASTER, unsigned int nrSLAVE>
//SC_MODULE(SystemBus)
class SystemBus : public sc_module
{
  public:
    
    /*!------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    //! Types
    typedef tlm::tlm_generic_payload       payLoad_t;
   
    typedef tlm_utils::simple_target_socket_tagged<SystemBus>    slaveSock_t;
    typedef tlm_utils::simple_initiator_socket_tagged<SystemBus> masteSock_t;
    //! Sockets
    slaveSock_t    slaveSocket[nrMASTER]; //! Slave sockets for the masters
    masteSock_t    masterSocket[nrSLAVE]; //! Master sockets for the slaves
    /*!------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    SC_CTOR(SystemBus);
  private:
   
    virtual void b_transport(int id, payLoad_t& trans, sc_time& t);
   
    inline unsigned int decode_address(sc_dt::uint64 address,
				       sc_dt::uint64& masked_address)
    {
      unsigned int target_nr =
        static_cast<unsigned int>((address >> BUSMEMINTBITS) & BUSTARGETMASK);
      masked_address = address & BUSDECODEMASK; //! Upper nibble forced to 0.
      return target_nr;
    }

    //! helper function: print
    void print01(char* text, unsigned int addr, unsigned int maddr,
                 int data, sc_time timestamp, unsigned int target,
                 unsigned int id);
};
#endif
