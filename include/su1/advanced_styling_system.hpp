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

class AdvancedStylingSystem {
public:
    AdvancedStylingSystem();
    ~AdvancedStylingSystem();

    bool initialize();
    void shutdown();

    // Theme management
    void apply_theme(const std::string& theme_name);
    std::vector<std::string> get_available_themes() const;
    std::string get_current_theme() const;

    // Custom theme creation
    void create_custom_theme(const std::string& name, const std::string& base_theme = "light");

    // Theme customization
    void modify_theme_color(const std::string& theme_name, const std::string& property, const Vec4& color);
    Vec4 get_theme_color(const std::string& theme_name, const std::string& property) const;

    // Gradient support
    void create_gradient(const std::string& name, const std::vector<Vec4>& colors,
                        const std::vector<float>& positions, const std::string& type = "linear");
    Vec4 sample_gradient(const std::string& name, float t) const;

    // Style property access
    Vec4 get_global_color(const std::string& property) const;
    float get_global_float(const std::string& property) const;
    std::string get_global_string(const std::string& property) const;

    // Style sheet management
    void create_style_sheet(const std::string& name, const std::string& description);
    void add_style_property(const std::string& stylesheet, const std::string& selector,
                           const std::string& property, const Vec4& value);
    void add_style_property(const std::string& stylesheet, const std::string& selector,
                           const std::string& property, float value);
    void add_style_property(const std::string& stylesheet, const std::string& selector,
                           const std::string& property, const std::string& value);

    // Performance metrics
    double get_last_render_time_ms() const;
    int get_total_properties() const;
    int get_active_rules() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace SU1

