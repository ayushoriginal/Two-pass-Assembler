# Two-pass-Assembler
This is my design f a 32-bit ISA and implementation a two pass assembler using C code to demonstrate how assembly language computation occurs with the help of different sets of instructions and different addressing modes defined in ISA. It converted Assembly Language code into Machine Language code and then using the machine language code generated executed the program. 

![0](http://i.imgur.com/BzQ3ybQ.gif)

# Assembly Language
An assembly language is a low-level programming language for a computer, or other programmable device, in which there is a very strong (generally one-to-one) correspondence between the language and the architecture's machine code instructions. Each assembly language is specific to a particular computer architecture, in contrast to most high-level programming languages, which are generally portable across multiple architectures, but require interpreting or compiling.
Assembly language uses a mnemonic to represent each low-level machine operation or opcode. Some op-codes require one or more operands as part of the instruction.

# Assembler
Assembly language is converted into executable machine code by a utility program referred to as an assembler; the conversion process is referred to as assembly, or assembling the code.

![1](http://i.imgur.com/HPjwJOQ.jpg)

An assembler is a translator that translates an assembler program into a conventional machine language program. Basically, the assembler goes through the program one line at a time, and generates machine code for that instruction. Then the assembler proceeds to the next instruction. In this way, the entire machine code program is created.

![2](http://i.imgur.com/Mqntp81.jpg)

#Assembler Directives
•         Assembler directives are pseudo instructions
–        They will not be translated into machine instructions.
–        They only provide instruction/direction/information to the assembler.
•         Basic assembler directives :
–        START :
•       Specify name and starting address for the program
–        END :
•       Indicate the end of the source program.
–        EQU
•       The EQU directive is used to replace a number by a symbol. For example:
                                              MAXIMUM EQU 99
After using this directive, every appearance of the label “MAXIMUM” in the program will be interpreted by the assembler as the number 99 (MAXIMUM = 99). Symbols may be defined this way only once in the program. The EQU directive is mostly used at the beginning of the program.

#Three Main Data Structures
•         Operation Code Table (OPTAB)
•         Location Counter (LOCCTR)
•         Symbol Table (SYMTAB)

##OPTAB (operation code table)
•         Content
–        The fields of this table are class, mnemonics and op-codes.
–        The class defines whether the statement is imperative statement, declarative statement or assembler directive.
–        The mapping between mnemonic and machine code. Also include the instruction format, available addressing modes, and length information.
•         Characteristic
–        Static table. The content will never change.
•         Implementation
–        Array or hash table. Because the content will never change, we can optimize its search speed (a good search speed is required because for every source line input, assembler has to search the OPCODE table).
•         In pass 1, OPTAB is used to look up and validate mnemonics in the source program.
•         In pass 2, OPTAB is used to translate mnemonics to machine instructions.
##Location Counter (LOCCTR)
•         This variable helps in the assignment of addresses.
•         It is initialized to the beginning address specified in the START statement.
•         After each source statement is processed, the length of the assembled instruction (in words) and data area to be generated is added to LOCCTR.
Thus, when we reach a label in the source program, the current value of LOCCTR gives the address to be associated with that label.
Alternatively, LOCCTR can be defined as a variable that contains the address of the current instruction that the assembler is processing. It is used to assign values to the labels during pass 1.

##Symbol Table (SYMTAB)
•         Content
–        Include the label name and value (address) for each label in the source program.
–        Include type and length information (e.g., int64)
–        With flag to indicate errors (e.g., a symbol defined in two places)
•         Characteristic
–        Created during the analysis phase.
–        Generated maintained and used by the assembler
–        Dynamic table (I.e., symbols may be inserted, deleted, or searched in the table)
•         Implementation
–        Before a label is inserted into the table the label has to be searched in the table, whether the label to be inserted already exist or not.
–        Hash table can be used to speed up search
–        Because of frequent searches and similarity in label names (e.g., LOOP1, LOOP2), the selected hash function must perform well with such non-random keys.

#Machine-Dependent Assembler Features
·       Instruction formats and addressing modes
·       Program Relocation

Instruction formats
Addressing modes
·        Direct addressing (address of operand is given in instruction itself)
·        Register addressing (one of the operand is general purpose register)
·        Register indirect addressing (address of operand is specified by register pair)
·        Immediate addressing (operand  - data is specified in the instruction itself)
·       Implicit addressing (mostly the operation operates on the contents of accumulator)
Program relocation
·        It is desirable to load and run several programs and resources at the same time
·        The system must be able to load programs into memory wherever there is room
·        The exact starting address of the program is not known until load time.
·        The assembler can identify (for the loader) those parts of the program that need modification.
·        An object program that contains this type of modification information necessary to perform modification is called a re-locatable program.

Machine - Independent Assembler Features


Literal
It is convenient for the programmer to be able to write the value of a constant operand as a part of the instruction that uses it. Such an operand is called a literal.
In this assembler language notation, a literal is identified with the prefix ‘=’, which is followed by a specification of the literal value.
The difference between literal operands and immediate operands
◦      for literal operand we use ‘=’ as prefix, and with immediate operand we use ‘#’ as prefix
◦      During immediate addressing, the operand value is assembled as part of the machine instruction, and there is no memory reference.
◦      With a literal, the assembler generates the specified value as a constant at some other memory location.
Symbol defining statements
—  Allow the programmer to define symbols and specify their values.
Assembler directive EQU
Expressions
Allow arithmetic expressions formed
◦      Using the operators +, -, ×, /.
Division is usually defined to produce an integer result.
Expression may be constants, user-defined symbols, or special terms.
Program blocks
◦      Refer to segments of code that are rearranged within a single object program unit.
Control sections
◦      Refer to segments of code that are translated into independent object program units.d

3One-pass assemblers
A one pass assembler passes over the source file exactly once, in the same pass collecting the labels, resolving future references and doing the actual assembly. The difficult part is to resolve future label references (the problem of forward referencing) and assemble code in one pass
.

 Forward reference in one pass assembler
·         Omits the operand address if the symbol has not yet been defined
·         Enters this undefined symbol into SYMTAB and indicates that it is undefined
·         Adds the address of this operand address to a list of forward references associated with the SYMTAB entry
·         When the definition for the symbol is encountered, scans the reference list and inserts the address.
·         At the end of the program, reports the error if there are still SYMTAB entries indicated undefined symbols.

Two-pass assemblers
The two pass assembler performs two passes over the source program. In the first pass, it reads the entire source program, looking only for label definitions. All the labels are collected, assigned address, and placed in the symbol table in this pass, no instructions as assembled and at the end the symbol table should contain all the labels defined in the program. To assign address to labels, the assembles maintains a Location Counter (LC).
In the second pass the instructions are again read and are assembled using the symbol table.
Basically, the assembler goes through the program one line at a time, and generates machine code for that instruction. Then the assembler proceeds to the next instruction. In this way, the entire machine code program is created. For most instructions this process works fine, for example for instructions that only reference registers, the assembler can compute the machine code easily, since the assembler knows where the registers are.
Consider an assembler instruction like the following
          JMP  LATER
          ...
          ...
LATER:
This is known as a forward reference. If the assembler is processing the file one line at a time, then it doesn't know where LATER is when it first encounters the jump instruction. So, it doesn't know if the jump is a short jump, a near jump or a far jump. There is a large difference amongst these instructions. They are 2, 3, and 5 bytes long respectively. The assembler would have to guess how far away the instruction is in order to generate the correct instruction. If the assembler guesses wrong, then the addresses for all other labels later in the program woulds be wrong, and the code would have to be regenerated. Or, the assembler could alway choose the worst case. But this would mean generating inefficiency in the program, since all jumps would be considered far jumps and would be 5 bytes long, where actually most jumps are short jumps, which are only 2 bytes long.
So, what is to be done to allow the assembler to generate the correct instruction? Answer: scan the code twice. The first time, just count how long the machine code instructions will be, just to find out the addresses of all the labels. Also, create a table that has a list of all the addresses and where they will be in the program. This table is known as the symbol table. On the second scan, generate the machine code, and use the symbol table to determine how far away jump labels are, and to generate the most efficient instruction.
This is known as a two-pass assembler. Each pass scans the program, the first pass generates the symbol table and the second pass generates the machine code.


Difference between One Pass and Two Pass Assemblers
The difference between one pass and two pass assemblers are:-
A one pass assembler passes over the source file exactly once, in the same pass collecting the labels, resolving future references and doing the actual assembly. The difficult part is to resolve future label references (the problem of forward referencing) and assemble code in one pass. The one pass assembler prepares an intermediate file, which is used as input by the two pass assembler.
A two pass assembler does two passes over the source file (the second pass can be over an intermediate file generated in the first pass of the assembler). In the first pass all it does is looks for label definitions and introduces them in the symbol table (a dynamic table which includes the label name and address for each label in the source program). In the second pass, after the symbol table is complete, it does the actual assembly by translating the operations into machine codes and so on.
