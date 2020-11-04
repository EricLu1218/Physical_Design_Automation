#include "Parser/Parser.hpp"
#include "Partitioner/Partitioner.hpp"
#include "ResultWriter/ResultWriter.hpp"
#include <ctime>
#include <iostream>

double getRuntime(timespec t[])
{
    timespec start = t[0], end = t[1], temp;
    if (end.tv_nsec - start.tv_nsec < 0)
    {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = 1000000000 + end.tv_nsec - start.tv_nsec;
    }
    else
    {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }
    return temp.tv_sec + temp.tv_nsec / 1000000000.0;
}

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <net file> <cell file> <output file>" << std::endl;
        return 0;
    }

    timespec total_timespec[2], fm_timespec[2];
    clock_gettime(CLOCK_MONOTONIC, &total_timespec[0]);
    Parser parser;
    auto input = parser.parse(argv);

    clock_gettime(CLOCK_MONOTONIC, &fm_timespec[0]);
    FM_Partitioner partitioner(input);
    auto result = partitioner.solve();
    clock_gettime(CLOCK_MONOTONIC, &fm_timespec[1]);

    result->write(argv[3]);
    clock_gettime(CLOCK_MONOTONIC, &total_timespec[1]);

    auto total_time = getRuntime(total_timespec);
    auto fm_time = getRuntime(fm_timespec);
    std::cout << "I/O time: " << total_time - fm_time << std::endl
              << "FM time:  " << fm_time << std::endl
              << "runtime:  " << total_time << std::endl;

    return 0;
}