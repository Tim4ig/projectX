#pragma once

#include <stacktrace>

namespace x::core::utils
{
    class StackTrace
    {
    public:
        StackTrace()
        {
            for (const auto stacktrace = std::stacktrace::current(); auto frame : stacktrace)
            {
                if (frame == *stacktrace.begin() || frame == *(stacktrace.begin() + 1))
                {
                    continue;
                }

                trace += "    " + frame.description() + "\n";
            }
        }

        std::string GetTrace()
        {
            return trace;
        }

    private:
        std::string trace;
    };
}
