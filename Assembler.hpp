#include <string>
#include <map>
#include <vector>
#include <fstream>
#include "pre.hpp"
#include <functional>
#include <vector>
#include <boost/format.hpp>
#include <boost/format/group.hpp>
#include <iomanip>
#include "SyntaxException.hpp"
#ifndef ASSEMBLER
#define ASSEMBLER
struct type{
    int id;
    int size;
    std::string name;
    std::string token;
    std::map<std::string , std::string> ins;
    // just in case we need information other than opcode for example funct in R-type
    std::map<std::string , std::string> extra;
};


class Assembler ;
typedef int(*micro_function)(std::string & ,int ,type* , Assembler * ); 
#define NO_MORE_LINES -1
#define THERE_STILL_MORE_LINES 1
class Assembler{
    private :
        std::map<std::string , int> labels;
        std::map<std::string , micro_function > func;
        std::string delimiter_ins = ", \t";
        std::string delimiter_micro_function = "{}()|, \t";
        std::ifstream file;
        std::vector<type *> types;
        Preprocessor * prep;
        int pc =0;

    public:
        Assembler(std::string input_file);
        std::string next(int * status);

        void add_type(type * t);
        type * get_type(std::string & line);
        int get_label(std::string & x);
        std::string parse_instruction(std::string & ins);
        int get_instruction_pointer() {return pc ;};
        void add_micro_function(std::string name , int(*func)(std::string & ,int ,type* , Assembler * ));
};





Assembler::Assembler(std::string input_file){
    std::string x;
	std::string output_file = "file_after_preproccessing_AAsembler_need_long_name_to_sure_no_other_file_has_the_same_name.txt";
    this->pc = 0;
    this->prep = new Preprocessor(input_file, output_file, &this->labels);
    this->prep->process();
    this->file.open(output_file);


};
std::string Assembler::next(int * status){
    std::string line ;
    std::string hex;
   if(!std::getline(this->file , line)){
       *status = NO_MORE_LINES;
       return "";
   }
    *status = THERE_STILL_MORE_LINES;
    hex = this->parse_instruction(line);
    this->pc++;
    return hex;
    
}


type *  Assembler::get_type(std::string & ins){
    for(auto t : this->types){
        auto result = t->ins.find(ins);
        if(result != t->ins.end())
            return t;  
    }

    throw SyntaxException("unkown instruction type" + ins);


}

// type
std::string Assembler::parse_instruction(std::string & ins){
    std::string func_str  ,arg0 ,token , strhex;
    std::vector<std::string> tokens , ins_split;
    long long hex = 0;
    int arg1,x;
    int shift_amount = 0;
    try {
        boost::split(ins_split , ins , boost::is_any_of(this->delimiter_ins) , boost::token_compress_on);
        type  * t = get_type(ins_split[0]);

        boost::trim_if(t->token , boost::is_any_of(this->delimiter_micro_function));
        boost::split(tokens , t->token  , boost::is_any_of(this->delimiter_micro_function) , boost::token_compress_on);

            for(int i = tokens.size() - 1; i > 0; i-=3){
            func_str = tokens[i - 2];
            if(tokens[i - 1][0] == '$')
                   arg0 = tokens[i - 1].substr(1);
            else
                arg0 = ins_split[std::stoul(tokens[i - 1])];
            arg1 = std::stoul(tokens[i]);
            x = func[func_str](arg0 , arg1 , t  , this);
          //  if(x > (1 << (arg1 + 1))-1)
           //     perror("return values exceeded the limit cant garantee the output");
            x = x << shift_amount;
            hex = hex | x ;
            shift_amount += arg1;
        }

        strhex = boost::str(boost::format("%x") % boost::io::group(std::setw(t->size / 4), std::setfill('0'), hex));
        return strhex;


    } catch (SyntaxException e) {
        throw SyntaxException(ins +":\nLine:" + std::to_string(pc) +".\nInformation:" + e.what());
    }catch(std::exception e){
        throw SyntaxException(ins +":\nLine:" + std::to_string(pc) +".\nInformation: Unknown Excption " + e.what());

    }

}

void Assembler::add_micro_function(std::string name , int(*func)(std::string & ,int ,type* , Assembler * )){

    this->func[name] = func;

}

void Assembler::add_type(type * t){
    // we may need to check if the pointe is valid :(
    
    this->types.push_back(t);
};

int Assembler::get_label(std::string & label){
    boost::algorithm::to_lower(label);
    auto res = this->labels.find(label);
    return res->second;

}


#endif
