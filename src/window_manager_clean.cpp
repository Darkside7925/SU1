#include "su1/window_manager.hpp"
#include <iostream>
#include <algorithm>
#include <memory>

namespace su1 {

WindowManager::WindowManager()
    : initialized_(false) {
    std::cout << "[WINDOW_MANAGER] WindowManager created" << std::endl;
}

WindowManager::~WindowManager() {
    if (initialized_) {
        shutdown();
    }
}

bool WindowManager::initialize() {
    std::cout << "[WINDOW_MANAGER] Initializing window manager..." << std::endl;

    initialized_ = true;

    std::cout << "[WINDOW_MANAGER] Window manager initialized successfully" << std::endl;
    return true;
}

void WindowManager::shutdown() {
    if (initialized_) {
        std::cout << "[WINDOW_MANAGER] Shutting down window manager..." << std::endl;

        windows_.clear();

        initialized_ = false;
        std::cout << "[WINDOW_MANAGER] Window manager shutdown complete" << std::endl;
    }
}

bool WindowManager::is_initialized() const {
    return initialized_;
}

} // namespace su1
