#include "comp.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
Comp::Comp(sc_module_name nm)
  : sc_module(nm)
{
  cout << "ConstructorCPP - Comp: " << name() << endl;
  SC_METHOD(CompMethod);
  sensitive << data1_i << data2_i<< func4_i;
}
void Comp::CompMethod(void)
{  
  sc_uint<32> a;
  sc_uint<32> b;
  sc_uint<4> c;
  //sc_uint<3> d;
   a = data1_i.read();
   b = data2_i.read(); 
   c = func4_i.read(); 
   //d = func3_i.read(); 
   //cout << "A=" << a << " " << "B=" << b << "C=" << c << endl;
   
   next_trigger(data1_i.default_event()|data2_i.default_event()|func4_i.default_event());
}
