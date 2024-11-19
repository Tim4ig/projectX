#pragma once

#include <string>
#include <stdexcept>

#include "stacktrace.hpp"
#include "logger.hpp"
#include "settings.hpp"

#include "exception_macros.hpp"

namespace x::core::utils
{
    class Exception final : public std::exception
    {
    public:
        Exception()
        {
            message = "An exception occurred";
            stackTrace = StackTrace().GetTrace();
            ToString();
        }

        explicit Exception(std::string message)
        {
            this->message = std::move(message);
            stackTrace = StackTrace().GetTrace();
            ToString();
        }

        explicit Exception(std::string message, std::string function, const int line)
        {
            this->message = std::move(message);
            this->function = std::move(function);
            this->line = line;
            stackTrace = StackTrace().GetTrace();
            ToString();
        }

        [[nodiscard]] const char* what() const noexcept override
        {
            return fullMessage.c_str();
        }

        [[nodiscard]] std::string GetMessage() const
        {
            return message;
        }

        [[nodiscard]] std::string GetStackTrace() const
        {
            return stackTrace;
        }

        [[nodiscard]] std::string GetFunction() const
        {
            return function;
        }

        [[nodiscard]] int GetLine() const
        {
            return line;
        }

        void ToString()
        {
            fullMessage = "";

            fullMessage += "\n----------------------------------------------------------------\n";
            fullMessage += "\nAn exception has occurred!";
            fullMessage += "\nMessage: " + message;
            fullMessage += "\nIn function: " + function;
            fullMessage += "\nOn line: " + std::to_string(line);
            fullMessage += "\nIn thread: " + std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id())) + ((mainThreadId != std::this_thread::get_id()) ? " (Main thread)" :
                " \nWARNING: Exception thrown from a different thread than the main thread!");
            fullMessage += "\nStack trace:\n" + stackTrace;
            fullMessage += "\n----------------------------------------------------------------";
        }

        static void SetMainThreadId()
        {
            mainThreadId = std::this_thread::get_id();
        }

    private:
        std::string message;
        std::string stackTrace;
        std::string function;
        std::string fullMessage;
        int line = 0;
        static std::thread::id mainThreadId;
    };

    inline std::thread::id Exception::mainThreadId;
}
