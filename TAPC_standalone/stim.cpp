#include "stim.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-------------------------------------------------------------------------------------------
---constructor
--------------------------------------------------------------------------------------------*/
ssStim::ssStim(sc_module_name nm) : sc_module(nm)
{
 cout << "StimulationModule" << name() << endl;
 SC_THREAD(ssStimThread);
}

void ssStim::ssStimThread(void)
{

  tms.write(1);
  wait(100, SC_NS);
  
  tms.write(0);
  wait(100, SC_NS);
  
  tms.write(1);
  wait(100, SC_NS);
  
  tms.write(1);
  wait(100, SC_NS);
  
  tms.write(0);
  wait(100, SC_NS);
  
  tms.write(1);
  wait(100, SC_NS);
  
  tms.write(1);
  wait(100, SC_NS);
  
  tms.write(1);
  wait(100, SC_NS);
      
  cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
  cout << "xx End of Simulation!" << endl;
  cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
  sc_stop();

}
