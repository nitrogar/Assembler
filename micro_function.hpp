#include <string>
#include "Assembler.hpp"
#include <vector>
#include "SyntaxException.hpp"
int inline remove_extra(int value , int bits_num){
    int mask = (1 << bits_num) - 1 ;

    value = value & mask ;
     return value ;

} 

int label(std::string & str ,int size,type* t , Assembler * assembler){
    // we need to access labels tabel inside the Assembler object
    int pos =  assembler->get_label(str) - assembler->get_instruction_pointer();
    // mask in case if its a negative number

};
int reg(std::string & str ,int size,type* t , Assembler * assembler){
/// assuming that the asm code contains register in the form $0 , $1 , $2 , ...etc
/// but we can also name the register as t0 , t1 , eax , eip .. whatever
/// str == the name of the register .
/// t   ==  pointer to struct type  in case we need some information
/// as all the other 'micro_fuctions' this will return the hex code which will be put togather in parse_instruction function
/// all the shifting and masking will be inside parse_instruction
/// I will implement $0 , $1 , $2 , ....   etc
    //if(str[0] != '$')
    //    throw SyntaxException("unkown " + str);
    // remove the '$' and retrun the reset as number , example '$55' ==> '55' ==> stoi('55') ==> 55 integer
    return std::stoul(str.substr(1));


};
int imm(std::string & str ,int size,type* t , Assembler * assembler){
    int value;
    try{
        if(str[1] == 'x' || str[1] == 'X')
            value =std::stoi(str , nullptr , 16);
        else
            value  = std::stoi(str);
       }catch(std::exception e){
        // may be its a label
        value = label(str , size , t , assembler);
     }

 // mask in case if its a negative number
    value = remove_extra(value , size);
    return value;

};
int opcode(std::string & str ,int size,type* t , Assembler * assembler){
    auto it = t->ins.find(str);
    if(it == t->ins.end())
       throw SyntaxException("unknown instruction "  + str);
    std::string opcd = it->second;
    int opcd_hex = std::stoul(opcd , 0 , 2);
    // just in case mask the output
    return opcd_hex;

};
int funct(std::string & str ,int size,type* t , Assembler * assembler){
    auto res = t->extra.find(str);
    if(res == t->ins.end())
       throw SyntaxException("cant find funct parameter for this instructions : "  + str);
    std::string funct_hex_string = res->second;
    int funct_hex_int = std::stoul(funct_hex_string , 0 , 2);
    return funct_hex_int;
}

int offset_reg(std::string & str ,int size,type* t , Assembler * assembler){
    std::vector<std::string> split ;
    boost::split(split , str, boost::is_any_of("()") , boost::token_compress_on);
    return reg(split[1] , size , t ,assembler);
}
int offset_imm(std::string & str ,int size,type* t , Assembler * assembler){
    std::vector<std::string> split ;
    boost::split(split , str, boost::is_any_of("()") , boost::token_compress_on);
    return imm(split[0] , size , t ,assembler);
}

int fill_gap(std::string & str ,int size,type* t , Assembler * assembler){
    return std::stoi(str , nullptr , 2);
}
int imm_hex(std::string & str ,int size,type* t , Assembler * assembler){
    return  std::stoi(str , nullptr , 16);
}
