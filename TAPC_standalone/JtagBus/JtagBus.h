//! \brief Simple TLM bus model - the bus module (h).
/*! A simple TLM bus model with configurable masters and slaves.
*/
#pragma once
#ifndef _JTAGBUS_H_
#define _JTAGBUS_H_

#include <systemc>
#include <iomanip>
#include "../asProcPack.h"
#include "tlm.h"
#include "tlm_utils/simple_target_socket.h"
#include "tlm_utils/simple_initiator_socket.h"

using namespace std;
using namespace sc_core;
using namespace sc_dt;

//! The template gives the number of masters and slaves which will be attached
// to the bus.
template <unsigned int nrMASTER, unsigned int nrSLAVE>
//SC_MODULE(asSystemBus)
class JtagBus : public sc_module
{
  public:
    /*!------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    /*!------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    //! Types
    typedef tlm::tlm_generic_payload       payLoad_t;
    //typedef tlm::tlm_phase                 phase_type;
    //typedef tlm::tlm_sync_enum             sync_enum_type;
    typedef tlm_utils::simple_target_socket_tagged<JtagBus>    slaveSock_t;
    typedef tlm_utils::simple_initiator_socket_tagged<JtagBus> masteSock_t;
    //! Sockets
    slaveSock_t    slaveSocket[nrMASTER]; //! Slave sockets for the masters
    masteSock_t    masterSocket[nrSLAVE]; //! Master sockets for the slaves
    /*!------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    SC_CTOR(JtagBus);
  private:
    /*!------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    //! Trans(ports) the data, address, cmd (read, write) through the
    // channels (the sockets).
    virtual void b_transport(int id, payLoad_t& trans, sc_time& t);
    //! Takes the upper two (four) bits (out of eight (32)) are the target_nr.
    // The masked_address is the original address without the upper
    // two bits. Decodes for chip selects.
    // In case of a 32 bit bus, the target_nr could be 4 bits, so the
    // remaining address bits for the memory intenal address are
    // 28 bits (BUSMEMINTBITS=28). So, the total address needs to be
    // shifted 28 positions to the right side and to mask the needed 4 bits
    // with the BUSTARGETMASK. => target_nr.
    // The masked_address is the original address masked with
    // 7 nibbles F (BUSDECODEMASK).
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
