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
#include"helper.h"

#ifndef LEXER_H
#define LEXER_H

class Lexer
{
	public:
		Lexer();
		static std::vector<std::vector<std::string> > syntaxCheckWithTokenization(std::vector<std::string>,std::vector<std::pair<std::string,int> >);
		static std::vector<std::string> handleLocationCounter(std::vector<std::vector<std::string> >);
	private:
		static std::string checkErrors_GiveCodeSnippet(int,std::string);
		static bool isAlienCharacter(char);
		static std::vector<std::string> getLineTokens_CheckForValidity(int,std::string,std::vector<std::pair<std::string,int>>);
};
Lexer::Lexer()
{
}
std::vector<std::vector<std::string> > Lexer::syntaxCheckWithTokenization(std::vector<std::string> lines,std::vector<std::pair<std::string,int> > opCodes)
{
	std::vector<std::vector<std::string> > tokens; 
	std::string codeLine;
	std::vector<std::string> t;

	for(int i=0;i<lines.size();i++)
	{
		codeLine=Lexer::checkErrors_GiveCodeSnippet(i,lines[i]);
		t=Lexer::getLineTokens_CheckForValidity(i,codeLine,opCodes);
		if(t.size()!=0)
		{
			if(t.size()==1 && t[0][t[0].size()-1]!=':')
				t[0]=Helper::toUpperString(t[0]);
			else if(t.size()==2)
			{
				if(t[0][t[0].size()-1]==':')
					t[1]=Helper::toUpperString(t[1]);
				else
					t[0]=Helper::toUpperString(t[0]);
			}
			else if(t.size()==3)
				t[1]=Helper::toUpperString(t[1]);
		}
		tokens.push_back(t);
	}
	
	for(int i=tokens.size()-1;i>=0;i--)
	{
		if(tokens[i].size()==0)
			continue;
		else if(tokens[i].size()>=2)
			throw ErrorHandler::endStatementMissing(i+1);
		else
		{
			if(!Helper::getKeywordType(tokens[i][0],opCodes)==0)
				throw ErrorHandler::endStatementMissing(i+1);
			else
				break;		
		}
	}

	return tokens;
}
std::string Lexer::checkErrors_GiveCodeSnippet(int l,std::string s)
{	
	std::string ss=s;
	std::replace(ss.begin(),ss.end(),'\t',' ');
	int s1,s2;
	int s3=-1;

	s1=ss.find_first_not_of(" ");
	if(s1!=std::string::npos)
	{	
		ss=ss.substr(s1,s.size()-s1);
		s2=ss.find_last_not_of(" ");
		
		if(s2!=std::string::npos)
		{
			ss=ss.substr(0,s2+1);
			s2+=s1+1;
			s3=ss.find(" ");
			if(s3==std::string::npos)
				s3=-1;
			else
				s3+=s1;
		}
		else
			s2=s.size();
	}
	else
		s1=0;

	bool foundSharp=false;
	bool foundNum=false;
	for(int i=s1;i<s2;i++)
	{	
		if(s[i]=='#')//Comment starts
			return s.substr(0,i);
		else if(s[i]==':') // : exists as part of label only
		{	
			if(foundSharp)
				throw ErrorHandler::invalidSymbol(l+1,i+1,s[i]);
			
			foundSharp=true;
			
			if(i==s1)
				throw ErrorHandler::invalidSymbol(l+1,i+1,s[i]);
			else if(s[i-1]==' ' || s[i-1]=='\t')
				throw ErrorHandler::invalidSymbol(l+1,i+1,s[i]);
			else if(s3==-1 && i!=s2)
				throw ErrorHandler::invalidSymbol(l+1,i+1,s[i]);
			else if(s3!=-1 && i>s3)
				throw ErrorHandler::invalidSymbol(l+1,i+1,s[i]);
		}
		else if(std::isdigit(s[i])) //exists as numeric value
		{	
			if(foundNum)
				ErrorHandler::invalidNaming(l+1);

			if(i<(s2-1) && !std::isdigit(s[i+1]))
				foundNum=true;
			
			if(i==s1)
				ErrorHandler::invalidNaming(l+1);
			else if(i<(s2-1) && !std::isdigit(s[i+1]) && s[i+1]!=' ' && s[i+1]!='\t' && s[i+1]!='#')
				ErrorHandler::invalidNaming(l+1);
			else if(s3==-1)
				ErrorHandler::invalidNaming(l+1);
			else if(s3!=-1 && i<s3)
				ErrorHandler::invalidNaming(l+1);
			else if(!std::isdigit(s[i-1]) && s[i-1]!=' ' && s[i-1]!='\t')
				ErrorHandler::invalidNaming(l+1);
		}
		else if(Lexer::isAlienCharacter(s[i]))
			throw ErrorHandler::invalidSymbol(l+1,i+1,s[i]);
	}
	return s;
}
bool Lexer::isAlienCharacter(char c)
{	
	if(c=='#')
		return false;
	else if(c==' ' || c=='\t')
		return false;
	else if(c==':')
		return false;
	else if(std::isalpha(c))
		return false;
	else if(std::isdigit(c))
		return false;
	else
		return true;
}
std::vector<std::string> Lexer::getLineTokens_CheckForValidity(int l,std::string s,std::vector<std::pair<std::string,int>> opCodes)
{
	std::vector<std::string> tokens;
	
	tokens=Helper::splitOnSpaces(s);

	if(tokens.size()==0)
		return tokens;
	else if(tokens.size()==1)
	{	
		if(tokens[0][tokens[0].size()-1]==':') //Label
		{	
			if(Helper::isTokenKeyword(Helper::toUpperString(tokens[0].substr(0,tokens[0].size()-1)),opCodes))
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[1],"Reserved Name");
			else	
				return tokens;
		}
		else if(Helper::isTokenKeyword(Helper::toUpperString(tokens[0]),opCodes))
		{
			if(Helper::getKeywordType(Helper::toUpperString(tokens[0]),opCodes)<=1)
				return tokens;
			else
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[0]);
		}
		else
			throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[0],"Invalid Operand");
	}
	else if(tokens.size()==2)
	{
		if(tokens[0][tokens[0].size()-1]==':') //Label
		{
			if(Helper::isTokenKeyword(Helper::toUpperString(tokens[0].substr(0,tokens[0].size()-1)),opCodes))
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[0].substr(0,tokens[0].size()-1),"Reserved Name");
			else if(Helper::isTokenKeyword(Helper::toUpperString(tokens[1]),opCodes))
			{
				if(Helper::getKeywordType(Helper::toUpperString(tokens[1]),opCodes)<=1)
					return tokens;
				else
					throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[1]);
			}
			else
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[1],"Invalid Operand");
		}
		else if(Helper::isTokenKeyword(Helper::toUpperString(tokens[0]),opCodes))
		{
			if(Helper::getKeywordType(Helper::toUpperString(tokens[0]),opCodes)<=1)
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[0],"Invalid Operand type");
			else if(Helper::isTokenKeyword(Helper::toUpperString(tokens[1]),opCodes))	
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[1],"Reserved Name");
			else	
				return tokens;
		}
		else
			throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[0],"Invalid Operand");
	}
	else if(tokens.size()==3)
	{
		if(tokens[0][tokens[0].size()-1]!=':')
		{
			if(Helper::isTokenKeyword(Helper::toUpperString(tokens[0]),opCodes))
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[0],"Invalid Operand type");
			else
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[0],"Invalid Operand");
		}
		else
		{	
			if(Helper::isTokenKeyword(Helper::toUpperString(tokens[0].substr(0,tokens[0].size()-1)),opCodes))
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[0].substr(0,tokens[0].size()-1),"Reserved Name");
			else if(!Helper::isTokenKeyword(Helper::toUpperString(tokens[1]),opCodes))
				throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[1],"Invalid Operand");
			else 
			{	
				if(Helper::getKeywordType(Helper::toUpperString(tokens[1]),opCodes)<=1)
					throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[1],"Invalid Operand type");
				else
				{	
					if(Helper::isTokenKeyword(Helper::toUpperString(tokens[2]),opCodes))	
						throw ErrorHandler::invalidInstruction(l+1,tokens.size(),tokens[2],"Reserved Name");
					else	
						return tokens;
				}
			}
		}
	}
	else
		throw ErrorHandler::invalidInstruction(l+1,tokens.size());
}
std::vector<std::string> Lexer::handleLocationCounter(std::vector<std::vector<std::string> > tokens)
{
	std::vector<std::string> lc;

	int c=1;

	for(int i=0;i<tokens.size();i++)
	{
		if(c>=128)
			throw::ErrorHandler::memoryExhausted(i+1);
		else
		{
			if(tokens[i].size()==0)
				lc.push_back("");
			else if(tokens[i].size()==1 && tokens[i][0][tokens[i][0].size()-1]==':')
				lc.push_back("");
			else
			{
				lc.push_back(Helper::integerToBinary(8,c));
				c+=12;
			}
		}
	}

	return lc;
}
#endif