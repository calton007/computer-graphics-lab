#pragma once

#include "graphics.h"

inline int _getch()
{
    graphics_compat::State& s = graphics_compat::state();
    while (s.keyMessages.empty() && graphics_compat::is_open()) {
        graphics_compat::pump_messages(true);
    }
    if (s.keyMessages.empty()) {
        return 0;
    }
    int key = s.keyMessages.front();
    s.keyMessages.pop();
    return key;
}
