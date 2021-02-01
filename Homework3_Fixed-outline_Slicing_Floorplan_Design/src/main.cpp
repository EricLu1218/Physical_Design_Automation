#include "GlobalTimer/GlobalTimer.hpp"
#include "Parser/Parser.hpp"
#include "Solver/Solver.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 5)
    {
        std::cerr << " Usage: " << argv[0] << " <hardblock file> <net file> <pl file> <floorplan file> <deadspace ratio>\n";
        return 0;
    }

    GlobalTimer globalTimer(20 * 60 - 5);
    globalTimer.startTimer("runtime");
    globalTimer.startTimer("parse input");

    Parser parser;
    auto input = parser.parse(argv);

    globalTimer.stopTimer("parse input");
    globalTimer.startTimer("SA process");

    SA_Solver solver(input, globalTimer);
    auto result = solver.solve();

    globalTimer.stopTimer("SA process");
    globalTimer.startTimer("write output");

    result->write(argv[4]);

    globalTimer.stopTimer("write output");
    globalTimer.stopTimer("runtime");

    globalTimer.printTime("parse input");
    globalTimer.printTime("SA process");
    globalTimer.printTime("write output");
    globalTimer.printTime("runtime");
    return 0;
}
