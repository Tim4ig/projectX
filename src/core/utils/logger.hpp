#pragma once

#include <iostream>
#include <chrono>
#include <mutex>
#include <fstream>
#include <format>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "core.hpp"

constexpr auto COLOR_RED = "\033[31m";
constexpr auto COLOR_GREEN = "\033[32m";
constexpr auto COLOR_YELLOW = "\033[33m";
constexpr auto COLOR_BLUE = "\033[34m";
constexpr auto COLOR_MAGENTA = "\033[35m";
constexpr auto COLOR_CYAN = "\033[36m";
constexpr auto COLOR_RESET = "\033[0m";

#define PREFIX (x::core::utils::Logger::FuncPrefix(__FUNCTION__) + " - ")

namespace x::core::utils
{
    class Logger
    {
    public:
        Logger() = default;

        void SetFile(const std::string& path)
        {
            if (path.empty()) out.m_file.close();
            out.m_file.open(path);
        }

        template <class... T>
        void Info(T... message)
        {
            m_Log("INFO", COLOR_RESET, message...);
        }

        template <class... T>
        void Error(T... message)
        {
            m_Log("ERROR", COLOR_RED, message...);
        }

        template <class... T>
        void Debug(T... message)
        {
            m_Log("DEBUG", COLOR_BLUE, message...);
        }

        template <class... T>
        void Warning(T... message)
        {
            m_Log("WARNING", COLOR_YELLOW, message...);
        }

        template <class... T>
        void Exception(T... message)
        {
            m_Log("EXCEPTION", COLOR_RED, message...);
        }

        template <class F, class... T>
        std::string F(F format, T... args)
        {
            return std::format(format, args...);
        }

        static std::string ToA(std::wstring wmessage)
        {
            const auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wmessage.c_str(), -1, nullptr, 0, nullptr, nullptr);

            std::string result(sizeNeeded, 0);
            WideCharToMultiByte(CP_UTF8, 0, wmessage.c_str(), -1, result.data(), sizeNeeded, nullptr, nullptr);
            return result;
        }

        void NL()
        {
            out << '\n';
        }

        static std::string FuncPrefix(const std::string& input)
        {
            const auto lastDoubleColon = input.rfind("::");

            if (lastDoubleColon == std::string::npos)
            {
                return "";
            }

            auto method = input.substr(lastDoubleColon + 2);
            const auto secondLastDoubleColon = input.rfind("::", lastDoubleColon - 1);

            if (secondLastDoubleColon == std::string::npos)
            {
                return method;
            }

            const auto className = input.substr(secondLastDoubleColon + 2, lastDoubleColon - secondLastDoubleColon - 2);

            return '[' + className + "::" + method + ']';
        }

    private:
        std::mutex m_mutex;

        class output
        {
        public:
            template <class T>
            output& operator<<(T t)
            {
                std::cout << t;
                if (m_file) m_file << t;
                return *this;
            }

            std::ofstream m_file;
        } out;

        static std::string m_GetDT()
        {
            const auto now = std::chrono::system_clock::now();
            const auto nowTime = std::chrono::system_clock::to_time_t(now);
            const auto localTm = *std::localtime(&nowTime);

            std::ostringstream oss;
            oss << std::put_time(&localTm, "%m/%d/%Y %I:%M:%S %p");
            return oss.str();
        }

        template <class... T>
        void m_Log(const char* type, const char* color, T... message)
        {
            std::lock_guard lock(m_mutex);

            out << color;
            out << "[" << m_GetDT() << "] ";
            out << "[" << type << "] ";
            ((out << message), ...);
            out << COLOR_RESET << '\n';
        }
    };

    inline Logger logger;
}

using x::core::utils::logger;
