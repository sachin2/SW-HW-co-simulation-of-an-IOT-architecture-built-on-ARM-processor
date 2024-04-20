#ifndef _ASPROPACK_H_
#define _ASPROPACK_H_

#include <string>
#include "systemc.h"

//#define ADDR_WIDTH        7
//#define RAM_DEPTH         1 << ADDR_WIDTH

// Data Bus
#define BITWIDTH         32
#define BYTESPERMEMLINE   4

// Address Bus
// 1010_1010_1010_1010_1010_1010_1010_1010
#define BUSWIDTHS        32//8    // #address bits
// 1010_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx_xxxx
#define BUSDECODEBITS     4//2    // #upper chip select decode bits
// xxxx_1010_1010_1010_1010_1010_1010_1010
#define BUSMEMINTBITS    28//6    //=32-4, #memory internal address bits
#define BUSTARGETMASK   0xF//0x3    // mask for the upper chip select decode bits
#define BUSDECODEMASK   0x0FFFFFFF//0x3F   // 7 Nibbles, mask for the memory internal address bits
// END: Address Bus

// Memories
#define MEM0WIDTH         4 // bytes, = 32 bits
#define MEM0BYTES        4*1024 // #bytes (or lines? I guess lines of 32 bits)
#define MEM1WIDTH         4 // bytes
#define MEM1BYTES        8*1024
#define MEM2WIDTH         4 // bytes
#define MEM2BYTES        64
#define MEM3WIDTH         4 // bytes
#define MEM3BYTES        64
#define REGIWIDTH         4 // #Bytes for registers in general
#define REG0RESET         0xAABBCCDD
#define REG0ADDRE         0x00000000 // byte address
#define REG0BYTES         4*1
#define REG1RESET         0x55667788
#define REG1ADDRE         0x00000000 // byte address
#define REG1BYTES         4//*3 // up to 3 registers, if the address is higher than 0, the range is needed
#define REGWIDTH         4 
#define GPSBYTES         44
// END: Memories

// Masters
// Words and Bytes in the Masters
#define MAS1WIDTH         4 // data width of master 1
#define MAS2WIDTH         4
// Total number of addresses needed in the masters
#define MAS1ADDRESSES   256 // = 4*64
#define MAS2ADDRESSES   256 // = 4*64
// END: Masters


// Registers
// setmask and clrmask are for rw, r, h, w bit settings
#define GPS_ID_ADDR     0x00000000
#define GPS_ID_RST      0x000000D3
#define GPS_ID_SETMASK  0xFFFFFFFF
#define GPS_ID_CLRMASK  0xFFFFFFFF
#define GPS_RUNCTRL_ADDR     0x00000004
#define GPS_RUNCTRL_RST      0x00000000
#define GPS_RUNCTRL_SETMASK  0xFFFFFFFF
#define GPS_RUNCTRL_CLRMASK  0xFFFFFFFF
#define GPS_TIME_ADDR        0x00000008
#define GPS_TIME_RST         0x00000000
#define GPS_TIME_SETMASK     0xFFFFFFFF
#define GPS_TIME_CLRMASK     0xFFFFFFFF
#define GPS_TIMEDEC_ADDR     0x0000000C
#define GPS_TIMEDEC_RST      0x00000000
#define GPS_TIMEDEC_SETMASK  0xFFFFFFFF
#define GPS_TIMEDEC_CLRMASK  0xFFFFFFFF
#define GPS_LAT_ADDR         0x00000010
#define GPS_LAT_RST          0x00000000
#define GPS_LAT_SETMASK      0xFFFFFFFF
#define GPS_LAT_CLRMASK      0xFFFFFFFF
#define GPS_LATDEC_ADDR      0x00000014
#define GPS_LATDEC_RST       0x00000000
#define GPS_LATDEC_SETMASK   0xFFFFFFFF
#define GPS_LATDEC_CLRMASK   0xFFFFFFFF
#define GPS_LONG_ADDR        0x00000018
#define GPS_LONG_RST         0x00000000
#define GPS_LONG_SETMASK     0xFFFFFFFF
#define GPS_LONG_CLRMASK     0xFFFFFFFF
#define GPS_LONGDEC_ADDR     0x0000001C
#define GPS_LONGDEC_RST      0x00000000
#define GPS_LONGDEC_SETMASK  0xFFFFFFFF
#define GPS_LONGDEC_CLRMASK  0xFFFFFFFF
#define GPS_EASTNORTH_ADDR     0x00000020 // reg(7:0) = N|S (ASCII), reg(15:8) = E|W (ASCII)
#define GPS_EASTNORTH_RST      0x00000000
#define GPS_EASTNORTH_SETMASK  0xFFFFFFFF
#define GPS_EASTNORTH_CLRMASK  0xFFFFFFFF
#define GPS_FIX_ADDR         0x00000024
#define GPS_FIX_RST          0x00000000
#define GPS_FIX_SETMASK      0xFFFFFFFF
#define GPS_FIX_CLRMASK      0xFFFFFFFF
#define GPS_NRSAT_ADDR       0x00000028
#define GPS_NRSAT_RST        0x00000000
#define GPS_NRSAT_SETMASK    0xFFFFFFFF
#define GPS_NRSAT_CLRMASK    0xFFFFFFFF

#define CYPHERROT         1

#define DEBUG             0

//#define CPU1PRINTS        1
#define SLAVEBPIPRINTS    0
#define SLAVEFUNPRINTS    0
#define MASTERBPIPRINTS   0
#define MASTERFUNPRINTS   0
#define MASTER1FUNPRINTS  0
#define MASTER2FUNPRINTS  0
#define BUSPRINTS         0
#define CPUPRINTS         1
#define GPSFUNPRINTS      0

const sc_time debOp2Op(10, SC_NS); // time between instructions
const sc_time cpuSleep(100, SC_NS);
const sc_time rwAccess_t(1, SC_NS);
const sc_time rwReadDelay_t(1, SC_NS);
const sc_time rwWriteDelay_t(1, SC_NS);

const sc_time memAccess00_c(10, SC_NS);
const sc_time memAccess01_c(10, SC_NS);
const sc_time memAccess02_c(10, SC_NS);
const sc_time memAccess03_c(10, SC_NS);
const sc_time memDMIInvalidate_c(3, SC_US);
const sc_time quantumKeeperSync_c(1, SC_US);
const sc_time quantumKeeperAdd_c(100, SC_NS);

#endif
