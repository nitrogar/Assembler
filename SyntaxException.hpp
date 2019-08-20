#pragma once
#include <string>
#include <exception>
class SyntaxException : public std::exception
{

	std::string * msg;
public:
	SyntaxException(std::string );
	std::string what();

	~SyntaxException();
};







SyntaxException::SyntaxException(std::string s)
{
	msg = new std::string(s);
}

std::string SyntaxException::what()
{
	return  *msg;
}

SyntaxException::~SyntaxException()
{
	delete msg;
}

