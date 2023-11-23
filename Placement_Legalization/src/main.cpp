#include "Legalizer/Legalizer.hpp"
#include "Parser/ArgumentParser.hpp"
#include "Parser/Parser.hpp"
#include "Timer/Timer.hpp"

int main(int argc, char *argv[])
{
    ArgumentParser argParser;
    if (!argParser.parse(argc, argv))
        return 1;

    Timer timer(10 * 60 - 5);
    timer.startTimer("runtime");
    timer.startTimer("parse input");

    Parser parser;
    auto input = parser.parse(argParser.auxFilepath);

    timer.stopTimer("parse input");
    timer.startTimer("abacus process");

    Legalizer legalizer(input.get());
    auto result = legalizer.solve();

    timer.stopTimer("abacus process");
    timer.startTimer("write output");

    result->write(argParser.outputFilepath);

    timer.stopTimer("write output");
    timer.stopTimer("runtime");

    timer.printTime("parse input");
    timer.printTime("abacus process");
    timer.printTime("write output");
    timer.printTime("runtime");
    return 0;
}
