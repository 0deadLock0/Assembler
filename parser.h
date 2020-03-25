/*
Two Pass Assembler:
	"parser.h" - Part of a set of programs 	
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

#include"helper.h"

#ifndef PARSER_H
#define PARSER_H

class Parser
{
	public:
		Parser();
		static std::vector<std::vector<std::string> > parseTokens(std::vector<std::vector<std::string> >,std::vector<std::pair<std::string,int> >,std::vector<Helper::TableContents>,std::unordered_map<std::string,std::string>);
};
Parser::Parser()
{
}
std::vector<std::vector<std::string> > Parser::parseTokens(std::vector<std::vector<std::string> > tokens,std::vector<std::pair<std::string,int> > asm_opCodes,std::vector<Helper::TableContents> symbolTable,std::unordered_map<std::string,std::string> asm_to_mac_opCodes)
{
	std::vector<std::vector<std::string> > parsedTokens;
	std::vector<std::string> pt;

	for(auto &t:tokens)
	{
		pt.clear();

		if(t.size()==0)
			pt.push_back("");
		else if(t.size()==1)
		{
			if(t[0][t[0].size()-1]==':')
				pt.push_back("");
			else
			{
				pt.push_back(asm_to_mac_opCodes[t[0]]);
				pt.push_back("00000000");
			}
		}
		else if(t.size()==2)
		{
			if(t[0][t[0].size()-1]==':')
			{
				pt.push_back(asm_to_mac_opCodes[t[1]]);
				pt.push_back("00000000");
			}
			else
			{
				pt.push_back(asm_to_mac_opCodes[t[0]]);
				pt.push_back(Helper::getSymbolRegister(t[1],symbolTable));
			}
		}
		else
		{
		
			pt.push_back(asm_to_mac_opCodes[t[1]]);
			pt.push_back(Helper::getSymbolRegister(t[2],symbolTable));

		}

		parsedTokens.push_back(pt);
	}

	return parsedTokens;
}
#endif