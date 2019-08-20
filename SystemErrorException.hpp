#pragma once
#include <string>
#include <exception>
class SystemErrorException : public std::exception
{
	std::string _msg;
public:
	SystemErrorException();
	SystemErrorException(std::string msg);
	std::string what();
	~SystemErrorException(); 
};



SystemErrorException::SystemErrorException()
{
}

SystemErrorException::SystemErrorException(std::string msg)
{
}

std::string SystemErrorException::what()
{
    return _msg;
}


SystemErrorException::~SystemErrorException()
{
}

