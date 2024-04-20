#include "SystemBus.h"


template <unsigned int nrMASTER, unsigned int nrSLAVE>
SystemBus<nrMASTER, nrSLAVE>::SystemBus(sc_module_name nm)
  : sc_module(nm)
{
  //SC_HAS_PROCESS(SystemBus);
  cout << left << setw(30) << "ConstructorCPP - SystemBus: "
       << left << setw(36) << name()
       << left << setw(10) << ", Masters: "
       << left << setw(5)  << dec << nrMASTER
       << left << setw(10) <<  ", Slaves: "
       << left << setw(5)  << dec << nrSLAVE
       << endl;
  
  for (unsigned int i = 0; i < nrMASTER; ++i)
  {
    slaveSocket[i].register_b_transport(this, &SystemBus::b_transport, i);
  }
  //! Register master sockets for all slaves
  for (unsigned int i = 0; i < nrSLAVE; ++i)
  {
    // nothing
  }
}

template <unsigned int nrMASTER, unsigned int nrSLAVE>
void SystemBus<nrMASTER, nrSLAVE>::b_transport(int id,
					 payLoad_t& trans,
					 sc_time& t)
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
    if(BUSPRINTS)
      print01((char*)"Bus        - b_trans.: ", address,
              masked_address,
              *reinterpret_cast<unsigned int*>(trans.get_data_ptr()),
              sc_time_stamp(), portId, id);
  }
  else
    SC_REPORT_FATAL("TLM-2", "Invalid tagged socket id in bus");
  
  
}

// prints; ID is the number of the master;
// target is the number of the slave
template <unsigned int nrMASTER, unsigned int nrSLAVE>
void SystemBus<nrMASTER, nrSLAVE>::print01(char* text, unsigned int addr,
		 unsigned int maddr,
                 int data, sc_time timestamp, unsigned int target,
                 unsigned int id)
{
  cout << left  << setw(31) << text
       << left  << setw(3)  << " @ "
       << right << setw(9)  << timestamp
       << left  << setw(10) << ", byteAdr."
       << right << setw(9)  << hex << addr
       << left  << setw(7)  << ", data "
       << right << setw(9)  << hex << data
       << left  << setw(10) << ", maskAdr."
       << right << setw(9)  << hex << maddr
       << left  << setw(10) << ", targID."
       << right << setw(4)  << dec << target
       << left  << setw(10) << ", masteID."
       << right << setw(4)  << dec << id
       << endl;
}
