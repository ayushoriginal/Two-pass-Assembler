# Two-pass-Assembler
This is my design of a 32-bit ISA and implementation a two pass assembler using C code to demonstrate how assembly language computation occurs with the help of different sets of instructions and different addressing modes defined in ISA. It converted Assembly Language code into Machine Language code and then using the machine language code generated executed the program. The GNUPro process of converting source file to executable file is shown below.

![0](http://i.imgur.com/BzQ3ybQ.gif)

# Assembly Language
An assembly language is a low-level programming language for a computer, or other programmable device, in which there is a very strong (generally one-to-one) correspondence between the language and the architecture's machine code instructions. Each assembly language is specific to a particular computer architecture, in contrast to most high-level programming languages, which are generally portable across multiple architectures, but require interpreting or compiling.
Assembly language uses a mnemonic to represent each low-level machine operation or opcode. Some op-codes require one or more operands as part of the instruction.

# Assembler
Assembly language is converted into executable machine code by a utility program referred to as an assembler; the conversion process is referred to as assembly, or assembling the code.

![1](http://i.imgur.com/HPjwJOQ.jpg)

An assembler is a translator that translates an assembler program into a conventional machine language program. Basically, the assembler goes through the program one line at a time, and generates machine code for that instruction. Then the assembler proceeds to the next instruction. In this way, the entire machine code program is created.

![2](http://i.imgur.com/Mqntp81.jpg)

# Assembler Directives
- Assembler directives are pseudo instructions
    - They will not be translated into machine instructions.
    - They only provide instruction/direction/information to the assembler.
- Basic assembler directives :
    -   **START :**  Specify name and starting address for the program
    -   **END :**    Indicate the end of the source program.
    -   **EQU :**   The EQU directive is used to replace a number by a symbol. For example:                                         MAXIMUM EQU 99.
After using this directive, every appearance of the label “MAXIMUM” in the program will be interpreted by the assembler as the number 99 (MAXIMUM = 99). Symbols may be defined this way only once in the program. The EQU directive is mostly used at the beginning of the program.

# Three Main Data Structures
- Operation Code Table (OPTAB)

- Location Counter (LOCCTR)

- Symbol Table (SYMTAB)

![3](http://i.imgur.com/NEINDaG.jpg)
(Machine Dependent Assembler)

# Instruction formats

- Addressing modes
·        Direct addressing (address of operand is given in instruction itself)
·        Register addressing (one of the operand is general purpose register)
·        Register indirect addressing (address of operand is specified by register pair)
·        Immediate addressing (operand  - data is specified in the instruction itself)
·       Implicit addressing (mostly the operation operates on the contents of accumulator)

- Program relocation
·        It is desirable to load and run several programs and resources at the same time
·        The system must be able to load programs into memory wherever there is room
·        The exact starting address of the program is not known until load time.
·        The assembler can identify (for the loader) those parts of the program that need modification.
·        An object program that contains this type of modification information necessary to perform modification is called a re-locatable program.




# Literal
It is convenient for the programmer to be able to write the value of a constant operand as a part of the instruction that uses it. Such an operand is called a literal.
In this assembler language notation, a literal is identified with the prefix ‘=’, which is followed by a specification of the literal value.

## The difference between literal operands and immediate operands
- for literal operand we use ‘=’ as prefix, and with immediate operand we use ‘#’ as prefix
- During immediate addressing, the operand value is assembled as part of the machine instruction, and there is no memory reference.
- With a literal, the assembler generates the specified value as a constant at some other memory location.

# One-pass assemblers
A one pass assembler passes over the source file exactly once, in the same pass collecting the labels, resolving future references and doing the actual assembly. The difficult part is to resolve future label references (the problem of forward referencing) and assemble code in one pass

![4](http://i.imgur.com/oyn0JCi.png)

## Forward reference in one pass assembler
- Omits the operand address if the symbol has not yet been defined
- Enters this undefined symbol into SYMTAB and indicates that it is undefined
- Adds the address of this operand address to a list of forward references associated with the SYMTAB entry
- When the definition for the symbol is encountered, scans the reference list and inserts the address.
- At the end of the program, reports the error if there are still SYMTAB entries indicated undefined symbols.

# Two-pass assemblers
The two pass assembler performs two passes over the source program. 

In the first pass, it reads the entire source program, looking only for label definitions. All the labels are collected, assigned address, and placed in the symbol table in this pass, no instructions as assembled and at the end the symbol table should contain all the labels defined in the program. To assign address to labels, the assembles maintains a Location Counter (LC).


In the second pass the instructions are again read and are assembled using the symbol table.
Basically, the assembler goes through the program one line at a time, and generates machine code for that instruction. Then the assembler proceeds to the next instruction. In this way, the entire machine code program is created. For most instructions this process works fine, for example for instructions that only reference registers, the assembler can compute the machine code easily, since the assembler knows where the registers are.

![5](http://i.imgur.com/XIway7g.jpg)

## Difference between One Pass and Two Pass Assemblers
The difference between one pass and two pass assemblers are:-

A one pass assembler passes over the source file exactly once, in the same pass collecting the labels, resolving future references and doing the actual assembly. The difficult part is to resolve future label references (the problem of forward referencing) and assemble code in one pass. The one pass assembler prepares an intermediate file, which is used as input by the two pass assembler.


A two pass assembler does two passes over the source file (the second pass can be over an intermediate file generated in the first pass of the assembler). In the first pass all it does is looks for label definitions and introduces them in the symbol table (a dynamic table which includes the label name and address for each label in the source program). In the second pass, after the symbol table is complete, it does the actual assembly by translating the operations into machine codes and so on.
