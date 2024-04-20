#include "Scanchain_top.h"


using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*!----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template <class T, class U>
Scanchain_top<T, U>::Scanchain_top(sc_module_name nm)
    : sc_module(nm) {
   cout << left << setw(30) << "ConstructorCPP -Scanchain_top:         "
       << left << setw(35) << name()
       << endl;
       
    // Connect  ports

   
}


