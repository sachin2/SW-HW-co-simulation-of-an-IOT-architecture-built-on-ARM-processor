#include "DebuggerFunc.h"

using namespace sc_core;
using namespace sc_dt;
using namespace std;

// Constructor
template<class T, class U>
DebuggerFunc<T,U>::DebuggerFunc(sc_module_name nm, sc_event& mTestDatRdEv) : sc_module(nm), mTestDatRdEv_e(mTestDatRdEv)
{
  SC_HAS_PROCESS(DebuggerFunc); // because no SC_CTOR()
  cout << left << setw(30) << "ConstructorCPP - DebuggerFunc:   "
       << left << setw(36) << name()
       << endl;
    
  SC_THREAD(getDatThread);
  SC_THREAD(debDRThread);
  
  //! binds the fifo to the export
  tdo_i.bind(tdo_s);
}
//Methods/Threads

//! Definition: Gets read data from MasterBPI (and so from the memory).
template<class T, class U>
void DebuggerFunc<T,U>::getDatThread(void)
{
  while(true)
  {
    // wait on the information, that the read data is available
    wait(mTestDatRdEv_e);
    // notify, that that the data is available - captured by the algorithm
    readMemRdy_e.notify(SC_ZERO_TIME);
  }
}

template<class T, class U>
void DebuggerFunc<T,U>::debDRThread(void)
{ 
  
  //unsigned int addr_v;
  T dataDR_v;
  T dataIR_v;
  sc_bv<16> tms_r;
  
  while(true)
  { 
	enum states {RESET,IDLE,SCAN_DR,SCAN_IR,CAPTURE_DR,SHIFT_DR,EXIT1_DR,PAUSE_DR,EXIT2_DR,UPDATE_DR,
		     CAPTURE_IR,SHIFT_IR,EXIT1_IR,PAUSE_IR,EXIT2_IR,UPDATE_IR};
	char state = RESET;
	
	cout << left  << setw(3)  << " @ "
             << right << setw(9)  << sc_time_stamp() 
             << endl;   
	tms_r[0]= 0; //IDLE
	tms_r[1]= 1; //ScanDR
	tms_r[2]= 1; //ScanIR
	tms_r[3]= 0; //CaptureIR
	tms_r[4]= 0; //ShiftIR
	tms_r[5]= 1; //Exit1IR
	tms_r[6]= 1; //UpdateIR
	tms_r[7]= 0; //IDLE
	tms_r[8]= 0; //IDLE
	tms_r[9]= 1; //ScanDR
	tms_r[10]= 0; //CaptureDR
	tms_r[11]= 0; //ShiftDR
	tms_r[12]= 1; //Exit1DR
	tms_r[13]= 1; //UpdateDR
	tms_r[14]= 0; //IDLE
	tms_r[15]= 0; //IDLE
	cout << left  << setw(3)  << " @ "
             << right << setw(9)  << sc_time_stamp() 
             << endl;
        
	dataDR_v = (T)0xDEADBEEF;
	//BYPASS
	//dataIR_v = (T)0xFF;
	//PRELOAD
	//dataIR_v = (T)0xAA;
	//SAMPLE
	//dataIR_v = (T)0xBB;
	//INTEST
	//dataIR_v = (T)0xCC;
	//EXTEST
	//dataIR_v = (T)0xDD;
	//SCAN
	dataIR_v = (T)0xF0;
	//IDCODE
	//dataIR_v = (T)0x00;
	
	unsigned int slaveID = 0x0;
	write_o->write(tlm::TLM_IGNORE_COMMAND);
        wait(cpuSleep);
	for(int i=0; i<16; ++i)
	{
	 cout << " " << endl;
	 cout << " " << endl;
	 cout << "Next FSM State: " << endl;
		if (tms_r[i] == 1)                 // TMS is high
		{ 
			switch (state) 
			{
				case RESET:
					state = RESET;
					cout << "RESET" << endl;
					break;        
				case IDLE:
					state = SCAN_DR;
					cout << "SCAN_DR" << endl;
					break; 
				case SCAN_DR:
					state = SCAN_IR;
					cout << "SCAN_IR" << endl;
					break; 
				case SCAN_IR:
					state = RESET;
					cout << "RESET" << endl;
					break;
				case CAPTURE_DR:
					state = EXIT1_DR;
					cout << "EXIT1_DR" << endl;
					break;
				case SHIFT_DR:
					state = EXIT1_DR;
					cout << "EXIT1_DR" << endl;
					break;
				case EXIT1_DR:
					state = UPDATE_DR;
					cout << "UPDATE_DR" << endl;
				        cout << "------------------------------------------------------" << endl;
				        cout << "... Update to tdo from Data Register ...." << endl;
				        cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
				        cout << " " << endl;
				        tdi_o = bufferDR_r; 
				        updateDR(bufferDR_r);
					break;
				case PAUSE_DR:
					state = EXIT2_DR;
					cout << "EXIT2_DR" << endl;
					break;
				case EXIT2_DR:
					state = UPDATE_DR;
					cout << "UPDATE_DR" << endl;
				        cout << "------------------------------------------------------" << endl;
				        cout << "... Update to tdo from Data Register ...." << endl;
				        cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
				        cout << " " << endl;
				        tdi_o = bufferDR_r; 
				        updateDR(bufferDR_r);
					break;
				case UPDATE_DR:
					state = IDLE;
					cout << "IDLE" << endl;
					break;
				case CAPTURE_IR:
					state = EXIT1_IR;
					cout << "EXIT1_IR" << endl;
					break;
				case SHIFT_IR:
					state = EXIT1_IR;
					cout << "EXIT1_IR" << endl;
					break;
				case EXIT1_IR:
					state = UPDATE_IR;
					cout << "UPDATE_IR" << endl;
				        cout << "------------------------------------------------------" << endl;
				        cout << "... Update to tdo from Instruction Register ...." << endl;
				        cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
				        cout << " " << endl;
				        tdi_o = bufferIR_r; 
				        updateIR(bufferIR_r);
				        break;
				case PAUSE_IR:
					state = EXIT2_IR;
					cout << "EXIT2_IR" << endl;
					break;
				case EXIT2_IR:
					state = UPDATE_IR;
					cout << "UPDATE_IR" << endl;
				        cout << "------------------------------------------------------" << endl;
				        cout << "... Update to tdo from Instruction Register ...." << endl;
				        cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
				        cout << " " << endl;
				        tdi_o = bufferIR_r; 
				        updateIR(bufferIR_r);
					break;
				case UPDATE_IR:
					state = IDLE;
					cout << "IDLE" << endl;
					break;	
			}
		} 
		else                                 // TMS is low
		{ 
			switch (state) 
			{
				case RESET:
					state = IDLE;
					cout << "IDLE" << endl;
					break;
				case IDLE:
					state = IDLE;
					cout << "IDLE" << endl;
					break;
				case SCAN_DR:
					state = CAPTURE_DR;
					cout << "CAPTURE_DR" << endl;
					cout << "------------------------------------------------------" << endl;
					cout << "... Capture to Data register  ...." << endl;
				        cout << "------------------------------------------------------" << endl;
		                        cout << " " << endl;
					cout << " " << endl;
					// Data Register Capture
					slaveID = 0x1; // chose the slave ID, Register
					debWrAndPrintDR( (slaveID << 28), dataDR_v); // tdi_s, Capture mnemonic
					wait(cpuSleep);
					break;
				case SCAN_IR:
					state = CAPTURE_IR;
					cout << "CAPTURE_IR" << endl;
				        cout << "------------------------------------------------------" << endl;
				        cout << "... Capture to Instruction register  ...." << endl;
				        cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
				        cout << " " << endl;
				        // Instruction Register Capture
				        slaveID = 0x0; // chose the slave ID, Register
				        debWrAndPrintDR( (slaveID << 28), dataIR_v); // instrW_s, Capture mnemonic
				        wait(cpuSleep);
					break;
				case CAPTURE_DR:
					state = SHIFT_DR;
					cout << "SHIFT_DR" << endl;
				        cout << "------------------------------------------------------" << endl;
				        cout << "... Shift Data register  ...." << endl;
				        cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
				        cout << " " << endl;
				        // Data Register Shift
				        slaveID = 0x1; // chose the slave ID, Register
				        bufferDR_r = shiftDR( (slaveID << 28)); // Capture mnemonic
				        wait(cpuSleep);
					break;
				case SHIFT_DR:
					state = SHIFT_DR;
					cout << "SHIFT_DR" << endl;
				        cout << "------------------------------------------------------" << endl;
				        cout << "... Shift Data register  ...." << endl;
				        cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
				        cout << " " << endl;
				        // Data Register Shift
				        slaveID = 0x1; // chose the slave ID, Register
				        bufferDR_r = shiftDR( (slaveID << 28)); // Capture mnemonic
				        wait(cpuSleep);
					break;
				case EXIT1_DR:
					state = PAUSE_DR;
					cout << "PAUSE_DR" << endl;
					break;
				case PAUSE_DR:
					state = PAUSE_DR;
					cout << "PAUSE_DR" << endl;
					break;
				case EXIT2_DR:
					state = SHIFT_DR;
					cout << "SHIFT_DR" << endl;
				        cout << "------------------------------------------------------" << endl;
				        cout << "... Shift Data register  ...." << endl;
				        cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
				        cout << " " << endl;
				        // Data Register Shift
				        slaveID = 0x1; // chose the slave ID, Register
				        bufferDR_r = shiftDR( (slaveID << 28)); // Capture mnemonic
				        wait(cpuSleep);
					break;
				case UPDATE_DR:
					state = IDLE;
					cout << "IDLE" << endl;
					break;
				case CAPTURE_IR:
					state = SHIFT_IR;
					cout << "SHIFT_IR" << endl;
					cout << "------------------------------------------------------" << endl;
					cout << "... Shift Instruction register  ...." << endl;
					cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
					cout << " " << endl;
					// Instruction Register Shift
					slaveID = 0x0; // chose the slave ID, Register
					bufferIR_r = shiftDR( (slaveID << 28)); // Capture mnemonic 
					wait(cpuSleep); 
					break;
				case SHIFT_IR:
					state = SHIFT_IR;
					cout << "SHIFT_IR" << endl;
					cout << "------------------------------------------------------" << endl;
					cout << "... Shift Instruction register  ...." << endl;
					cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
					cout << " " << endl;
					// Instruction Register Shift
					slaveID = 0x0; // chose the slave ID, Register
					bufferIR_r = shiftDR( (slaveID << 28)); // Capture mnemonic 
					wait(cpuSleep); 
					break;
				case EXIT1_IR:
					state = PAUSE_IR;
					cout << "PAUSE_IR" << endl;
					break;
				case PAUSE_IR:
					state = PAUSE_IR;
					cout << "PAUSE_IR" << endl;
					break;
				case EXIT2_IR:
					state = SHIFT_IR;
					cout << "SHIFT_IR" << endl;
					cout << "------------------------------------------------------" << endl;
					cout << "... Shift Instruction register  ...." << endl;
					cout << "------------------------------------------------------" << endl;
					cout << " " << endl;
					cout << " " << endl;
					// Instruction Register Shift
					slaveID = 0x0; // chose the slave ID, Register
					bufferIR_r = shiftDR( (slaveID << 28)); // Capture mnemonic 
					wait(cpuSleep);              
					break;
				case UPDATE_IR:
					state = IDLE;
					cout << "IDLE" << endl;
					break;
			}
		}
	}//for
	
  cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
  cout << "xx End of Simulation!" << endl;
  cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
  sc_stop();
	
  }//while
}


