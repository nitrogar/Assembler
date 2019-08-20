#include "Assembler.hpp"
#include "micro_function.hpp"
#include <string>
int main(){
    type type_r0 , type_r1 , type_j , type_i0 , type_i1 ,type_i2 , type_lui ;
    type_r0.name  = "R_general";
    type_r0.id = 0 ;
    //assume this instruction :
    //          add     $1,    $4 ,     $7
    //          {0}    {1} rd    {2} rs     {3}rt
    type_r0.token = "opcode({0},4)|reg({2},3)|reg({3},3)|reg({1},3)|funct({0},3)";
    type_r0.size = 16;



    type_r1.name  = "R_jump";
    type_r1.id = 4 ;
    //assume this instruction :
    //          jr $7
    //          {0}    {1} rd    {2} rs     {3}rt
    type_r1.token = "opcode({0},4)|reg({1},3)|fill_gap({$000000} , 6)|funct({0},3)";
    type_r1.size = 16;



    type_i0.name  = "I_general";
    type_i0.id = 1;
    //assume this instruction :
    //          add     $1,    $4 ,     755
    //          {0}    {1} rt   {2} rs  {3}imm
    type_i0.token = "opcode({0},4)|reg({2},3)|reg({1},3)|imm({3},6)";
    type_i0.size = 16;



    type_i1.name  = "I_mem";
    type_i1.id = 3;
    //assume this instruction :
    //          add     $1,    12($4)
    //          {0}    {1} rt   {2} imm(rs)
    type_i1.token = "opcode({0},4)|offset_reg({2},3)|reg({1},3)|offset_imm({2},6)";
    type_i1.size = 16;


    type_i2.name  = "I_branch";
    type_i2.id = 6;
    //assume this instruction :
    //          beq     $1,    $4 , loop
    //          {0}    {1} rs   {2}rt   {3}imm(rs)
    type_i2.token = "opcode({0},4)|reg({1},3)|reg({2},3)|imm({3},6)";
    type_i2.size = 16;


    type_lui.name  = "load upper immediate";
    type_lui.id = 6;
    //assume this instruction :
    //          lui 4444
    //          {0}    {1} rs   {2}rt   {3}imm(rs)
    type_lui.token = "opcode({0},4)|imm({1},12)";
    type_lui.size = 16;


    type_j.name  = "J";
    type_j.id = 2 ;
    //assume this instruction :
    //          j   342412411
    //          {0}    {1} imm
    type_j.token = "opcode({0},4)|imm({1} , 12)";
    type_j.size = 16;




    // now we need to fill the ins map inside struct 'type' , to map opcode to binary string
    // you can use any way you want to fill the map but dont leave the map empty !!!!
    // i will do it in code so it will be easiar to understand but you can use cofigration files


    // R Type                        // funct for R                  // I&J types
    type_r0.ins["and"] = "0000";    type_r0.extra["and"] = "000";   type_i1.ins["sw"] = "0011";
    type_r0.ins["or"] = "0000";     type_r0.extra["or"] = "001";    type_i0.ins["andi"] = "0100";
    type_r0.ins["nor"] = "0000";    type_r0.extra["nor"] = "010";   type_i0.ins["ori"] = "0101";
    type_r0.ins["xor"] = "0000";    type_r0.extra["xor"] = "011";   type_i0.ins["addi"] = "0110";
    type_r0.ins["xnor"] = "0000";   type_r0.extra["xnor"] = "100";  type_i2.ins["beq"] = "0111";
    type_r0.ins["sll"] = "0000";    type_r0.extra["sll"] = "101";   type_i2.ins["bne"] = "1000";
    type_r0.ins["srl"] = "0000";    type_r0.extra["srl"] = "110";   type_i2.ins["ble"] = "1010";
    type_r0.ins["sra"] = "0000";    type_r0.extra["sra"] = "111";   type_i2.ins["bgt"] = "1011";
    type_r0.ins["add"] = "0001";    type_r0.extra["add"] = "000";   type_i2.ins["bge"] = "1100";
    type_r0.ins["sub"] = "0001";    type_r0.extra["sub"] = "001";   type_j.ins["j"] = "1101";
    type_r0.ins["stl"] = "0001";    type_r0.extra["stl"] = "010";   type_j.ins["jal"] = "1110";
    type_r0.ins["sltu"] = "0001";   type_r0.extra["sltu"] = "011";  type_j.ins["lui"] = "1111";
    type_r0.ins["movz"] = "0001";   type_r0.extra["movz"] = "100";  type_i1.ins["lw"] = "0010";
    type_r0.ins["movn"] = "0001";   type_r0.extra["movn"] = "101";
    type_r1.ins["jr"] = "0001";     type_r1.extra["jr"] = "110";
    type_r0.ins["jalr"] = "0001";   type_r0.extra["jalr"] = "111";


    //// I will say it again you can do it by config file or what ever way you see suitable
   //// this is just to show you how to do it in code ....



    /* micro_function is what i call this line below
       "opcode({0},4)|reg({2},3)|reg({3},3)|reg({1},3)|funct({0},3)"
       this use three 'micro_function's :
       1- opcode
       2- reg
       3- funct
       the first argument is the index of the word we want to pass after using string.split
       the default split deliminter is " \t," , you can find it in Assembler.h member delimiter
       for example if the assembly line is add $3 , $1 , 3
       then after "add $3 , $1 , 3".split(" \t,") ===> ['add' , '$3' , '$1' , '3'] the first argument is the index ; {index} : {0} == 'add' {1} == '$3' and so on.
       the second argument is the size in bits (how many bits) this field needs

       all micro_functons are defined in micro_function.h header file
       if you want to write your own just use the same argument types i.e function signeture.

       after implement the micro_function you need to link it to be used inside the assebler object
    */

    char * input_file = "test.txt";
    char * output_file = "output.txt";
    std::ofstream outf;
    Assembler * nasm = new Assembler(input_file);

    outf.open(output_file);
    nasm->add_type(&type_r0);
    nasm->add_type(&type_r1);
    nasm->add_type(&type_j);
    nasm->add_type(&type_i0);
    nasm->add_type(&type_i1);
    nasm->add_type(&type_i2);

    nasm->add_micro_function("reg" , reg);
    nasm->add_micro_function("label" , label);
    nasm->add_micro_function("imm" , imm);
    nasm->add_micro_function("opcode" , opcode);
    nasm->add_micro_function("funct" , funct);
    nasm->add_micro_function("offset_reg" , offset_reg);
    nasm->add_micro_function("offset_imm" , offset_imm);
    nasm->add_micro_function("fill_gap" , fill_gap);

    int status;
    std::string string_hex;
    do{
        string_hex = nasm->next(&status);
        outf << string_hex << "\t";
        std::cout << string_hex;

    }
    while(status != NO_MORE_LINES);
    outf.close();

    return 0;



}
