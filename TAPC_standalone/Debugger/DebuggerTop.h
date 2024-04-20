#ifndef _DEBUGGERTOP_H_
#define _DEBUGGERTOP_H_

#include <systemc.h>
#include "DebuggerFunc.h"
//#include "stim.h"

template<class T, class U>
class DebuggerTop : public sc_module
{
  public:
  
    DebuggerTop(sc_module_name nm, const unsigned int ID);
  
    //! Declaration: Master BPI.
    DebuggerBPI<T>*      mbpi; //! The BPI of the master.
	
  
  private:
  
    //! Declaration: The event connects the functional part with the BPI.
    // It acts as the original, all others down the hirarchy are references.
    sc_event masterDataReadEvent_e;
  
  
    const unsigned int ssID;                   //! Target ID
  
    
    //dataGen              dgen;  // So ....     ; Stack allocation
    //! Declaration: Masters function.
    DebuggerFunc<T,U>*     mdeb; //! The functional part of the master.
    //ssStim*    	           mstim;
};
#endif
