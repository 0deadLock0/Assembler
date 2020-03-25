/*
Two Pass Assembler:
	"errorHandler.h" - Part of a set of programs 	
	Includes:
		driver.cpp - maintains the flow of the Assembler while handling serious errors
		lexer.h - provides functions which defines the lexical structure of program and do tokenisation alongwith basic syntax checking
		parser.h - provides functions which gives meaning to the assembly code and convert it to machine/object code
		symbolTable.h - provides functions which builds the symbol tabel for the provided assmebly code with looking for some context specific errors 
		errorHandler.h - provides functions which handles all the errors encountered along the flow of Assmebler 
		helper.h - provides non assembler specific functions which helps in smoothing of operations needed to be done at various points in the flow of Assembler
		printer.h - provides functions which helps in printing the data recieved and information processed 
	Functionality:
		Converts a given assembly code present in some txt file into its adequate machine/object code and writes it down in another txt file
	Created by: Abhimanyu Gupta
*/

#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

class ErrorHandler
{
	public:
		ErrorHandler();
		static bool programTerminates();
		static bool failedToOpenFile(std::string);
		static bool invalidSymbol(int,int,char);
		static bool invalidNaming(int);
		static bool mismatchedOperandOperation(int,std::string,std::string);
		static bool invalidInstruction(int,int,std::string,std::string);
		static bool undefinedVarriable(std::string,int);
		static bool undefinedLabel(std::string,int);
		static bool invalidPositionForLabel(int,std::string);
		static bool endStatementMissing(int);
		static bool multipleDeclarations(std::string,int);
		static bool forwardReferencingNotAllowed(std::string,int);
		static bool memoryOutOfRange(int,std::string);
		static bool outOfRangeForBinaryConversion(int,long);
		static bool memoryExhausted(int);
};
ErrorHandler::ErrorHandler()
{
}
bool ErrorHandler::programTerminates()
{
	std::cout<<"\n"<<"Program Terminates....."<<"\n";
	return true;
}
bool ErrorHandler::failedToOpenFile(std::string fileName)
{
	std::cout<<"\n"<<"File- ("<<fileName<<") Unable to open it!!!"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::invalidSymbol(int l,int w,char c)
{
	std::cout<<"\n"<<"Line-"<<l<<", index-"<<w<<" , Invalid symbol found ("<<c<<")"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::invalidNaming(int l)
{
	std::cout<<"\n"<<"Line-"<<l<<" , Varriable/Label name can't include digits in any form"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::mismatchedOperandOperation(int l,std::string s1,std::string s2)
{
	std::cout<<"\n"<<"Line-"<<l<<" , ("<<s1<<") and ("<<s2<<") can't be operand and operation in the same line"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::invalidInstruction(int l,int n=-1,std::string s="",std::string t="")
{
	if(t=="Invalid Operand")
		std::cout<<"\n"<<"Line-"<<l<<" , ("<<s<<") doesn't name an operand type"<<"\n";
	else if(t=="Invalid Operand type")
	{
		if(n<=2)
			std::cout<<"\n"<<"Line-"<<l<<" , ("<<s<<") can't be an operand here"<<"\n";
		else
			std::cout<<"\n"<<"Line-"<<l<<" , ("<<s<<") can't be an operand due to inappropriate number of tokens in the line"<<"\n";
	}
	else if(t=="Reserved Name")
		std::cout<<"\n"<<"Line-"<<l<<" , ("<<s<<") can't be a varriable/label name"<<"\n";
	else if(n==1 || n==2)
		std::cout<<"\n"<<"Line-"<<l<<" , ("<<s<<") doesn't standalone as a single token in the instruction"<<"\n";
	else if(n>2)
		std::cout<<"\n"<<"Line-"<<l<<" , "<<"Invalid number of tokens found ("<<n<<")"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::undefinedVarriable(std::string s,int l)
{
	std::cout<<"\n"<<"Line-"<<l<<" , varriable ("<<s<<") isn't defined";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::undefinedLabel(std::string s,int l)
{
	std::cout<<"\n"<<"Line-"<<l<<" , label ("<<s<<") isn't defined";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::invalidPositionForLabel(int l,std::string s)
{
	std::cout<<"\n"<<"Line-"<<l<<" , label ("<<s<<") defines nothing";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::endStatementMissing(int l)
{
	std::cout<<"\n"<<"Line-"<<l<<" , end statement expected after this line"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::multipleDeclarations(std::string s,int l)
{
	std::cout<<"\n"<<"Line-"<<l<<" , Multiple declarations for symbol- ("<<s<<")"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::forwardReferencingNotAllowed(std::string var,int l)
{
	std::cout<<"\n"<<"Line-"<<l<<" , varraible ("<<var<<") is used before declaration"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::memoryOutOfRange(int n,std::string t)
{
	std::cout<<"\n"<<"Program memory exceeded, "<<t<<" declaration limit exhausted ("<<n<<")"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::outOfRangeForBinaryConversion(int s,long n)
{
	std::cout<<"\n"<<"(::) ("<<n<<") can't be represented in "<<s<<" bits"<<"\n";
	throw ErrorHandler::programTerminates();
}
bool ErrorHandler::memoryExhausted(int l)
{
	std::cout<<"\n"<<"Line-"<<l<<" Program memory exhausted "<<"\n";
	throw ErrorHandler::programTerminates();
}
#endif