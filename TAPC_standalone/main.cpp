//! \brief Simple TLM bus model.
/*!
  A simple stub for a TLM bus model with one master and two slaves.
*/

#include "systemc.h"
#include "top.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

int exit_status = 0; //global

int sc_main (int argc, char* argv[])
{
  //!Direkte Instantiierung auf oberster Ebene; <Bus-Data-Type, FIFO-Type>
  top<unsigned int, char> topx("DebuggerTAP-Top");

  
  cout << "Simulation started: Time resolution: "
       << sc_get_time_resolution() << endl;

  sc_start();

  if(!sc_end_of_simulation_invoked())
  {
    cout << "Simulation stopped: " << endl;
    sc_stop();
  }
  
  return exit_status;
}
