#ifndef  pre
#define pre
#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include "SystemErrorException.hpp"
#include <boost/algorithm/string.hpp>

//template <class LabelContainer>
class Preprocessor
{

	std::string  output_file;
	std::string  input_file;
	int counter;
	std::map<std::string, int> * container;
public:
	Preprocessor();
	Preprocessor(std::string & , std::string & , std::map<std::string, int> * container);
	void setInput(std::string & str);
	void setOutput(std::string & str);
	std::string getOutputName();
	std::string getInputName();
	std::map<std::string, int> * process();
	~Preprocessor();
};




Preprocessor::Preprocessor(std::string & input_file, std::string & output_file , std::map<std::string, int> * container)
{
	this->output_file = output_file;
	this->input_file = input_file;
	this->container = container;
}

//template<class LabelContainer>
void Preprocessor::setInput(std::string & str)
{
	this->input_file = str;
}

//template<class LabelContainer>
void Preprocessor::setOutput(std::string & str)
{
	this->output_file = str;
}

//template<class LabelContainer>
std::string Preprocessor::getOutputName()
{
	return std::string();
}

//template<class LabelContainer>
std::string Preprocessor::getInputName()
{
	return input_file;
}

//template<class LabelContainer>
std::map<std::string, int>  * Preprocessor::process()
{

	std::ifstream infile;
	std::ofstream outfile;
	std::string line;
	std::string label;
	int index;
	int line_num = 0;
	infile.open(input_file);
	outfile.open(output_file);
	std::ifstream config_file;
	config_file.open(input_file);
	if (!infile.is_open())
		throw  SystemErrorException("Preprocessor :: cant open file " + input_file);

	if (!outfile.is_open())
		throw  SystemErrorException("Preprocessor :: cant open file " + output_file);

	while (std::getline(infile, line)) {
		index = line.find('#');
		if (index != std::string::npos)
			line = line.substr(0, index);

		index = line.find(':');
		if (index != std::string::npos) {
			label = line.substr(0, index);
			boost::trim(label);
			boost::algorithm::to_lower(label);
			container->insert(std::pair<std::string, int>(label, line_num));
			line = line.substr(index+1);
		}
		boost::trim(line);
		if (!line.empty()){
			outfile << line <<std::endl;
			line_num++;
		}


	}




	return container;
}
//template<class LabelContainer>
Preprocessor::~Preprocessor()
{
	
}
#endif
