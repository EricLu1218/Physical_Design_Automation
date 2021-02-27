#include "GlobalTimer/GlobalTimer.hpp"
#include "Legalizer/Legalizer.hpp"
#include "Parser/Parser.hpp"
#include "Structure/Data.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <aux file>\n";
        return 0;
    }

    GlobalTimer globalTimer(10 * 60 - 5);
    globalTimer.startTimer("runtime");
    globalTimer.startTimer("parse input");

    Parser parser;
    auto input = parser.parse(argv[1]);

    globalTimer.stopTimer("parse input");
    globalTimer.startTimer("abacus process");

    Legalizer legalizer(input);
    auto result = legalizer.solve();

    globalTimer.stopTimer("abacus process");
    globalTimer.startTimer("write output");

    result->write(argv);

    globalTimer.stopTimer("write output");
    globalTimer.stopTimer("runtime");

    globalTimer.printTime("parse input");
    globalTimer.printTime("abacus process");
    globalTimer.printTime("write output");
    globalTimer.printTime("runtime");
    return 0;
}
