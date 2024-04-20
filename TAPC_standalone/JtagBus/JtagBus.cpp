#include "JtagBus.h"


template <unsigned int nrMASTER, unsigned int nrSLAVE>
JtagBus<nrMASTER, nrSLAVE>::JtagBus(sc_module_name nm)
  : sc_module(nm)
{
  //SC_HAS_PROCESS(JtagBus);
  cout << left << setw(30) << "ConstructorCPP - JtagBus: "
       << left << setw(36) << name()
       << left << setw(10) << ", Masters: "
       << left << setw(5)  << dec << nrMASTER
       << left << setw(10) <<  ", Slaves: "
       << left << setw(5)  << dec << nrSLAVE
       << endl;
  //! Register slave sockets for all master
  // - A master devices socket is connected to a bus slave  socket.
  // - A slave  devices socket is connected to a bus master socket.
  for (unsigned int i = 0; i < nrMASTER; ++i)
  {
    slaveSocket[i].register_b_transport(this, &JtagBus::b_transport, i);
  }
  //! Register master sockets for all slaves
  for (unsigned int i = 0; i < nrSLAVE; ++i)
  {
    // nothing
  }
}
/*!----------------------------------------------------------------------------
-- Methods
-----------------------------------------------------------------------------*/
template <unsigned int nrMASTER, unsigned int nrSLAVE>
void JtagBus<nrMASTER, nrSLAVE>::b_transport(int id, payLoad_t& trans, sc_time& t)
{
  //! The id is the number of the master
  if (id < (int)nrMASTER)
  {
    masteSock_t* decodeSocket;
    //! portId will be used to find the correct slave.
    // It is the index number of the array of sockets.
    sc_dt::uint64 address = trans.get_address();
    sc_dt::uint64 masked_address;
    unsigned int portId = decode_address(address, masked_address);
    if (portId < nrSLAVE)
    {
      //! Modify address within transaction; only the addresses inside the mem.
      trans.set_address(masked_address);
      //! Forward transaction to appropriate target
      //! The master socket of the bus must be connected to a slave.
      decodeSocket = &masterSocket[portId];
      (*decodeSocket)->b_transport(trans, t); //! ... goes to slave
    }
  }
  else
    SC_REPORT_FATAL("TLM-2", "Invalid tagged socket id in bus"); 
}

