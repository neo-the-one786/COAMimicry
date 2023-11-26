### README for the provided C code

#### Overview:

This C code simulates a simple virtual machine with a limited instruction set architecture (ISA). The program performs basic arithmetic, logical operations, and data transfer operations using a custom-defined instruction format. It emulates an instruction fetch-decode-execute cycle within a simulated memory space and registers.

#### Code Components:

- **Enums and Structs:**
  - `Opcode`: Defines operation codes for arithmetic, logical, and data transfer instructions.
  - `Register`: Enumerates available registers.
  - `InstructionType`: Specifies instruction types (arithmetic/logical or data transfer).
  - `AddrMode`: Defines addressing modes (register or immediate).
  - `Instruction`: Contains fields to store information about an instruction.

- **Constants and Global Variables:**
  - `WORD_LENGTH`: Defines the length of a word in the simulated memory.
  - `NUM_BLOCKS`: Specifies the number of memory blocks.
  - `memo`: Array simulating memory blocks.
  - `pc`: Program counter.
  - `reg`: Array representing registers.

- **Functions:**
  - `binToDec`: Converts a binary string to a decimal integer.
  - `loadData`: Loads data from a file into memory and registers.
  - `loadProg`: Loads a program into memory.
  - `instructionDecode`: Decodes the instruction at the current program counter.
  - `instructionFetch`: Fetches the instruction pointed by the program counter.
  - `performALUOps`: Performs arithmetic and logical operations based on the instruction and addressing mode.
  - `performMemoOps`: Performs memory operations (load/store) based on the instruction and addressing mode.
  - `instructionExecute`: Executes the decoded instruction.
  - `displayAllRegisterValues`: Displays the values stored in registers.
  - `displayMemory`: Displays the content of the memory blocks.

### Sample `data_input.txt`:
```plaintext
0000000000000101 0000000000000001
0000000000000110 0000000000000010
0000000000000111 0000000000000011
0000000000001000 0000000000000100
0000000000001001 0000000000000101
0000000000001010 0000000000000111
0000000000001011 0000000000001000
0000000000001100 0000000000001001
```
This represents data in the format `<memory_address> <data_value>` separated by spaces.

### Sample `program_input.txt`:
```plaintext
0000010000001010
0000000000001010
```
Each line represents an instruction in the simulated program, formatted as a sequence of bits.

Please replace these placeholder contents with actual data and program instructions relevant to your simulation while ensuring that the format matches the expectations of the virtual machine.

#### How to Run:

1. **Compile the Code:**
   ```bash
   gcc -o virtual_machine virtual_machine.c
   ```

2. **Execute the Program:**
   ```bash
   ./virtual_machine
   ```

#### Sample Input/Output:

- **Input:**
  - Enter data file name: `data_input.txt`
  - Enter program file name: `program_input.txt`

- **Sample Output:**
  - Displays the fetched instructions and their execution.
  - Final register values after executing the program.
  - Contents of the simulated memory.

#### Sample I/O:

```plaintext
Enter data file name: data_input.txt
Enter program file name: program_input.txt

Instruction: <type> <opcode> <addrMode> <operand1> <operand2> <operand3>
...

Register values:
R0: <value>
R1: <value>
...
Memory:
<contents of memory blocks>
```

Replace `<type>`, `<opcode>`, `<addrMode>`, `<operand1>`, `<operand2>`, `<operand3>`, `<value>`, and `<contents of memory blocks>` with the actual values and memory content obtained during execution.

#### Notes:

- Ensure the input files (`data_input.txt` and `program_input.txt`) are properly formatted and accessible.
- The simulated instruction set may have limitations compared to real architectures.

Feel free to adapt this README with more specific information or update it according to the actual functionality and expected input/output of your virtual machine.   
