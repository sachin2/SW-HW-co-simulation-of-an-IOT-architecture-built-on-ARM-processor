#include "top_simple.h"
#include "alu/alu.cpp"
#include "bus_system/SystemBus.cpp"
#include "bus_system/SystemRegisterBPI.cpp"
#include "register/RegisterTop_system.cpp"
#include "bus_system/SystemMasterBPI.cpp"
#include "arm/armtop.cpp"
#include "Scanchain/Scanchain_top.cpp"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*!----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
template<class T, class U>
top<T,U>::top(sc_module_name nm)
  : sc_module(nm)
  , sysbus("System-Bus")
{
  cout << left << setw(30) << "ConstructorCPP - top:         "
       << left << setw(35) << name()
       << endl;
  
  
  // Construct the ARM
  arm    = new ARMTop<T, U>("ARM", 1);

  // Construct the ALU
  alu    = new ALU<T>("ALU_top");

  // Construct the Scanchain_top
  scanchain_top    = new Scanchain_top<T, U>("Scanchain_top");

  // Construct the periperals
 

 reg1    = new RegisterTop<T>("Register1", 3, "Reg1Socket1",
                              REG0BYTES, REGIWIDTH, REG0ADDRE, REG0RESET,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
  
 reg2    = new RegisterTop<T>("Register2", 4, "Reg2Socket1",
                              REG1BYTES, REGIWIDTH, REG1ADDRE, REG1RESET,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
                              
 reg3    = new RegisterTop<T>("Register3", 5, "Reg2Socket1",
                              REG1BYTES, REGIWIDTH, REG1ADDRE, REG1RESET,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
  
 reg4    = new RegisterTop<T>("Register4", 6, "Reg2Socket1",
                              REG1BYTES, REGIWIDTH, REG1ADDRE, REG1RESET,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
                                                                                      
 reg5    = new RegisterTop<T>("Register5", 7, "Reg2Socket1",
                              REG1BYTES, REGIWIDTH, REG1ADDRE, REG1RESET,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
                              
 reg6    = new RegisterTop<T>("Register6", 8, "Reg2Socket1",
                              REG1BYTES, REGIWIDTH, REG1ADDRE, REG1RESET,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
                              
 reg7    = new RegisterTop<T>("Register7", 9, "Reg2Socket1",
                              REG1BYTES, REGIWIDTH, REG1ADDRE, REG1RESET,
                              rwAccess_t, rwReadDelay_t, rwWriteDelay_t);
                              
                                                            
  /*---------------------------------------------------------------------------
  -- Connect blocks
  ---------------------------------------------------------------------------*/
  
  arm->mbpi->Master1Socket(sysbus.slaveSocket[0]);
  
  sysbus.masterSocket[0](reg1->mbpi->Slave1Socket);
  sysbus.masterSocket[1](reg2->mbpi->Slave1Socket);
  sysbus.masterSocket[2](reg3->mbpi->Slave1Socket);
  sysbus.masterSocket[3](reg4->mbpi->Slave1Socket);
  sysbus.masterSocket[4](reg5->mbpi->Slave1Socket);
  sysbus.masterSocket[5](reg6->mbpi->Slave1Socket);
  sysbus.masterSocket[6](reg7->mbpi->Slave1Socket);

  gpio0_dir_i_s = 0; // write to Reg 1(Oprand1)
  gpio1_dir_i_s = 0; // write to Reg 2(Oprand2)
  gpio2_dir_i_s = 0; // write to Reg 3(OpCode)
  gpio3_dir_i_s = 1; // Read from Reg 4 (Result)
  gpio4_dir_i_s = 1;// Read from Reg 4 (Flags)
 

  // REGISTER 1 
  reg1->gpio_o(gpio0_o_s);
  reg1->gpio_i(gpio0_i_s);
  reg1->gpio_dir_i(gpio0_dir_i_s);

  // REGISTER 2 
  reg2->gpio_o(gpio1_o_s);
  reg2->gpio_i(gpio1_i_s);
  reg2->gpio_dir_i(gpio1_dir_i_s);
  
  // REGISTER 3 
  reg3->gpio_o(gpio2_o_s);
  reg3->gpio_i(gpio2_i_s);
  reg3->gpio_dir_i(gpio2_dir_i_s);
  
  // REGISTER 4 
  reg4->gpio_o(gpio3_o_s);
  reg4->gpio_i(gpio3_i_s);
  reg4->gpio_dir_i(gpio3_dir_i_s);
  
  // REGISTER 5 
  reg5->gpio_o(gpio4_o_s);
  reg5->gpio_i(gpio4_i_s);
  reg5->gpio_dir_i(gpio4_dir_i_s);
  
  // Switch Register
  reg6->gpio_o(gpio5_o_s);
  reg6->gpio_i(gpio5_i_s);
  reg6->gpio_dir_i(gpio5_dir_i_s);
  
  
  // LED Register
  reg7->gpio_o(gpio6_o_s);
  reg7->gpio_i(gpio6_i_s);
  reg7->gpio_dir_i(gpio6_dir_i_s);
  
  //******************************

  alu->data_op1_i(gpio0_o_s);
  alu->data_op2_i(gpio1_o_s);
  alu->control_sig_i(gpio2_o_s);
  alu->result_o(gpio3_i_s);
  alu->flag_o(gpio4_i_s);
  
  //reg4->gpio_i(alu->flag_s);
  //reg5->gpio_i(alu->result_s);
  
}


