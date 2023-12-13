#include "Parser/ArgumentParser.hpp"
#include "Parser/Parser.hpp"
#include "Router/Router.hpp"
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
    Input::ptr input = parser.parse(argParser.inputFilepath);

    timer.stopTimer("parse input");
    timer.startTimer("routing process");

    Router router(input.get(), timer);
    ResultWriter::ptr result = router.solve();

    timer.stopTimer("routing process");
    timer.startTimer("write output");

    result->write(argParser.outputFilepath);

    timer.stopTimer("write output");
    timer.stopTimer("runtime");

    timer.printTime("parse input");
    timer.printTime("routing process");
    timer.printTime("write output");
    timer.printTime("runtime");
    return 0;
}
