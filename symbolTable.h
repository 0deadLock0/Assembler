/*
Two Pass Assembler:
	"symbolTable.h" - Part of a set of programs 	
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

#include"errorHandler.h"
#include"helper.h"

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

class SymbolTable
{	
	public:
		SymbolTable();
		static std::vector<Helper::TableContents> buildSymbolTable(std::vector<std::vector<std::string> >,std::vector<std::string>,std::vector<std::pair<std::string,int> >);
};
SymbolTable::SymbolTable()
{
}
std::vector<Helper::TableContents> SymbolTable::buildSymbolTable(std::vector<std::vector<std::string> > tokens,std::vector<std::string> lc,std::vector<std::pair<std::string,int> > opCodes)
{
	std::vector<Helper::TableContents> st;

	int n=8;
	int vlCount=1; // 0 is reserved for single operand instruction

	for(int i=0;i<tokens.size();i++)
	{
		if(vlCount>(2<<(n-1)))
			ErrorHandler::memoryOutOfRange(vlCount,"VARRIABLE/LITERAL");

		if(tokens[i].size()==0)
			continue;
		else if(tokens[i].size()==1)
		{
			if(tokens[i][0][tokens[i][0].size()-1]==':')
			{
				if(Helper::isSymbolDefined(tokens[i][0].substr(0,tokens[i][0].size()-1),st))
					throw ErrorHandler::multipleDeclarations(tokens[i][0].substr(0,tokens[i][0].size()-1),i+1);
				else
				{
					int j=i+1;
					while(j<tokens.size() && tokens[j].size()==0)
						j++;
					if(j==tokens.size())
						throw ErrorHandler::invalidPositionForLabel(i+1,tokens[i][0].substr(0,tokens[i][0].size()-1));
					else
						st.push_back({tokens[i][0].substr(0,tokens[i][0].size()-1),"LABEL",lc[j],(i+1)});
				}
			}
		}
		else if(tokens[i].size()==2)
		{
			if(tokens[i][0][tokens[i][0].size()-1]==':')
			{
				if(Helper::isSymbolDefined(tokens[i][0].substr(0,tokens[i][0].size()-1),st))
					throw ErrorHandler::multipleDeclarations(tokens[i][0].substr(0,tokens[i][0].size()-1),i+1);
				else
					st.push_back({tokens[i][0].substr(0,tokens[i][0].size()-1),"LABEL",lc[i],(i+1)});
			}
			else if(Helper::getKeywordType(tokens[i][0],opCodes)==2)
		 	{
		 		if(Helper::isSymbolDefined(tokens[i][1],st))	
		 		{	
		 			if(Helper::getSymbolType(tokens[i][1],st)=="LABEL")
		 				throw ErrorHandler::multipleDeclarations(tokens[i][1],i+1);
		 			else if(Helper::isNumber(tokens[i][1]))
		 				throw ErrorHandler::invalidInstruction(i+1,tokens[i].size(),tokens[i][0],"Invalid Operand type");
		 		}
				else
		 		{
		 			if(!Helper::isNumber(tokens[i][1]))
		 			{
		 				st.push_back({tokens[i][1],"VARRIABLE","1"+Helper::integerToBinary(n-1,vlCount),(i+1)});
						vlCount+=12;
					}
					else
						throw ErrorHandler::invalidInstruction(i+1,tokens[i].size(),tokens[i][0],"Invalid Operand type");
				}
			}
			else if(Helper::getKeywordType(tokens[i][0],opCodes)==3)
		 	{
		 		if(Helper::isSymbolDefined(tokens[i][1],st))	
		 			continue;
				else //Symbol not defined handled seprately
		 		{
		 			if(Helper::isNumber(tokens[i][1]))
		 			{
		 				st.push_back({tokens[i][1],"LITERAL","1"+Helper::integerToBinary(n-1,vlCount),(i+1)});
						vlCount+=12;
					}
				}
			}
		}
		else
		{
			if(tokens[i][0][tokens[i][0].size()-1]==':')
			{
				if(Helper::isSymbolDefined(tokens[i][0].substr(0,tokens[i][0].size()-1),st))
					throw ErrorHandler::multipleDeclarations(tokens[i][0].substr(0,tokens[i][0].size()-1),i+1);
				else
					st.push_back({tokens[i][0].substr(0,tokens[i][0].size()-1),"LABEL",lc[i],(i+1)});
			}//Invalid number of tokens handled seprately
			if(Helper::getKeywordType(tokens[i][1],opCodes)==2)
		 	{
		 		if(Helper::isSymbolDefined(tokens[i][2],st))	
		 		{	
		 			if(Helper::getSymbolType(tokens[i][2],st)=="LABEL")
		 				throw ErrorHandler::multipleDeclarations(tokens[i][2],i+1);
		 			else if(Helper::isNumber(tokens[i][2]))
		 				throw ErrorHandler::invalidInstruction(i+1,tokens[i].size(),tokens[i][1],"Invalid Operand type");
		 		}
				else
		 		{
		 			if(!Helper::isNumber(tokens[i][2]))
		 			{
		 				st.push_back({tokens[i][2],"VARRIABLE","1"+Helper::integerToBinary(n-1,vlCount),(i+1)});
						vlCount+=12;
					}
					else
						throw ErrorHandler::invalidInstruction(i+1,tokens[i].size(),tokens[i][1],"Invalid Operand type");
				}
			}
			else if(Helper::getKeywordType(tokens[i][1],opCodes)==3)
		 	{
		 		if(Helper::isSymbolDefined(tokens[i][2],st))	
		 			continue;
				else //Symbol not defined handled seprately
		 		{
		 			if(Helper::isNumber(tokens[i][2]))
		 			{
		 				st.push_back({tokens[i][2],"LITERAL","1"+Helper::integerToBinary(n-1,vlCount),(i+1)});
						vlCount+=12;
					}
				}
			}
		}
	}

	return st;
}
#endif