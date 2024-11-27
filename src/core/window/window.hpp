#pragma once

#include "pch.h"
#include "thread/thread_manager.hpp"

namespace x::core
{
    class Window
    {
    public:
        ~Window();
        void OpenSync(const std::wstring& title, int width, int height);
        void OpenAsync(const std::wstring& title, int width, int height);
        [[nodiscard]] bool IsOpen() const { return m_open; }
        [[nodiscard]] HWND GetHandle() const { return m_hwnd; }

    private:
        thread m_threadId = 0;

        HWND m_hwnd = nullptr;
        bool m_open = false;

        static LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    };
}
