
#pragma once

#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
#include <future>

namespace x::core
{
    class Window
    {
    public:
        ~Window();
        void OpenSync(std::wstring title, int width, int height);
        void OpenAsync(std::wstring title, int width, int height);
        bool IsOpen() const { return m_open; }
    private:
        HWND m_hwnd = nullptr;
        std::future<void> m_thread;
        bool m_open = false;

        static LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    };
}
