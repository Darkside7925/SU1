#pragma once

#include "su1/core.hpp"

namespace su1 {

struct WindowInfo {
    String title;
    i32 x = 0;
    i32 y = 0;
    u32 width = 800;
    u32 height = 600;
    bool resizable = true;
    bool decorated = true;
    bool visible = true;
    u32 min_width = 100;
    u32 min_height = 100;
    u32 max_width = 0;  // 0 = no limit
    u32 max_height = 0; // 0 = no limit

    WindowInfo() = default;
    WindowInfo(const String& title, i32 x, i32 y, u32 width, u32 height)
        : title(title), x(x), y(y), width(width), height(height) {}
};

} // namespace su1