template<class T, class U>
void DebuggerFunc<T,U>::debWr1PrintDR(unsigned int addr, T data)
{
  sc_bv<32> binVal(data);
  cout << left  << setw(11)  << "DebuggerFunc "
       << left  << setw(20) << "- Captures: "
       << left  << setw(10)  << ", tdi"
       << right << setw(10)  << binVal.to_string(SC_BIN)
       << endl;
}


template<class T, class U>
void DebuggerFunc<T,U>::debWrDR(unsigned int addr, T data)
{
  //! Writes the generated data to the BPI - to send it to the slave.
  tdi_o->write(data);
  //! Write the corresponding address.
  address_o->write(addr);
  //! send the WRITE command to the BPI.
  write_o->write(tlm::TLM_WRITE_COMMAND);
  //! Wait for the write delay.
  wait(rwAccess_t + rwWriteDelay_t);
  //! Remove the WRITE command.
  write_o->write(tlm::TLM_IGNORE_COMMAND);
}

template<class T, class U>
void DebuggerFunc<T,U>::debWrAndPrintDR(unsigned int addr, T data)
{
  debWr1PrintDR(addr, data);
  debWrDR(addr, data);
  wait(debOp2Op);
}

template<class T, class U>
T DebuggerFunc<T,U>::debRdCmdDR(unsigned int addr)
{
  // write the address to the BPI for the data to be read
  address_o->write(addr);
  // send the READ command to the BPI
  write_o->write(tlm::TLM_READ_COMMAND);
  // wait for data from BPI - Bus - Memory
  wait(readMemRdy_e);
  // wait for the read delay
  wait(rwAccess_t + rwReadDelay_t);
  // remove the READ command
  write_o->write(tlm::TLM_IGNORE_COMMAND);
  // return the read data to the algorithm
  return tdo_s.read();
}


