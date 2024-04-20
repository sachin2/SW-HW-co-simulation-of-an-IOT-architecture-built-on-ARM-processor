//! \brief The top level of the CPU (h).
/*! A simple CPU model. It contains a BPI and a functional part.
The functional part contains the algorithm, the BPI is the bus peripheral 
interface.
*/
#ifndef _ARMTOP_H_
#define _ARMTOP_H_

#include "../ProcPack.h"
#include <systemc>
#include "armfunc.h"
#include "stim.h"
#include "comp.h"


template<class T, class U>

class ARMTop : public sc_module
{
  public:
    /*!------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    sc_signal<sc_uint<32>> data_op1_s;
    sc_signal<sc_uint<32>> data_op2_s;
    sc_signal<sc_uint<4>> op_code_s;
    //sc_signal<sc_uint<32>> result_s;
    //sc_signal<sc_uint<3>> flag_s;
    /*!------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(CPUTop);
    ARMTop(sc_module_name nm, const unsigned int ID);
  
    /*!------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    /*!------------------------------------------------------------------------
    -- Hierarchy: Public in case of needed traces.
    -------------------------------------------------------------------------*/
    //! Declaration: Master BPI.
    SystemMasterBPI<T>*      mbpi; //! The BPI of the master.
  
  private:
    /*!------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    /*!------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    //! Declaration: The event connects the functional part with the BPI.
    // It acts as the original, all others down the hirarchy are references.
    sc_event masterDataReadEvent_e;
  
    /*!------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    Comp*     mcomp;
    Stim*     mstim;
    /*!------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    const unsigned int ID;                   //! Target ID
  
    /*!------------------------------------------------------------------------
    -- Hierarchy: Private, if no traces needed.
    -------------------------------------------------------------------------*/
    //! Declaration: Masters function.
    ARMFunc<T,U>*     marm; //! The functional part of the master.
};
#endif