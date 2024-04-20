SystemC is a library in C++:
<img width="255" alt="image" src="https://github.com/sachin2/SW-HW-co-simulation-of-an-IOT-architecture-built-on-ARM-processor/assets/12724755/cd5db0ad-0ed3-477c-96dd-e24c482a6a45">
More info about SystemC can be found here: systemc.org![image](https://github.com/sachin2/SW-HW-co-simulation-of-an-IOT-architecture-built-on-ARM-processor/assets/12724755/3e8203bd-c2dc-4f51-a05e-5b57e16d7658)

**A few points on TLM2.0 in our implementation:**
The TLM-2.0 core interfaces enable communication between initiators and targets in a simulation. Initiators create and pass transactions to targets, with interconnects handling transaction flow. Initiators (e.g., ARM) perform write or read operations on targets (e.g., Memory, Registers, ALU). Interconnects like arbiters and routers manage transaction routing without initiating or being the final destination. The TLM-2.0 blocking transport interface is designed to facilitate the loosely-timed coding style, specifically catering to scenarios where an initiator aims to complete a transaction with a target within a single function call. This style focuses on timing points marking the transaction’s initiation and conclusion.
More info about TLM2.0 can be found here: 1 (Accellera.org)![image](https://github.com/sachin2/SW-HW-co-simulation-of-an-IOT-architecture-built-on-ARM-processor/assets/12724755/ddc95e8b-097b-417a-b5e7-e105c934bfbf)

**Debugger as TAP-Controller:**
<img width="417" alt="image" src="https://github.com/sachin2/SW-HW-co-simulation-of-an-IOT-architecture-built-on-ARM-processor/assets/12724755/eb75f58c-f8e8-4527-a87a-8841389373ab">
The Debugger, acting as the master entity within the Joint Test Action Group (JTAG) ecosystem, orchestrates operations using the Transaction-Level Modeling (TLM) 2.0 architecture. Employing terms such as Test Mode Select (TMS), Test Data Input (TDI), and Test Data Output (TDO), the Debugger interfaces with the Instruction Register and Data Register, manipulating their states and controlling the flow of test data. TAP State Machine: The TAP controller employs a Finite State Machine (FSM) as defined by the IEEE 1149.1 standard. The TAP state machine comprises states such as Test-Logic-Reset (TLR), Run-Test/Idle (RTI), and
Capture-DR (CDR).

**Scan Chain Design**:
To design a scan chain, you need to identify the flip-flops or latches that you want to include in the chain, and connect them in a way that allows you to shift data in and out of the chain. You also need to provide a scan enable signal that switches the circuit between normal mode and scan mode, and a scan clock signal that controls the timing of the data shifting. You can use a multiplexer or a tri-state buffer to switch between the normal and scan inputs and outputs of the flip-flops or latches. You can also use a scan register to store the scan enable signal and make it part of the chain.

**ARM ALU block:** (based on RISC-V)
![image](https://github.com/sachin2/SW-HW-co-simulation-of-an-IOT-architecture-built-on-ARM-processor/assets/12724755/fc9f812e-99da-4841-afdb-384a9ef540aa)
The above-depicted model will be actively implemented for the purpose of executing RISC-V Instructions via a 32-bit ALU. As mentioned in the above Functional block diagram, the ALU will carry out " Arithmetic and Logic" functions effectively. Instructions are determined by the Program Counter's address, passed to Instruction memory for reading and decoding. Operands are extracted from the register group based on the instruction's register number. Operand usage varies, serving as computational memory addresses, and calculating arithmetic results.

**Note:** _This complete project was intended to Simulate the performance of a ARM processor in an IOT architecture._
