#include "stim.h"
#include "ProcPack.h"
using namespace sc_core;
using namespace sc_dt;
using namespace std;

/*-----------------------------------------------------------------------------
-- Constructor
-----------------------------------------------------------------------------*/
Stim::Stim(sc_module_name nm)
  : sc_module(nm)
  
{
  cout << "ConstructorCPP - Stim: " << name() << endl;
  SC_THREAD(StimThread);
}
void Stim::StimThread(void)
{
    //Test 1
    data1_o.write(0x00011122);
    wait(100,SC_NS);
    cout << "Data 1 = 0x00011122";
    cout << endl;
    
    data2_o.write(0x0033004);
    wait(12,SC_NS);
    cout << "data 2 = 0x0033004";
    cout << endl;
    func4_o.write(0);
    wait(13 ,SC_NS);
    cout << "Opcode = 0........AND"<<func4_o.read();
    cout << endl;
            
    wait(150,SC_NS);
    cout << "Result...";
    cout << endl;
    wait(24,SC_NS);
    cout << "Flag...";
    cout << endl;
    wait(100,SC_NS);
    
    //Test 2
    data1_o.write(0x00055566);
    wait(100,SC_NS);
    cout << "Data 1 = 0x00055566";
    cout << endl;
    data2_o.write(0x00110070);
    wait(12,SC_NS);
    cout << "data 2 = 0x00110070";
    cout << endl;
    func4_o.write(0);
    wait(13 ,SC_NS);
    cout << "Opcode = 0........AND"<<func4_o.read();
    cout << endl;
            
    wait(112,SC_NS);
    cout << "Result ......";
    cout << endl;
    wait(24,SC_NS);
    cout << "Flag ......";
    cout << endl;
    wait(100,SC_NS);
    
    //Test 3
    /*data1_o.write(0x00077799);
    wait(100,SC_NS);
    cout << "Data 1 = 0x00077799";
    cout << endl;
    
    data2_o.write(0x00888070);
    wait(12,SC_NS);
    cout << "data 2 = 0x00888070";
    cout << endl;
    func4_o.write(ADD);
    wait(13 ,SC_NS);
    cout << "Opcode = 4........"<<func4_o.read();
    cout << endl;
            
    wait(112,SC_NS);
    cout << "Result = 0x00000001";
    cout << endl;
    wait(24,SC_NS);
    cout << "Flag = 0x003";
    cout << endl;
    wait(100,SC_NS);
    
    //Test 4
    data1_o.write(0x000ADB00);
    wait(100,SC_NS);
    cout << "Data 1 = 0x000ADB00";
    cout << endl;
    
    data2_o.write(0x00888070);
    wait(12,SC_NS);
    cout << "data 2 = 0x00888070";
    cout << endl;
    func4_o.write(SUBTRACT);
    wait(13 ,SC_NS);
    cout << "Opcode = 4........"<<func4_o.read();
    cout << endl;
            
    wait(112,SC_NS);
    cout << "Result = 0x00000001";
    cout << endl;
    wait(24,SC_NS);
    cout << "Flag = 0x003";
    cout << endl;*/
    wait(100,SC_NS);
    
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
    cout << "xx End of simulation! " << endl;
    cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
    sc_stop();
}
