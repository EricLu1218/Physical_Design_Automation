#pragma once
#include <iostream>
#include <string>
#include <unistd.h>

class ArgumentParser
{
public:
    std::string netFilepath, cellFilepath, outputFilepath;

    ArgumentParser() {}

    bool parse(int argc, char *argv[])
    {
        int opt;
        while ((opt = getopt(argc, argv, "h")) != -1)
        {
            switch (opt)
            {
            default:
                std::cerr << "Usage: " << argv[0] << " <net file> <cell file> <output file>\n";
                return false;
                break;
            }
        }

        if (argc - optind != 3)
        {
            std::cerr << "Usage: " << argv[0] << " <net file> <cell file> <output file>\n";
            return false;
        }
        netFilepath = argv[optind];
        cellFilepath = argv[optind + 1];
        outputFilepath = argv[optind + 2];
        return true;
    }
};
