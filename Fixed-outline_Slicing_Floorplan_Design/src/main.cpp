#include "Parser/ArgumentParser.hpp"
#include "Parser/Parser.hpp"
#include "Solver/Solver.hpp"
#include "Timer/Timer.hpp"

int main(int argc, char *argv[])
{
    ArgumentParser argParser;
    if (!argParser.parse(argc, argv))
        return 1;

    Timer timer(20 * 60 - 5);
    timer.startTimer("runtime");
    timer.startTimer("parse input");

    Parser parser;
    auto input = parser.parse(argParser.hardblockFile, argParser.plFile,
                              argParser.netFile, argParser.deadspaceRatio);

    timer.stopTimer("parse input");
    timer.startTimer("SA process");

    Solver solver(input.get(), timer);
    auto result = solver.solve();

    timer.stopTimer("SA process");
    timer.startTimer("write output");

    result->write(argParser.floorplanFile);

    timer.stopTimer("write output");
    timer.stopTimer("runtime");

    timer.printTime("parse input");
    timer.printTime("SA process");
    timer.printTime("write output");
    timer.printTime("runtime");
    return 0;
}
