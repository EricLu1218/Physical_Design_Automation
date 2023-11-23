#pragma once
#include <iostream>
#include <string>
#include <unistd.h>

class ArgumentParser
{
public:
    std::string inputFilepath, outputFilepath;

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
        inputFilepath = argv[optind];
        outputFilepath = argv[optind + 1];
        return true;
    }
};
