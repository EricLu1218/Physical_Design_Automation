#pragma once
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

class Timer
{
    struct TimerComponent
    {
        std::chrono::high_resolution_clock::time_point startTime, stopTime;
        bool isStopped;

        TimerComponent()
        {
            startTime = stopTime = std::chrono::high_resolution_clock::now();
            isStopped = false;
        }

        void start()
        {
            startTime = std::chrono::high_resolution_clock::now();
            isStopped = false;
        }

        void stop()
        {
            stopTime = std::chrono::high_resolution_clock::now();
            isStopped = true;
        }

        std::chrono::milliseconds getDuration()
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - startTime);
        }
    };

    std::unordered_map<std::string, TimerComponent> tagToTimer;
    std::chrono::seconds timeLimit;

public:
    Timer(int timeLimitInSecond)
        : timeLimit(std::chrono::seconds(timeLimitInSecond))
    {
        tagToTimer["_timeLimit"];
    }

    bool overTime()
    {
        tagToTimer["_timeLimit"].stop();
        return tagToTimer["_timeLimit"].getDuration() >= timeLimit;
    }

    void startTimer(const std::string &tag)
    {
        tagToTimer[tag].start();
    }

    void stopTimer(const std::string &tag)
    {
        if (tagToTimer.count(tag) == 0)
        {
            std::cerr << "[Error] Cannot stop timer. Timer \"" << tag << "\" never started.\n";
            return;
        }
        tagToTimer[tag].stop();
    }

    void printTime(const std::string &tag)
    {

        if (tagToTimer.count(tag) == 0)
        {
            std::cerr << "[Error] Cannot print time. Timer \"" << tag << "\" never started.\n";
            return;
        }
        else if (!tagToTimer[tag].isStopped)
        {
            std::cerr << "[Error] Cannot print time. Timer \"" << tag << "\" does not stop after starting.\n";
            return;
        }

        std::chrono::duration<double> second = tagToTimer[tag].getDuration();
        std::cout << std::setw(15) << std::left << tag + ":" << second.count() << " s\n";
    }
};
