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
    Input::ptr input = parser.parse(argParser.hardblockFilepath, argParser.plFilepath,
                                    argParser.netFilepath, argParser.deadspaceRatio);

    timer.stopTimer("parse input");
    timer.startTimer("SA process");

    Solver solver(input.get(), timer);
    ResultWriter::ptr result = solver.solve();

    timer.stopTimer("SA process");
    timer.startTimer("write output");

    result->write(argParser.floorplanFilepath);

    timer.stopTimer("write output");
    timer.stopTimer("runtime");

    timer.printTime("parse input");
    timer.printTime("SA process");
    timer.printTime("write output");
    timer.printTime("runtime");
    return 0;
}
