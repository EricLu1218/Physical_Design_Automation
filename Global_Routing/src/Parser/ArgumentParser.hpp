#pragma once
#include <iostream>
#include <string>
#include <unistd.h>

class ArgumentParser
{
public:
    std::string inputFile, outputFile;

    ArgumentParser() {}

    bool parse(int argc, char *argv[])
    {
        int opt;
        while ((opt = getopt(argc, argv, "h")) != -1)
        {
            switch (opt)
            {
            default:
                std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
                return false;
                break;
            }
        }

        if (argc - optind != 2)
        {
            std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
            return false;
        }
        inputFile = argv[optind];
        outputFile = argv[optind + 1];
        return true;
    }
};
