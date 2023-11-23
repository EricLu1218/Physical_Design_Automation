#pragma once
#include <iostream>
#include <string>
#include <unistd.h>

class ArgumentParser
{
public:
    std::string hardblockFilepath, netFilepath, plFilepath, floorplanFilepath;
    double deadspaceRatio;

    ArgumentParser() : deadspaceRatio(0) {}

    bool parse(int argc, char *argv[])
    {
        int opt;
        while ((opt = getopt(argc, argv, "h")) != -1)
        {
            switch (opt)
            {
            default:
                std::cerr << "Usage: " << argv[0] << " <hardblock file> <net file> <pl file> <floorplan file> <deadspace ratio>\n";
                return false;
                break;
            }
        }

        if (argc - optind != 5)
        {
            std::cerr << "Usage: " << argv[0] << " <hardblock file> <net file> <pl file> <floorplan file> <deadspace ratio>\n";
            return false;
        }
        hardblockFilepath = argv[optind];
        netFilepath = argv[optind + 1];
        plFilepath = argv[optind + 2];
        floorplanFilepath = argv[optind + 3];
        deadspaceRatio = std::stod(argv[optind + 4]);
        return true;
    }
};
