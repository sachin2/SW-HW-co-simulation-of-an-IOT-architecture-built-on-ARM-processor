#ifndef _TOP_H_
#define _TOP_H_

#include <systemc.h>
#include "tlm.h"
#include "ProcPack.h"
#include <iomanip>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "alu/alu.h"
#include "bus_system/SystemBus.h"
#include "bus_system/SystemRegisterBPI.h"
#include "register/RegisterTop_system.h"
#include "bus_system/SystemMasterBPI.h"
#include "arm/armtop.h"
#include "Scanchain/Scanchain_top.h"

template<class T, class U>
class top : public sc_module
{
  public:
    /*!-----------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    //! GPIO inputs
    sc_signal<T> gpio0_i_s;
    sc_signal<T> gpio1_i_s;
    sc_signal<T> gpio2_i_s;
    sc_signal<T> gpio3_i_s;
    sc_signal<T> gpio4_i_s;
    sc_signal<T> gpio5_i_s;
    sc_signal<T> gpio6_i_s;
   
    //! GPIO outouts
    sc_signal<T> gpio0_o_s;
    sc_signal<T> gpio1_o_s;
    sc_signal<T> gpio2_o_s;
    sc_signal<T> gpio3_o_s;
    sc_signal<T> gpio4_o_s;
    sc_signal<T> gpio5_o_s;
    sc_signal<T> gpio6_o_s;
    
    
    //! GPIO control: 0 for output, 1 for input
    sc_signal<bool> gpio0_dir_i_s;
    sc_signal<bool> gpio1_dir_i_s;
    sc_signal<bool> gpio2_dir_i_s;
    sc_signal<bool> gpio3_dir_i_s;
    sc_signal<bool> gpio4_dir_i_s;
    sc_signal<bool> gpio5_dir_i_s;
    sc_signal<bool> gpio6_dir_i_s;
    
    
    /*!------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    SC_CTOR(top);
    
    
  private:
   
    void switchReadThread(void); // read the switches file
   
    SystemBus<1, 7>         sysbus; //! Simple bus: 1 master, 7 slaves
    RegisterTop<T>*         reg1;   
    RegisterTop<T>*         reg2;   
    RegisterTop<T>*         reg3; 
    RegisterTop<T>*         reg4; 
    RegisterTop<T>*         reg5; 
    RegisterTop<T>*         reg6; 
    RegisterTop<T>*         reg7; 
    ARMTop<T, U>*           arm;
    ALU<T>*           alu;
    Scanchain_top<T, U>*    scanchain_top;
};
#endif
