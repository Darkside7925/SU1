#pragma once

#include "su1/core.hpp"
#include <vector>
#include <string>
#include <memory>

namespace SU1 {

struct Vec2 {
    f32 x, y;
};

struct Vec4 {
    f32 x, y, z, w;
};

class AdvancedWindowStyling {
public:
    AdvancedWindowStyling();
    ~AdvancedWindowStyling();

    bool initialize();
    void shutdown();

    // Theme management
    void apply_theme(const std::string& theme_name);
    std::vector<std::string> get_available_themes() const;
    std::string get_current_theme() const;

    // Custom style creation
    void create_custom_window_style(const std::string& style_name, const std::string& base_theme = "modern");
    void create_window_theme(const std::string& theme_name, const std::string& description);

    // Style customization
    void modify_style_color(const std::string& style_name, const std::string& property, const Vec4& color);
    void modify_style_float(const std::string& style_name, const std::string& property, float value);
    void modify_style_string(const std::string& style_name, const std::string& property, const std::string& value);
    void modify_style_bool(const std::string& style_name, const std::string& property, bool value);
    void modify_style_int(const std::string& style_name, const std::string& property, int value);

    // Preset modifications
    void set_window_transparency(const std::string& style_name, float opacity);
    void set_window_border_radius(const std::string& style_name, float radius);
    void set_window_shadow(const std::string& style_name, const Vec4& color, float blur, float offset_x, float offset_y);
    void set_title_bar_style(const std::string& style_name, const Vec4& color, float height, const Vec4& text_color);
    void set_window_glow(const std::string& style_name, const Vec4& color, float intensity, float radius);
    void set_window_gradient(const std::string& style_name, const Vec4& start_color, const Vec4& end_color, float angle);
    void set_window_blur(const std::string& style_name, float strength);
    void set_window_particles(const std::string& style_name, const std::string& particle_type, int count);
    void set_window_fractal_background(const std::string& style_name, const std::string& fractal_type);

    // Animation settings
    void set_window_animation_duration(const std::string& style_name, const std::string& animation_type, float duration);
    void set_window_animation_easing(const std::string& style_name, const std::string& easing);
    void enable_window_shake(const std::string& style_name, bool enable, float intensity = 5.0f);

    // Accessibility
    void set_high_contrast_mode(const std::string& style_name, bool enable);
    void set_focus_indicator(const std::string& style_name, const Vec4& color, float width);
    void set_keyboard_navigation(const std::string& style_name, bool enable, float speed = 10.0f);

    // Responsive design
    void set_window_size_constraints(const std::string& style_name, float min_width, float min_height,
                                   float max_width = 0.0f, float max_height = 0.0f);
    void set_auto_resize(const std::string& style_name, bool enable);
    void set_resize_border_width(const std::string& style_name, float width);

    // Performance metrics
    double get_average_render_time_ms() const;
    int get_total_styles() const;
    int get_active_windows() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace SU1
