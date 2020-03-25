/*
Two Pass Assembler:
	"printer.h" - Part of a set of programs 	
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

#ifndef PRINTER_H
#define PRINTER_H

class Printer
{
	public:
		Printer();
		static void printCodeLines(std::vector<std::string>);
		static void printTokens(std::vector<std::vector<std::string> >);
		static void printTokensAndLC(std::vector<std::vector<std::string> >,std::vector<std::string>);
		static void printSymbolTable(std::vector<Helper::TableContents>);
};
Printer::Printer()
{
}
void Printer::printCodeLines(std::vector<std::string> vS)
{	
	std::cout<<"\n";
	for(int i=0;i<vS.size();i++)
		std::cout<<i+1<<": "<<vS[i]<<"\n";
	std::cout<<"\n";
}
void Printer::printTokens(std::vector<std::vector<std::string> > tokens)
{
	std::cout<<"\n";
	for(int i=0;i<tokens.size();i++)
	{
		for(int j=0;j<tokens[i].size();j++)
			std::cout<<tokens[i][j]<<" ";
		std::cout<<"\n";
	}
}
void Printer::printTokensAndLC(std::vector<std::vector<std::string> > tokens,std::vector<std::string> lc)
{
	int s=0;
	int l;

	for(auto &t:tokens)
	{
		l=0;
		for(auto &ts:t)
			l+=ts.size()+1;
		s=(l>s)?l:s;
	}

	std::cout<<"\n";
	for(int i=0;i<tokens.size();i++)
	{
		l=0;
		for(int j=0;j<tokens[i].size();j++)
		{
			std::cout<<tokens[i][j]<<" ";
			l+=tokens[i][j].size()+1;
		}
		std::cout<<std::setw(s-l+1)<<" "<<"   "<<lc[i]<<"\n";
	}
}
void Printer::printSymbolTable(std::vector<Helper::TableContents> t)
{
	int i0,i1,i2;
	std::string s0="NAME",s1="TYPE",s2="ADDRESS";

	i0=s0.size();
	i1=s1.size();
	i2=s2.size();

	for(auto &s:t)
	{
		i0=(s.name.size()>i0)?s.name.size():i0;
		i1=(s.type.size()>i1)?s.type.size():i1;
		i2=(s.address.size()>i2)?s.address.size():i2;
	}

	std::cout<<"\n"<<std::setw(5)<<"|"<<std::setw(i0)<<s0<<"|   |"<<std::setw(i1)<<s1<<"|   |"<<std::setw(i2)<<s2<<"|"<<"\n";

	std::cout<<"\n";
	for(auto &s:t)
		std::cout<<std::setw(5)<<"|"<<std::setw(i0)<<s.name<<"|   |"<<std::setw(i1)<<s.type<<"|   |"<<std::setw(i2)<<s.address<<"|"<<"\n";
	std::cout<<"\n";

	return;
}
#endif