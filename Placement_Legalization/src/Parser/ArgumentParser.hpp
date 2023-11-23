#pragma once
#include <iostream>
#include <string>
#include <unistd.h>

class ArgumentParser
{
public:
    std::string auxFilepath, outputFilepath;

    ArgumentParser() {}

    bool parse(int argc, char *argv[])
    {
        int opt;
        while ((opt = getopt(argc, argv, "o:h")) != -1)
        {
            switch (opt)
            {
            case 'o':
                outputFilepath = optarg;
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
        auxFilepath = argv[optind];

        if (outputFilepath.empty())
        {
            size_t begin = auxFilepath.find_last_of('/') + 1;
            size_t end = auxFilepath.find_last_of('.');
            std::string filename = auxFilepath.substr(begin, end - begin);
            outputFilepath = "../output/" + filename + ".result";
        }
        return true;
    }
};
