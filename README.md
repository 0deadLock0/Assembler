# Assembler
https://github.com/0deadLock0/Assembler/

DOCUMENTATION::
**************************************************************BRIEF*********************************************************************
Author: Abhimanyu Gupta (guptaabhimanyu23@gmail.com)
Functionality:
	Converts a given assembly code present in some txt file into its adequate machine/object code and writes it down in another txt file
For Purpose: Assembler Project in CSE112-Computer Organization at IIIT-Delhi
Includes:
	driver.cpp - maintains the flow of the Assembler while handling serious errors
	lexer.h - provides functions which defines the lexical structure of program and do tokenisation alongwith basic syntax checking
	parser.h - provides functions which gives meaning to the assembly code and convert it to machine/object code
	symbolTable.h - provides functions which builds the symbol tabel for the provided assmebly code with looking for some context specific errors 
	errorHandler.h - provides functions which handles all the errors encountered along the flow of Assmebler 
	helper.h - provides non assembler specific functions which helps in smoothing of operations needed to be done at various points in the flow of Assembler
	printer.h - provides functions which helps in printing the data recieved and information processed 
****************************************************************************************************************************************

******************************************************RULES AND REGULATION*************************************************************
INSTRUCTION FORMAT:
*Opcode
*Opcode Operand
*Label:
*Label: Opcode
*Label: Opcode Operand
Note- Comments can be placed after end of the instruction in any above format or can even comment whole line without any instruction

CHARACTERS SUPPORTED:
*Alphabets
*Digits (only as Literals)
* '#' (defines start of a comment)
* ':' (defines a label,can't exists seprately)
*Space and Horizontal Tab Space

VARRIABLES/LABEL/OPCODE NAMING RULES:
*Can include alphabets only
*No support for names with digits
*No support for names with ':' and '#'
*No support for names with underscores or any other character which is not support by language(mentioned above)

RECOGNISED OPCODES:
Machine/Object OpCode				Meaning										Assembly Opcode
	0000				Clear accumulator										CLA
	0001				Load into accumulator from address								LAC
	0010				Store accumulator contents into address								SAC
	0011				Add address contents to accumulator contents							ADD
	0100				Subtract address contents from accumulator contents						SUB
	0101				Branch to address if accumulator contains zero							BRZ
	0110				Branch to address if accumulator contains negative value					BRN
	0111				Branch to address if accumulator contains positive value					BRP
	1000				Read from terminal and put in address								INP
	1001				Display value in address on terminal								DSP
	1010				Multiply accumulator and address contents							MUL
	1011				Divide accumulator contents by address content. Quotient in R1 and remainder in R2		DIV
	1100				Stop execution											STP

VARRIABLES USAGE:
*Varriable is assumed to be declared when either INP or SAC is attached to it in the same line for the time
*A same varriable can be used to take inputs multiple time or store value from accumulator (This doesn't count as Multiple Declaration)
*Varriable attached to neither INP or SAC, is conisdered to be Undeclared Varriable
*Forward Referencing of Varriables is not allowed

LABELS USAGE:
*Label is defined when encountered with ':' at the start of a line
*Label can be defined only once in the whole program
*Label can't be defined at any other position in a line
*Forward Referencing of Labels is allowed
*For a branch statement to be valid Label must be defined
*Label found in a branch statement doesn't count in its declaration 

LITERALS USAGE:
*Literals can be used just as normal Numbers
*Floating numbers aren't treated as Literals
*No support for negative literals.
*Literals can't be used with INP, SAC, BRP, BRN, BRZ opcodes
*Literals can be used with DSP, LAC, ADD, SUB, DIV, MUL  

COMMENTS FORMAT:
*Comments starts from the point '#' is encountered and includes the whole line after it
*Comments are sngle line comment only
*No support for multiple line comments

GUIDLINES FOR FILE CONTANING ASSEMBLY CODE:
*File should be a text file with ".txt" extension.
*File name is Case-Insensitive.
*File name can contain any number of  words with any kind of character.
*File should be encoded in ANSI encoding.
*File should be present in the same folder in which program is run.
*When prompted to enter file name, enter it in a single line.
****************************************************************************************************************************************

**********************************************************PROGRAM FLOW******************************************************************
*The Program is distributed among 7 files working together to run the Assembler:
	driver.cpp
	lexer.h
	parser.h
	symbolTable.h
	errorHandler.h
	helper.h
	printer.h
*The program has 2 main dvisions:
	First Pass
	Second Pass

FLOW:
*Input file name is asked (with extension).
*File is read and it is converted into a list of instruction strings.
*First Pass class is called, to check for all Errors and to format instruction strings to actual usable data to write machine/object code.
	=>checks for all possible errors (listed below) and eliminate them/notify the user regarding the same.
	=>generates symbol table, which has refferences to all the symbols and their locations in virtual memory.
	=>provides an opcode table,which has opcode type,operands and location counter info usefull for second pass. 
*Newly generated Symbol and Opcode List are printed on the console, after the first pass.
*Second Pass is called to use these Symbol and Opcode Lists, to generate the corresponding Machine/Object Code file.
	=>genrates a string list of outputs corresponding to each line of instruction.
	=>generates the final output file using the proccessed strings.
*Final output file is generated with the name of file contaning assembly code, current Date/Time and respective Machine/Object Code.

ASSUMTIONS IN THE FLOW:
*each line of input file contains only one instruction
*line may or may not contain any instruction at all
*program starts at location counter "00000001" and runs at max till "01111111"
*varriables/literals starts defining from address "10000001"  and runs at max till "11111111"
*when converting a line to object/machine instruction:
	when no instruction is present, a blank line gets printed
	in case of only label present in the line, even then a blank line is printed
	when a proper instruction is found its equivalent code is printed
*When a single valid standalone opcode is found the remaining bits are filled with "00000000"

****************************************************************************************************************************************

***************************************************ERROR HANDLING******************************************************************
The following Error have been handled:
	*Input/Output file can't be opened
	*Unsupported Character found
	*Invalid Varriable/Label/Opcode name
	*Mismatch in Opcode type and Operand supplied
	*Too Much Tokens(Opcode/Operand) supplied
	*Less Tokens(Opcode/Operand) supplied
	*Reserve name (Opcode) used as Symbol 
	*Undefined Symbol
	*Bad Positioning of Label
	*End Statement Missing
	*Multiple Declaration for a Symbol
	*Forward Referencing of Varriables
	*Memory Unavailable for Varriable Declaration
	*Exhausted program memory

Note- The main aim of Error Handling was to terminate program when an error is encoutered.
          Two or more errors could be present when program terminats from a particular line.
          The program terminates with any of these errors and in no case all.
          As a result the error statement may seem uncontextual sometimes.
****************************************************************************************************************************************

***************************************************INSTRUCTIONS TO RUN******************************************************************
Language and Version Support:
*C++17 support  needed to run the program.

Method 1: Using executable program
	*Run the main.exe file.
	*Enter the file name containing assembly code.
	*The assembler will start working now.
	*Check the generated object/machine code in the produced output file.
Method 2:	Using command prompt or terminal
	*Enter "g++ -std=c++17 driver.cpp -o main" (without quotes)
	*Enter "main" (without quotes)
	*Enter the file name containing assembly code.
	*The assembler will start working now.
	*Check the generated object/machine code in the produced output file.
Method 3:	Using IDE
	*First check if C++17 is supported or not.
	*Complie and Run the "driver.cpp"
	*Enter the file name containing assembly code.
	*The assembler will start working now.
	*Check the generated object/machine code in the produced output file.
****************************************************************************************************************************************

