#pragma once
#include <iostream>
#include <string>
#include <unistd.h>

class ArgumentParser
{
public:
    std::string auxFile, outputFile;

    ArgumentParser() {}

    bool parse(int argc, char *argv[])
    {
        int opt;
        while ((opt = getopt(argc, argv, "o:h")) != -1)
        {
            switch (opt)
            {
            case 'o':
                outputFile = optarg;
                break;

            default:
                std::cerr << "Usage: " << argv[0] << " [-o output_file] <aux file>\n";
                return false;
                break;
            }
        }

        if (argc - optind != 1)
        {
            std::cerr << "Usage: " << argv[0] << " [-o output_file] <aux file>\n";
            return false;
        }
        auxFile = argv[optind];

        if (outputFile.empty())
        {
            size_t begin = auxFile.find_last_of('/') + 1;
            size_t end = auxFile.find_last_of('.');
            std::string filename = auxFile.substr(begin, end - begin);
            outputFile = "../output/" + filename + ".result";
        }
        return true;
    }
};
