#pragma once
#include <iostream>
#include <string>
#include <unistd.h>

class ArgumentParser
{
public:
    std::string netFile, cellFile, outputFile;

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
        netFile = argv[optind];
        cellFile = argv[optind + 1];
        outputFile = argv[optind + 2];
        return true;
    }
};
