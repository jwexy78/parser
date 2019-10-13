#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "Tokenizer.h"

int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>"
                  << std::endl;
        exit(1);
    }

    std::ifstream inputfstream(argv[1]);
    if (!inputfstream.good()) {
        std::cerr << "Invalid Input File: " << argv[1] << std::endl;
        exit(1);
    }
    std::stringstream inputbuffer;
    inputbuffer << inputfstream.rdbuf();
    std::string contents = inputbuffer.str();

    std::vector<Tokenizer::Token> tokens = Tokenizer::tokenize(contents.c_str());
    std::cout << Tokenizer::toString(tokens);
}