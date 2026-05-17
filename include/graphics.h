#pragma once

#include <windows.h>
#include <windowsx.h>

#include <cmath>
#include <cstdlib>
#include <queue>
#include <stdexcept>
#include <string>

#ifndef WHITE
#define WHITE RGB(255, 255, 255)
#endif
#ifndef BLACK
#define BLACK RGB(0, 0, 0)
#endif
#ifndef BLUE
#define BLUE RGB(0, 0, 255)
#endif
#ifndef RED
#define RED RGB(255, 0, 0)
#endif
#ifndef GREEN
#define GREEN RGB(0, 255, 0)
#endif
#ifndef YELLOW
#define YELLOW RGB(255, 255, 0)
#endif

struct MOUSEMSG {
    UINT uMsg;
    int x;
    int y;
};

namespace graphics_compat {

struct State {
    HWND hwnd = nullptr;
    HDC hdc = nullptr;
    int width = 0;
    int height = 0;
    int originX = 0;
    int originY = 0;
    float aspectX = 1.0f;
    float aspectY = 1.0f;
    int currentX = 0;
    int currentY = 0;
    COLORREF backgroundColor = BLACK;
    COLORREF lineColor = WHITE;
    COLORREF fillColor = WHITE;
    bool closed = false;
    std::queue<MOUSEMSG> mouseMessages;
    std::queue<int> keyMessages;
};

inline State& state()
{
    static State s;
    return s;
}

inline int tx(float x)
{
    State& s = state();
    return s.originX + static_cast<int>(std::lround(x * s.aspectX));
}

inline int ty(float y)
{
    State& s = state();
    return s.originY + static_cast<int>(std::lround(y * s.aspectY));
}

inline POINT tp(const POINT& p)
{
    return POINT{ tx(static_cast<float>(p.x)), ty(static_cast<float>(p.y)) };
}

inline void require_window()
{
    State& s = state();
    if (s.hwnd == nullptr || s.hdc == nullptr || s.closed) {
        throw std::runtime_error("graphics window is not initialized");
    }
}

inline LRESULT CALLBACK wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    State& s = state();
    switch (msg) {
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
        s.mouseMessages.push(MOUSEMSG{ msg, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) });
        return 0;
    case WM_KEYDOWN:
        s.keyMessages.push(static_cast<int>(wParam));
        if (wParam == VK_ESCAPE) {
            s.closed = true;
            PostQuitMessage(0);
        }
        return 0;
    case WM_CLOSE:
        s.closed = true;
        PostQuitMessage(0);
        return 0;
    case WM_DESTROY:
        s.closed = true;
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}

