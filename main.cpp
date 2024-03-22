#include <iostream>
#include "CmdArgsCpp.hpp"

int main(int argc, char** argv){
    

    std::cout << "Hellooo" << std::endl;

    CmdArgsCpp cmd_args;

    cmd_args.AddArgument("i");

    cmd_args.AddArgument("o", "output", "The output", "en");

    cmd_args.ParseArguments(argc, argv);

    cmd_args.DebugArgs();

    return EXIT_SUCCESS;

}
