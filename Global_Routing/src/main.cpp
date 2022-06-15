#include "GlobalTimer/GlobalTimer.hpp"
#include "Parser/Parser.hpp"
#include "Router/Router.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <modified.txt file> <result file>\n";
        return 0;
    }

    GlobalTimer globalTimer(10 * 60 * 60 - 5);
    globalTimer.startTimer("runtime");
    globalTimer.startTimer("parse input");

    Parser parser;
    auto input = parser.parse(argv);

    globalTimer.stopTimer("parse input");
    globalTimer.startTimer("routing process");

    Router router(input, globalTimer);
    auto result = router.solve();

    globalTimer.stopTimer("routing process");
    globalTimer.startTimer("write output");

    result->write(argv[2]);

    globalTimer.stopTimer("write output");
    globalTimer.stopTimer("runtime");

    globalTimer.printTime("parse input");
    globalTimer.printTime("routing process");
    globalTimer.printTime("write output");
    globalTimer.printTime("runtime");
    return 0;
}
