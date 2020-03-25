/*
Two Pass Assembler:
	"driver.cpp" - Part of a set of programs 	
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

#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include<array>
#include<vector>
#include<unordered_map>
#include<utility>
#include<cctype>
#include<iomanip>
#include<ctime>

//User defined headers
#include"parser.h"
#include"lexer.h"
#include"symbolTable.h"
#include"errorHandler.h"
#include"helper.h"
#include"printer.h"

class Driver
{
	public:
		Driver();
		void driveAssembler();
	private:
		std::string input_fileName;
		std::string output_fileName;
		std::unordered_map<std::string,std::string> asm_to_mac;
		std::vector<std::pair<std::string,int> > asm_opCodes;
		std::vector<std::string> asm_code_lines;
		std::vector<std::vector<std::string> > tokens;
		std::vector<std::string> tokensLC;
		std::vector<Helper::TableContents> symbolsTable;
		std::vector<std::vector<std::string> > parsedTokens;
		void doFirstPass();
		void doSecondPass();
		void checkForMoreErrors();
		void checkForUndefinedSymbols();
		void checkForUnsuitableOperandUse();
		void checkForForwardReferencing();
};
Driver::Driver()
{
	this->input_fileName="";
	this->output_fileName="";
	this->asm_opCodes.clear();
	this->asm_to_mac.clear();
	this->asm_code_lines.clear();
	this->tokens.clear();
	this->symbolsTable.clear();
	this->parsedTokens.clear();

	//0-end statement, 1-operation without operand, 2-kind of variable declaration, 3-operation with operand, 4-jump statements with label
	this->asm_opCodes.push_back(std::make_pair("CLA",1));
	this->asm_opCodes.push_back(std::make_pair("LAC",3));
	this->asm_opCodes.push_back(std::make_pair("SAC",2));
	this->asm_opCodes.push_back(std::make_pair("ADD",3));
	this->asm_opCodes.push_back(std::make_pair("SUB",3));
	this->asm_opCodes.push_back(std::make_pair("BRZ",4));
	this->asm_opCodes.push_back(std::make_pair("BRN",4));
	this->asm_opCodes.push_back(std::make_pair("BRP",4));
	this->asm_opCodes.push_back(std::make_pair("INP",2));
	this->asm_opCodes.push_back(std::make_pair("DSP",3));
	this->asm_opCodes.push_back(std::make_pair("MUL",3));
	this->asm_opCodes.push_back(std::make_pair("DIV",3));
	this->asm_opCodes.push_back(std::make_pair("STP",0));

	this->asm_to_mac["CLA"]="0000";
	this->asm_to_mac["LAC"]="0001";
	this->asm_to_mac["SAC"]="0010";
	this->asm_to_mac["ADD"]="0011";
	this->asm_to_mac["SUB"]="0100";
	this->asm_to_mac["BRZ"]="0101";
	this->asm_to_mac["BRN"]="0110";
	this->asm_to_mac["BRP"]="0111";
	this->asm_to_mac["INP"]="1000";
	this->asm_to_mac["DSP"]="1001";
	this->asm_to_mac["MUL"]="1010";
	this->asm_to_mac["DIV"]="1011";
	this->asm_to_mac["STP"]="1100";
}
void Driver::driveAssembler()
{
	std::cout<<"\n"<<"Enter text file contaning assembly code"<<std::endl;
	std::getline(std::cin,this->input_fileName);

	std::cout<<"\n"<<"#)Reading \""<<this->input_fileName<<"\" ....."<<std::endl;
	this->asm_code_lines=Helper::readFromFile(this->input_fileName);
	std::cout<<"\n"<<"#)Read Complete-"<<std::endl;
	Printer::printCodeLines(this->asm_code_lines);

	this->output_fileName=this->input_fileName.substr(0,this->input_fileName.rfind('.'))+"_object_code.txt";

	std::cout<<"\n"<<"#)First pass going on...."<<std::endl;
	this->doFirstPass();
	std::cout<<"\n"<<"#)First pass successfull"<<std::endl;

	std::cout<<"\n"<<"#)Second pass going on...."<<std::endl;
	this->doSecondPass();
	std::cout<<"\n"<<"#)Second pass successfull"<<std::endl;
	
	std::cout<<"\n"<<"Object Code-"<<"\n";
	Printer::printTokens(this->parsedTokens);

	std::cout<<"\n"<<"#)Writing the code to the file- "<<this->output_fileName<<std::endl;
	Helper::writeToFile(this->output_fileName,this->parsedTokens);
	std::cout<<"\n"<<"#)Writing complete"<<std::endl;

	return ;
}
void Driver::doFirstPass()
{
	std::cin.clear();
	std::cout<<"\n"<<"#)Syntax Checking and Tokenization starts...."<<std::endl;
	this->tokens=Lexer::syntaxCheckWithTokenization(this->asm_code_lines,this->asm_opCodes);
	
	this->tokensLC=Lexer::handleLocationCounter(this->tokens);

	this->symbolsTable=SymbolTable::buildSymbolTable(this->tokens,this->tokensLC,this->asm_opCodes);
	
	this->checkForMoreErrors();
	std::cout<<"\n"<<"#)Syntax Checking and Tokenization complete-"<<std::endl;
	Printer::printTokensAndLC(this->tokens,this->tokensLC);

	std::cout<<"\n"<<"#)Building symbol table...."<<std::endl;
	std::cout<<"\n"<<"#)Symbol Table built-"<<std::endl;
	Printer::printSymbolTable(this->symbolsTable);

	return;
}
void Driver::checkForMoreErrors()
{
	this->checkForUndefinedSymbols();
	this->checkForUnsuitableOperandUse();
	this->checkForForwardReferencing();
	
	return;
}
void Driver::checkForUndefinedSymbols()
{
	for(int i=0;i<this->tokens.size();i++)
	{
		if(this->tokens[i].size()<=1)
			continue;
		else if(this->tokens[i][0][tokens[i][0].size()-1]==':')
		{
			if(this->tokens[i].size()==2)
				continue;
			else
			{
				if(!Helper::isSymbolDefined(this->tokens[i][2],this->symbolsTable))
				{
					if(Helper::getKeywordType(this->tokens[i][1],this->asm_opCodes)==3)
						throw ErrorHandler::undefinedVarriable(tokens[i][2],i+1);
					else
						throw ErrorHandler::undefinedLabel(this->tokens[i][2],i+1);
				}
			}
		}
		else if(!Helper::isSymbolDefined(this->tokens[i][1],this->symbolsTable))
		{
			if(Helper::getKeywordType(this->tokens[i][0],this->asm_opCodes)==3)
				throw ErrorHandler::undefinedVarriable(tokens[i][1],i+1);
			else
				throw ErrorHandler::undefinedLabel(this->tokens[i][1],i+1);
		}
	}

	return ;
}
void Driver::checkForUnsuitableOperandUse()
{
	for(int i=0;i<this->tokens.size();i++)
	{
		if(this->tokens[i].size()<=1)
			continue;
		else if(this->tokens[i].size()==2)
		{
			if(this->tokens[i][0][tokens[i][0].size()-1]==':')
				continue;
			else
			{
				int kType=Helper::getKeywordType(this->tokens[i][0],this->asm_opCodes);
				std::string sType=Helper::getSymbolType(this->tokens[i][1],this->symbolsTable);
				
				if(sType=="LABEL")
				{
					if(kType!=4)
						throw ErrorHandler::mismatchedOperandOperation(i+1,this->tokens[i][0],this->tokens[i][1]);
				}
				else
				{
					if(kType==4)	
						throw ErrorHandler::mismatchedOperandOperation(i+1,this->tokens[i][0],this->tokens[i][1]);
				}
			}
		}
		else if(this->tokens[i].size()==3)
		{
			int kType=Helper::getKeywordType(this->tokens[i][1],this->asm_opCodes);
			std::string sType=Helper::getSymbolType(this->tokens[i][2],this->symbolsTable);
				
			if(sType=="LABEL")
			{
				if(kType!=4)
					throw ErrorHandler::mismatchedOperandOperation(i+1,this->tokens[i][1],this->tokens[i][2]);
			}
			else
			{
				if(kType==4)	
					throw ErrorHandler::mismatchedOperandOperation(i+1,this->tokens[i][1],this->tokens[i][2]);
			}
		}
	}

	return ;
}
void Driver::checkForForwardReferencing()
{
	std::vector<std::pair<std::string,int> > vl=Helper::getAllOfInTable(this->symbolsTable,"VARRIABLE");;

	for(int i=0;i<vl.size();i++)
		for(int j=vl[i].second-2;j>=0;j--)
		{
			if(tokens[j].size()<=1)
				continue;
			else if(tokens[j][0][tokens[j][0].size()-1]==':')
			{
				if(tokens[j].size()==2)
					continue;
				else
				{
					if(tokens[j][2]==vl[i].first)
						throw ErrorHandler::forwardReferencingNotAllowed(vl[i].first,j+1);
				}
			}
			else if(tokens[j].size()==2)
			{	
				if(tokens[j][1]==vl[i].first)
					throw ErrorHandler::forwardReferencingNotAllowed(vl[i].first,j+1);
			}	
		}

	return ;
}
void Driver::doSecondPass()
{
	this->parsedTokens=Parser::parseTokens(this->tokens,this->asm_opCodes,this->symbolsTable,this->asm_to_mac);

	return ;
}
int main(int argc,char *argv[])
{
	Driver d;
	std::cout<<"\n"<<"#)Assembler starting"<<"\n";
	d.driveAssembler();
	std::cout<<"\n"<<"#)Assembler finished"<<"\n";
	
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
	std::cin.get();

	return 0;
}
