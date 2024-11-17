#pragma once

#include <string>
#include <stdexcept>

#include "stacktrace.hpp"
#include "logger.hpp"

#define THROW_MESSAGE_DEBUG(e) do { OutputDebugStringA((e).ToString().c_str()); x::core::utils::logger.Exception(PREFIX, (e).ToString()); } while (0)
#define XTHROW(message) do { const auto e = x::core::utils::Exception(message, __FUNCTION__, __LINE__); THROW_MESSAGE_DEBUG(e); throw(e); } while (0)

namespace x::core::utils
{
    class Exception final : public std::exception
    {
    public:
        Exception()
        {
            message = "An exception occurred";
            stackTrace = StackTrace().GetTrace();
        }

        explicit Exception(std::string message)
        {
            this->message = std::move(message);
            stackTrace = StackTrace().GetTrace();
        }

        explicit Exception(std::string message, std::string function, const int line)
        {
            this->message = std::move(message);
            this->function = std::move(function);
            this->line = line;
            stackTrace = StackTrace().GetTrace();
        }

        [[nodiscard]] const char* what() const noexcept override
        {
            return ToString().c_str();
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

        [[nodiscard]] std::string ToString() const
        {
            std::string fullMessage;

            fullMessage += "\n----------------------------------------------------------------\n";
            fullMessage += "\nAn exception has occurred!";
            fullMessage += "\nMessage: " + message;
            fullMessage += "\nIn function: " + function;
            fullMessage += "\nOn line: " + std::to_string(line);
            fullMessage += "\nStack trace:\n" + stackTrace;
            fullMessage += "\n----------------------------------------------------------------";
            fullMessage += "\n\n";

            return fullMessage;
        }

    private:
        std::string message;
        std::string stackTrace;
        std::string function;
        int line = 0;
    };
}
