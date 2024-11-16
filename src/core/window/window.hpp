
#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>

namespace x::core
{
    class window
    {
    public:
        void OpenAsync(std::wstring title, int width, int height);
    private:
        HWND m_hwnd = nullptr;

        static LRESULT __stdcall WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
    };
}