template<class T, class U>
T DebuggerFunc<T,U>::shiftDR(unsigned int addr)
{
  T data;
  
  data = debRdCmdDR(addr);
  debRd1Print(addr, data);
  wait(debOp2Op);
  
  return data;
}


template<class T, class U>
void DebuggerFunc<T,U>::updateDR(T data)
{
  
  debRd2Print(data);
  wait(debOp2Op);
  
}

template<class T, class U>
void DebuggerFunc<T,U>::updateIR(T data)
{
  
  debRd2Print(data);
  wait(debOp2Op);
  
  cout << left  << setw(11) << "Instr Register Decodes"
       << endl;
  if(data == (T)0xFF)
  {
    cout << left << setw(20) << "BYPASS "
         << endl;
  }
  else if(data == (T)0xAA)
  {
    cout << left << setw(20) << "PRELOAD "
         << endl;
  }
  else if(data == (T)0xBB)
  {
    cout << left << setw(20) << "SAMPLE "
         << endl;
  }
  else if(data == (T)0xCC)
  {
    cout << left << setw(20) << "INTEST "
         << endl;
  }
  else if(data == (T)0xDD)
  {
    cout << left << setw(20) << "EXTEST "
         << endl;
  }
  else if(data == (T)0xF0)
  {
    cout << left << setw(20) << "SCAN "
         << endl;
  }
  else
  {
    cout << left << setw(20) << "IDCODE "
         << endl;
  }
  
}


template<class T, class U>
void DebuggerFunc<T,U>::debRd1Print(unsigned int addr, T data)
{
  sc_bv<32> binVal(data);
  cout << left  << setw(11) << "DebuggerFunc "
       << left  << setw(20) << "- shifts: "
       << left  << setw(7)  << ", tdo "
       << right << binVal.to_string(SC_BIN)
       << endl;
}

template<class T, class U>
void DebuggerFunc<T,U>::debRd2Print(T data)
{
  sc_bv<32> binVal(data);
  cout << left  << setw(11) << "DebuggerFunc "
       << left  << setw(20) << "- updates: "
       << left  << setw(7)  << ", tdo "
       << right << binVal.to_string(SC_BIN)
       << endl;
}

