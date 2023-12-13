#include "Parser/ArgumentParser.hpp"
#include "Parser/Parser.hpp"
#include "Partitioner/Partitioner.hpp"
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
    Input::ptr input = parser.parse(argParser.cellFilepath, argParser.netFilepath);

    timer.stopTimer("parse input");
    timer.startTimer("FM process");

    Partitioner partitioner(input.get());
    ResultWriter::ptr result = partitioner.solve();

    timer.stopTimer("FM process");
    timer.startTimer("write output");

    result->write(argParser.outputFilepath);

    timer.stopTimer("write output");
    timer.stopTimer("runtime");

    timer.printTime("parse input");
    timer.printTime("FM process");
    timer.printTime("write output");
    timer.printTime("runtime");
    return 0;
}