inline void pump_messages(bool wait)
{
    State& s = state();
    MSG msg;
    if (wait) {
        const BOOL result = GetMessage(&msg, nullptr, 0, 0);
        if (result <= 0) {
            s.closed = true;
            return;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) {
            s.closed = true;
            return;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

inline bool is_open()
{
    pump_messages(false);
    return !state().closed;
}

inline void request_close()
{
    State& s = state();
    s.closed = true;
    PostQuitMessage(0);
}

inline HPEN make_pen()
{
    return CreatePen(PS_SOLID, 1, state().lineColor);
}

inline HBRUSH make_brush(COLORREF color)
{
    return CreateSolidBrush(color);
}

template <class Draw>
inline void with_pen(Draw draw)
{
    require_window();
    HDC hdc = state().hdc;
    HPEN pen = make_pen();
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    draw(hdc);
    SelectObject(hdc, oldPen);
    DeleteObject(pen);
}

template <class Draw>
inline void with_pen_and_brush(Draw draw)
{
    require_window();
    HDC hdc = state().hdc;
    HPEN pen = make_pen();
    HBRUSH brush = make_brush(state().fillColor);
    HGDIOBJ oldPen = SelectObject(hdc, pen);
    HGDIOBJ oldBrush = SelectObject(hdc, brush);
    draw(hdc);
    SelectObject(hdc, oldBrush);
    SelectObject(hdc, oldPen);
    DeleteObject(brush);
    DeleteObject(pen);
}

} // namespace graphics_compat

inline void initgraph(int width, int height)
{
    using namespace graphics_compat;
    State& s = state();

    HINSTANCE instance = GetModuleHandle(nullptr);
    const wchar_t* className = L"ComputerGraphicsWindow";

    static bool registered = false;
    if (!registered) {
        WNDCLASS wc{};
        wc.style = CS_DBLCLKS;
        wc.lpfnWndProc = wndproc;
        wc.hInstance = instance;
        wc.lpszClassName = className;
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
        if (!RegisterClass(&wc)) {
            throw std::runtime_error("failed to register graphics window class");
        }
        registered = true;
    }

    RECT rect{ 0, 0, width, height };
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

    s.hwnd = CreateWindowEx(
        0,
        className,
        L"Computer Graphics",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rect.right - rect.left,
        rect.bottom - rect.top,
        nullptr,
        nullptr,
        instance,
        nullptr);

    if (s.hwnd == nullptr) {
        throw std::runtime_error("failed to create graphics window");
    }

    s.hdc = GetDC(s.hwnd);
    s.width = width;
    s.height = height;
    s.originX = 0;
    s.originY = 0;
    s.aspectX = 1.0f;
    s.aspectY = 1.0f;
    s.currentX = 0;
    s.currentY = 0;
    s.closed = false;
}

inline void closegraph()
{
    using namespace graphics_compat;
    State& s = state();
    if (s.hwnd != nullptr && s.hdc != nullptr) {
        ReleaseDC(s.hwnd, s.hdc);
        s.hdc = nullptr;
    }
    if (s.hwnd != nullptr) {
        DestroyWindow(s.hwnd);
        s.hwnd = nullptr;
    }
    s.closed = true;
}

inline void cleardevice()
{
    using namespace graphics_compat;
    require_window();
    RECT rect{ 0, 0, state().width, state().height };
    HBRUSH brush = CreateSolidBrush(state().backgroundColor);
    FillRect(state().hdc, &rect, brush);
    DeleteObject(brush);
}

inline void setbkcolor(COLORREF color)
{
    graphics_compat::state().backgroundColor = color;
}

inline void setlinecolor(COLORREF color)
{
    graphics_compat::state().lineColor = color;
}

inline void setfillcolor(COLORREF color)
{
    graphics_compat::state().fillColor = color;
}

inline void moveto(int x, int y)
{
    graphics_compat::state().currentX = x;
    graphics_compat::state().currentY = y;
}

inline void lineto(int x, int y)
{
    using namespace graphics_compat;
    with_pen([&](HDC hdc) {
        MoveToEx(hdc, tx(static_cast<float>(state().currentX)), ty(static_cast<float>(state().currentY)), nullptr);
        LineTo(hdc, tx(static_cast<float>(x)), ty(static_cast<float>(y)));
    });
    state().currentX = x;
    state().currentY = y;
}

inline void line(int x1, int y1, int x2, int y2)
{
    using namespace graphics_compat;
    with_pen([&](HDC hdc) {
        MoveToEx(hdc, tx(static_cast<float>(x1)), ty(static_cast<float>(y1)), nullptr);
        LineTo(hdc, tx(static_cast<float>(x2)), ty(static_cast<float>(y2)));
    });
}

inline void rectangle(int left, int top, int right, int bottom)
{
    using namespace graphics_compat;
    with_pen([&](HDC hdc) {
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, tx(static_cast<float>(left)), ty(static_cast<float>(top)), tx(static_cast<float>(right)), ty(static_cast<float>(bottom)));
        SelectObject(hdc, oldBrush);
    });
}

inline void circle(int x, int y, int radius)
{
    using namespace graphics_compat;
    with_pen([&](HDC hdc) {
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Ellipse(hdc, tx(static_cast<float>(x - radius)), ty(static_cast<float>(y - radius)), tx(static_cast<float>(x + radius)), ty(static_cast<float>(y + radius)));
        SelectObject(hdc, oldBrush);
    });
}

inline void solidcircle(int x, int y, int radius)
{
    using namespace graphics_compat;
    with_pen_and_brush([&](HDC hdc) {
        Ellipse(hdc, tx(static_cast<float>(x - radius)), ty(static_cast<float>(y - radius)), tx(static_cast<float>(x + radius)), ty(static_cast<float>(y + radius)));
    });
}

inline void polygon(const POINT* points, int count)
{
    using namespace graphics_compat;
    with_pen([&](HDC hdc) {
        HGDIOBJ oldBrush = SelectObject(hdc, GetStockObject(NULL_BRUSH));
        POINT* transformed = new POINT[count];
        for (int i = 0; i < count; ++i) {
            transformed[i] = tp(points[i]);
        }
        Polygon(hdc, transformed, count);
        delete[] transformed;
        SelectObject(hdc, oldBrush);
    });
}

inline void fillpolygon(const POINT* points, int count)
{
    using namespace graphics_compat;
    with_pen_and_brush([&](HDC hdc) {
        POINT* transformed = new POINT[count];
        for (int i = 0; i < count; ++i) {
            transformed[i] = tp(points[i]);
        }
        Polygon(hdc, transformed, count);
        delete[] transformed;
    });
}

inline void putpixel(int x, int y, COLORREF color)
{
    using namespace graphics_compat;
    require_window();
    SetPixel(state().hdc, tx(static_cast<float>(x)), ty(static_cast<float>(y)), color);
}

inline void setorigin(int x, int y)
{
    graphics_compat::state().originX = x;
    graphics_compat::state().originY = y;
}

inline void setaspectratio(float x, float y)
{
    graphics_compat::state().aspectX = x;
    graphics_compat::state().aspectY = y;
}

inline MOUSEMSG GetMouseMsg()
{
    using namespace graphics_compat;
    State& s = state();
    while (s.mouseMessages.empty() && !s.closed) {
        pump_messages(true);
    }
    if (s.mouseMessages.empty()) {
        return MOUSEMSG{ WM_CLOSE, 0, 0 };
    }
    MOUSEMSG message = s.mouseMessages.front();
    s.mouseMessages.pop();
    return message;
}

inline bool is_graph_open()
{
    return graphics_compat::is_open();
}
