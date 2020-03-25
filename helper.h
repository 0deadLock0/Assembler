/*
Two Pass Assembler:
	"helper.h" - Part of a set of programs 	
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

#ifndef HELPER_H
#define HELPER_H

class Helper
{
	public:
		Helper();
		struct TableContents
		{
			public:
				std::string name;
				std::string type;
				std::string address;
				int referenceLine;
		};
		static std::vector<std::string> readFromFile(std::string);
		static void writeToFile(std::string,std::vector<std::vector<std::string> >);
		static bool isTokenKeyword(std::string,std::vector<std::pair<std::string,int> >);
		static int getKeywordType(std::string,std::vector<std::pair<std::string,int> >);
		static bool isSymbolDefined(std::string,std::vector<Helper::TableContents>);
		static std::string getSymbolType(std::string,std::vector<Helper::TableContents>);
		static std::string getSymbolRegister(std::string,std::vector<Helper::TableContents>);
		static std::vector<std::pair<std::string,int> > getAllOfInTable(std::vector<Helper::TableContents>,std::string);
		static std::vector<std::string> splitOnSpaces(std::string);
		static std::string integerToBinary(int,long);
		static bool isNumber(std::string);
		static std::string toUpperString(std::string);
};
Helper::Helper()
{
}
std::vector<std::string> Helper::readFromFile(std::string fileName)
{
	std::vector<std::string> lines;
	std::fstream file;

	file.open(fileName.c_str(),std::ios::in);

	if(!file.is_open())
		throw ErrorHandler::failedToOpenFile(fileName);
	
	std::string line;
	while(std::getline(file,line))
		lines.push_back(line);

	file.close();

	return lines;
}
void Helper::writeToFile(std::string fileName,std::vector<std::vector<std::string> > words)
{
	std::fstream file; 
	std::time_t curr_time=std::time(NULL);
	std::string time=std::ctime(&curr_time);

	file.open(fileName.c_str(),std::ios::out);

	file<<"\n"<<"Object code of machine code present in \""<<fileName.substr(0,fileName.rfind("_object_code.txt"))+".txt\" file"<<"\n";
	file<<"\n"<<"Created at- "<<time<<"\n";
	for(auto &ww:words)
	{
		for(auto &w:ww)
			file<<w<<" ";
		file<<"\n";
	}

	file.close();

	return ;
}
bool Helper::isTokenKeyword(std::string token,std::vector<std::pair<std::string,int> > keywords)
{
	for(auto &k:keywords)
		if(token==k.first)
			return true;

	return false;
}
bool Helper::isSymbolDefined(std::string symbol,std::vector<Helper::TableContents> symbolsList)
{
	for(auto &s:symbolsList)
		if(symbol==s.name)
			return true;

	return false;
}
std::string Helper::getSymbolType(std::string symbol,std::vector<Helper::TableContents> symbolTable)
{
	for(auto &st:symbolTable)
		if(st.name==symbol)
			return st.type;

	return "Not found";
}
std::string Helper::getSymbolRegister(std::string symbol,std::vector<Helper::TableContents> symbolTable)
{
	for(auto &st:symbolTable)
		if(st.name==symbol)
			return st.address;

	return "Not found";
}
int Helper::getKeywordType(std::string token,std::vector<std::pair<std::string,int> > keywords)
{
	for(auto &k:keywords)
		if(token==k.first)
			return k.second;

	return -1;
}
std::vector<std::pair<std::string,int> > Helper::getAllOfInTable(std::vector<Helper::TableContents> symbolsList,std::string type)
{
	std::vector<std::pair<std::string,int> > lines;

	for(auto &s:symbolsList)
		if(type==s.type)
			lines.push_back(std::make_pair(s.name,s.referenceLine));

	return lines;
}
std::vector<std::string> Helper::splitOnSpaces(std::string s)
{
	std::vector<std::string> vs;
	std::stringstream ss(s);

	while(ss>>s)
		vs.push_back(s);
	
	return vs;
}
std::string Helper::toUpperString(std::string s)
{
	std::string ns="";
	for(int i=0;i<s.size();i++)
		ns+=std::toupper(s[i]);

	return ns;
}
bool Helper::isNumber(std::string s)
{
	for(int i=0;i<s.size();i++)
		if(!std::isdigit(s[i]))
			return false;

	return true;
}
std::string Helper::integerToBinary(int s,long n)
{	
	float m=std::log2(n);
	if(m>s)
		throw ErrorHandler::outOfRangeForBinaryConversion(s,n);

	std::string binary;
	for (int i=s-1;i>=0;i--) 
	{ 
        int k=n>>i; 
        if (k&1) 
            binary+="1"; 
        else
            binary+="0"; 
    } 

    return binary;
}
#endif