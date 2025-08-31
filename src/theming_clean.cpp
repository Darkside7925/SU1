#include "su1/theming.hpp"
#include "su1/core.hpp"
#include <iostream>
#include <algorithm>

namespace su1 {

ThemeManager::ThemeManager()
    : initialized_(false) {
    std::cout << "[THEMING] ThemeManager created" << std::endl;
}

ThemeManager::~ThemeManager() {
    if (initialized_) {
        shutdown();
    }
}

bool ThemeManager::initialize() {
    std::cout << "[THEMING] Initializing theme manager..." << std::endl;

    initialized_ = true;

    std::cout << "[THEMING] Theme manager initialized successfully" << std::endl;
    return true;
}

void ThemeManager::shutdown() {
    if (initialized_) {
        std::cout << "[THEMING] Shutting down theme manager..." << std::endl;

        initialized_ = false;
        std::cout << "[THEMING] Theme manager shutdown complete" << std::endl;
    }
}

bool ThemeManager::is_initialized() const {
    return initialized_;
}

// Helper functions for theming
Color adjust_color_brightness(const Color& color, f32 factor) {
    Color result = color;
    result.r *= factor;
    result.g *= factor;
    result.b *= factor;
    return result;
}

Color blend_colors(const Color& color1, const Color& color2, f32 factor) {
    Color result;
    result.r = color1.r + (color2.r - color1.r) * factor;
    result.g = color1.g + (color2.g - color1.g) * factor;
    result.b = color1.b + (color2.b - color1.b) * factor;
    result.a = color1.a + (color2.a - color1.a) * factor;
    return result;
}

f32 calculate_color_luminance(const Color& color) {
    return 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
}

f32 calculate_color_contrast_ratio(const Color& color1, const Color& color2) {
    f32 lum1 = calculate_color_luminance(color1);
    f32 lum2 = calculate_color_luminance(color2);

    f32 lighter = std::max(lum1, lum2);
    f32 darker = std::min(lum1, lum2);

    return (lighter + 0.05f) / (darker + 0.05f);
}

bool is_color_accessible(const Color& foreground, const Color& background) {
    f32 contrast_ratio = calculate_color_contrast_ratio(foreground, background);
    return contrast_ratio >= 4.5f; // WCAG AA standard
}

void apply_consciousness_color_harmony(ColorPalette& palette) {
    // Apply consciousness-based color harmony
    palette.accent = adjust_color_brightness(palette.primary, 1.2f);
}

void apply_transcendent_color_alignment(ColorPalette& palette) {
    // Apply transcendent color alignment
    palette.surface = adjust_color_brightness(palette.background, 1.1f);
}

void apply_ultimate_color_perfection(ColorPalette& palette) {
    // Apply ultimate color perfection
    palette.primary = blend_colors(palette.primary, palette.accent, 0.5f);
}

} // namespace su1
