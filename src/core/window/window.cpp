#include "window.hpp"

#include "exception.hpp"

namespace x::core
{
    Window::~Window()
    {
        m_open = false;
        ThreadManager::GetDefaultManager()->Wait(m_threadId);

        if (m_hwnd != nullptr)
        {
            DestroyWindow(m_hwnd);
        }
    };

    void Window::OpenAsync(const std::wstring& title, const int width, const int height)
    {
        m_threadId = ThreadManager::GetDefaultManager()->Start([&] { this->OpenSync(title, width, height); });
        while (ThreadManager::GetDefaultManager()->IsRunning(m_threadId) && !m_open) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    void Window::OpenSync(const std::wstring& title, const int width, const int height)
    {
        WNDCLASS wc = {};
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hInstance = GetModuleHandle(nullptr);
        wc.lpszClassName = L"XCOREWINDOW@ZXC12341234";
        wc.lpfnWndProc = WndProc;
        RegisterClass(&wc);

        m_hwnd = CreateWindowExW(
            NULL,
            wc.lpszClassName,
            title.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            width, height,
            nullptr,
            nullptr,
            wc.hInstance,
            nullptr
        );

        SetWindowLongPtrW(m_hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        ShowWindow(m_hwnd, SW_SHOWNORMAL);

        if (GetLastError() != 0)
        {
            XTHROW("Failed to create window");
        }

        m_open = true;

        MSG msg = {};
        while (GetMessageW(&msg, nullptr, 0, 0) && m_open)
        {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        m_open = false;
    }

    LRESULT Window::WndProc(const HWND hwnd, const UINT msg, const WPARAM wp, const LPARAM lp)
    {
        switch (msg)
        {
        case WM_DESTROY:
            PostQuitMessage(0);
            break;

        case WM_KEYDOWN:
        {
            if (wp == VK_RETURN || (GetAsyncKeyState(VK_MENU))) {
                std::cout << "ALT+ENTER" << std::endl;
                return 0;
            }
            break;
        }

        default:
            break;
        }

        return DefWindowProc(hwnd, msg, wp, lp);
    }
}
