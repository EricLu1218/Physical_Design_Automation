#include "GlobalTimer/GlobalTimer.hpp"
#include "Parser/Parser.hpp"
#include "Partitioner/Partitioner.hpp"
#include "ResultWriter/ResultWriter.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <net file> <cell file> <output file>\n";
        return 0;
    }

    GlobalTimer globalTimer(10 * 60 - 5);
    globalTimer.startTimer("runtime");
    globalTimer.startTimer("parse input");

    Parser parser;
    auto input = parser.parse(argv);

    globalTimer.stopTimer("parse input");
    globalTimer.startTimer("FM process");

    FM_Partitioner partitioner(input);
    auto result = partitioner.solve();

    globalTimer.stopTimer("FM process");
    globalTimer.startTimer("write output");

    result->write(argv[3]);

    globalTimer.stopTimer("write output");
    globalTimer.stopTimer("runtime");

    globalTimer.printTime("parse input");
    globalTimer.printTime("FM process");
    globalTimer.printTime("write output");
    globalTimer.printTime("runtime");
    return 0;
}
