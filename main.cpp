#include <iostream>
#include "CmdArgsCpp.hpp"

int main(int, char**){
    

    std::cout << "Hellooo" << std::endl;

    CmdArgsCpp cmd_args;

    cmd_args.AddArgument("i");

    cmd_args.AddArgument("o", "output", "The output", "en");

    cmd_args.DebugArgs();

    // kane parse ta char ** kai psakse gia 
    // na vreis an iparxoun

    return EXIT_SUCCESS;

}
