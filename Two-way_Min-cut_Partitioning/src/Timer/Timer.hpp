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

    std::chrono::seconds timeLimit;
    TimerComponent limitTimer;
    size_t maxTagLength;
    std::unordered_map<std::string, TimerComponent> tagToTimer;

public:
    Timer(int timeLimitInSecond)
        : timeLimit(std::chrono::seconds(timeLimitInSecond)), limitTimer(TimerComponent()), maxTagLength(0) {}

    bool overTime()
    {
        limitTimer.stop();
        return limitTimer.getDuration() >= timeLimit;
    }

    void startTimer(const std::string &tag)
    {
        tagToTimer[tag].start();
        if (maxTagLength < tag.size())
            maxTagLength = tag.size();
    }

    void stopTimer(const std::string &tag)
    {
        if (!tagToTimer.count(tag))
        {
            std::cerr << "[Error] Cannot stop timer. Timer \"" << tag << "\" never started.\n";
            return;
        }
        tagToTimer[tag].stop();
    }

    void printTime(const std::string &tag)
    {
        if (!tagToTimer.count(tag))
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
        std::cout << std::setw(maxTagLength + 2) << std::left << tag + ":" << second.count() << " s\n";
    }
};
