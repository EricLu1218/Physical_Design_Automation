#pragma once
#include <cassert>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>

class GlobalTimer
{
    struct TimeInfo
    {
        bool isStart, isEnd;
        std::chrono::high_resolution_clock::time_point startTime, endTime;

        TimeInfo() : isStart(true), isEnd(false),
                     startTime(std::chrono::high_resolution_clock::now()),
                     endTime(std::chrono::high_resolution_clock::now())
        {
        }
    };

    std::unordered_map<std::string, TimeInfo *> timeInfos;

    template <class ToDuration = std::chrono::milliseconds>
    ToDuration getDuration(std::string const &tag)
    {
        auto timeInfo = timeInfos.at(tag);
        assert(timeInfo->isStart && timeInfo->isEnd);
        return std::chrono::duration_cast<ToDuration>(timeInfo->endTime - timeInfo->startTime);
    }

public:
    void startTimer(std::string const &tag)
    {
        if (timeInfos.find(tag) == timeInfos.end())
        {
            auto timeInfo = new TimeInfo();
            timeInfos.emplace(tag, timeInfo);
        }
        else
        {
            timeInfos.at(tag)->startTime = std::chrono::high_resolution_clock::now();
            timeInfos.at(tag)->isEnd = false;
        }
    }

    void stopTimer(std::string const &tag)
    {
        timeInfos.at(tag)->endTime = std::chrono::high_resolution_clock::now();
        timeInfos.at(tag)->isEnd = true;
    }

    void printTime(std::string const &tag)
    {
        auto duration = getDuration<>(tag).count();
        auto minute = duration / 1000 / 60;
        auto second = duration / 1000.0 - minute * 60;
        std::cerr << std::setw(15) << std::left << tag + ":" << minute << " m " << second << " s" << std::endl;
    }
};