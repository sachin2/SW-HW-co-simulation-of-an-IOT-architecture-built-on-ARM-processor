#include "armtop.h"
#include "stim.cpp"
#include "armfunc.cpp"
#include "comp.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*!----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T, class U>
ARMTop<T,U>::ARMTop(sc_module_name nm
, const unsigned int   ID                   // target ID
)
  : sc_module(nm)
  , ID                 (ID)               // init target ID
{
  //SC_HAS_PROCESS(asCPUTop);
  cout << left << setw(30) << "ConstructorCPP - ARMTop:    "
       << left << setw(36) << name()
       << left << setw(14) << " with the ID: "
       << left << setw(5)  << dec << ID
       << endl;
       
  mcomp = new Comp("My_Comp_Module");
  mstim = new Stim("My_Stim_Module");
  mbpi = new SystemMasterBPI<T>("ARM1-BPI", ID, masterDataReadEvent_e);
  //! Initialization: Masters function.
  marm = new ARMFunc<T,U>("ARM1-Func", masterDataReadEvent_e);
  marm->data_op1_i(data_op1_s);
  marm->data_op2_i(data_op2_s);
  marm->op_code_i(op_code_s);
  //marm->result_i(result_s);
  //marm->flag_i(flag_s);

  mstim->data1_o(data_op1_s);
  mstim->data2_o(data_op2_s);
  mstim->func4_o(op_code_s);
  //mstim->func3_o(func3_s);
  
  mcomp->data1_i(data_op1_s);
  mcomp->data2_i(data_op2_s);
  mcomp->func4_i(op_code_s);
  //mcomp->func3_i(func3_s);

  //asreg1->gpio_o(gpio0_o_s);
  //asreg1->gpio_i(gpio0_i_s);
  //asreg1->gpio_dir_i(gpio0_dir_i_s);
  
  //asreg2->gpio_o(gpio1_o_s);
  //asreg2->gpio_i(gpio1_i_s);
  //asreg2->gpio_dir_i(gpio1_dir_i_s);
    
  //! Initialization: Master BPI.
  

  //! Binding of the sc_export's.
  marm->write_o(mbpi->write_s);
  marm->address_o(mbpi->address_s);
  marm->data_o(mbpi->dataWr_s);
  mbpi->data_o(marm->dataIn_s);
}