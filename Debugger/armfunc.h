/// \brief          The functional level of the ARM (.h).
/// \note           There are two address variables, one can be removed in a 
///                 next version.
/// \details        A simple ARM model. It contains the algorithm. The algorithm
///                 is writing dummy data to a memory (ID=0), writing dummy data to
///                 register1 (LED, ID=1), writing dummy data to register2 (Switch, ID=2),
///                 reading the switches and writing it to the memory. Finally,
///                 the memory is read to get again the switch values.
/// \param[in]      data_i          The data in case of a CPI read command.
///                                 It will be connected and read in the CPU BPI.
///                                 It is a sc_export of type T. 
/// \param[in]      var2            Description of variable two, an input
/// \param[out]     var3            Description of variable three, an output 
///                     (usu. via a pointer to a variable)
/// \param[in,out]  var4            Description of variable four, an 
///                     input/output (usu. via a pointer) since its initial 
///                     value is read and used, but then it is also updated by 
///                     the function at some point
/// \tparam T       Data type for the data, it can be char or unsigned int.
/// \tparam U       Data type for the GPIOs. It is not needed here.
/// \return         Description of return types. It may be an enum, with these
///                 possible values:
///                 - #ENUM_VALUE_1
///                 - #ENUM_VALUE_2
///                 - #ENUM_VALUE_3
#ifndef _ARMFUNC_H_
#define _ARMFUNC_H_

#include "../ProcPack.h"
#include "systemc.h"
#include "tlm.h"
#include <iomanip>

//! T: data type for "data"; U: data type for GPIOs
template<class T, class U>

class ARMFunc : public sc_module
{
  public:
      /*!------------------------------------------------------------------------
    -- Ports
    -------------------------------------------------------------------------*/
    //! data_i: is the data in case of a read; read from the payload.
    sc_export<sc_signal_in_if<T> >  data_i;
    //sc_in<sc_uint<32>>	    result_i;
    //sc_in<sc_uint<3>>		    flag_i;
    //! write_o: indicates the read/write direction; 0 for a read, 1 for a write,
    // 2 for ign.; write it to the payload. Connected to the BPI.
    sc_out<unsigned int>            write_o;
    sc_in<sc_uint<32>>		    data_op1_i;
    sc_in<sc_uint<32>>		    data_op2_i;
    sc_in<sc_uint<4>>		    op_code_i;
    //! address_o: is the memory or register address; write it to the payload.
    // Connected to the BPI.
    sc_out<unsigned int>            address_o;
    //! data_o: is the data in case of a write; write it to the payload.
    // Connected to the BPI.
    sc_out<T>                       data_o;
    /*!------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    //! sc_export connected signals
    sc_signal<T> dataIn_s;
  
    /*!------------------------------------------------------------------------
    -- Constructor
    -------------------------------------------------------------------------*/
    //SC_CTOR(asSlaveFunc);
    ARMFunc(sc_module_name nm, sc_event& mFuncDatRdEv);
  
    /*!------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
  
  private:
    /*!------------------------------------------------------------------------
    -- FIFOs/Signals
    -------------------------------------------------------------------------*/
    unsigned int IR; //! Instruction register
  
    /*!------------------------------------------------------------------------
    -- Events
    -------------------------------------------------------------------------*/
    /*! When the information from the BPI comes, that the read data is
     * available, this event will be notified and captured by the thread which
     * requests the data.*/
    sc_event readMemRdy_e;
    //! Event, that indicates that the read data is ready; read here,
    // notified in the BPI. It is a reference from the respective event
    // declared in the CPU top level.
    sc_event& mFuncDatRdEv_e;
  
    /*!------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    // ... for the bus actions
    void getDatThread(void); //! Declaration: Thread for getting the data.
    void armSequenceThread(void); //! Declaration: This is the algorithm.
    void armWr1Print(unsigned int addr, T data);
    void armWrCmd(unsigned int addr, T data); //! Write mnemonic
    void armWrAndPrint(unsigned int addr, T data);
    T armRdCmd(unsigned int addr); //! Read mnemonic
    T armRdAndPrint(unsigned int addr);
    void armRd1Print(unsigned int addr, T data);
    void armFuncPrint01(T data);
    
    /*!------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    //static const unsigned int  m_txn_data_size = BYTESPERMEMLINE;
    // unsigned int currentAddress_v;
};
#endif