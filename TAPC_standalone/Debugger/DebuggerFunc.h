#ifndef _DEBUGGERFUNC_H_
#define _DEBUGGERFUNC_H_

#include "../asProcPack.h"
#include "systemc.h"
#include "tlm.h"
#include <iomanip>


//! T: data type for "data"; U: data type for GPIOs
template<class T, class U>
class DebuggerFunc : public sc_module
{
  public:
    // ports
    sc_export<sc_signal_in_if<T> >  tdo_i;
    //! write_o: indicates the read/write direction; 0 for a read, 1 for a write,
    // 2 for ign.; write it to the payload. Connected to the BPI.
	sc_out<unsigned int>       write_o;
    //! address_o: is the register address; write it to the payload.
    // Connected to the BPI.
	sc_out<unsigned int>       address_o;
    //! data_o: is the data in case of a write; write it to the payload.
    // Connected to the BPI.
    	sc_out<T>          tdi_o;
	
	
    //fifo's and signals
    //! sc_export connected signals
	unsigned int bufferDR_r;
	unsigned int bufferIR_r;
	sc_signal<T> tdo_s;
	sc_signal<sc_bv<16>> tms_s;


    //Constructor
    DebuggerFunc(sc_module_name nm, sc_event& mTestDatRdEv);

  
  private:
    /*! When the information from the BPI comes, that the read data is
     * available, this event will be notified and captured by the thread which
     * requests the data.*/
    sc_event readMemRdy_e;
    //! Event, that indicates that the read data is ready; read here,
    // notified in the BPI. It is a reference from the respective event
    // declared in the CPU top level.
    sc_event& mTestDatRdEv_e;
	
    /*!------------------------------------------------------------------------
    -- Methods/Threads
    -------------------------------------------------------------------------*/
    // ... for the bus actions
    void getDatThread(void); //! Declaration: Thread for getting the data.
    void stateTransitionThread(void); 
    void debDRThread(void);  //! Declaration: This is the algorithm.
    void debWr1PrintDR(unsigned int addr, T data);
    void debWrDR(unsigned int addr, T data); 
    void debWrAndPrintDR(unsigned int addr, T data);
    T shiftIR(unsigned int addr);
    void updateIR(T data);
    void updateDR(T data);
    T debRdCmdDR(unsigned int addr); //! Read mnemonic
    T shiftDR(unsigned int addr);
    void debRd1Print(unsigned int addr, T data);
    void debRd2Print(T data);
    
    /*!------------------------------------------------------------------------
    -- Constants/Variables/Functions
    -------------------------------------------------------------------------*/
    static const unsigned int  m_txn_data_size = BYTESPERMEMLINE;
    unsigned int currentAddress_v;
};
#endif
