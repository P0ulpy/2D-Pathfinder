//
// Created by Flo on 28/11/2022.
//

#ifndef PATHFINDER_LOGGER_HPP
#define PATHFINDER_LOGGER_HPP

#include <list>
#include <thread>
#include <string>
#include <mutex>
#include <unordered_map>

namespace Engine
{
    enum LogType {
        LOG_INFO,
        LOG_WARNING,
        LOG_ERROR
    };

    struct LogEntry {
        LogType type;
        std::thread::id threadId;
        std::string message;
    };

    class Logger
    {
    public:
        static std::list<LogEntry> messages;
        static void Log(const std::string_view& message);
        static void Warn(const std::string_view& message);
        static void Err(const std::string_view& message);

        static void SetThreadLabel(const std::string_view& label);
        static void SetThreadLabel(const std::string_view& label, std::thread::id threadID);
    private:
        static std::mutex _mutex;

        static std::unordered_map<std::thread::id, std::string> _threadsLabels;
    };
} // Engine

#endif //PATHFINDER_LOGGER_HPP
