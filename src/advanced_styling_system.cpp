#include "su1/advanced_styling_system.hpp"
#include "su1/core.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cmath>

namespace SU1 {

class AdvancedStylingSystem::Impl {
public:
    struct StyleSheet {
        std::string name;
        std::string description;
        std::unordered_map<std::string, StyleProperty> properties;
        std::vector<std::string> property_order;
        std::chrono::steady_clock::time_point created;
        std::chrono::steady_clock::time_point last_modified;
        bool is_active;
    };

    struct StyleProperty {
        enum Type { COLOR, FLOAT, INT, STRING, VECTOR2, VECTOR3, VECTOR4, GRADIENT } type;
        std::string name;
        std::string description;
        std::string category;
        bool is_inherited;

        union {
            int int_value;
            float float_value;
        };
        std::string string_value;
        Vec4 color_value;
        Vec2 vector2_value;
        Vec3 vector3_value;
        Vec4 vector4_value;
        Gradient gradient_value;
    };

    struct Gradient {
        std::vector<Vec4> colors;
        std::vector<float> positions;
        enum Type { LINEAR, RADIAL, ANGULAR } gradient_type;
        Vec2 start_point;
        Vec2 end_point;
        float angle;
    };

    struct StyleRule {
        std::string selector;
        std::unordered_map<std::string, StyleProperty> properties;
        int specificity;
        bool is_active;
        std::string source_stylesheet;
    };

    struct ThemeVariant {
        std::string name;
        std::string description;
        std::unordered_map<std::string, StyleSheet> stylesheets;
        std::vector<std::string> stylesheet_order;
        bool is_dark_theme;
        Vec4 primary_color;
        Vec4 secondary_color;
        Vec4 accent_color;
        Vec4 background_color;
        Vec4 surface_color;
        Vec4 text_color;
        Vec4 text_secondary_color;
    };

    std::unordered_map<std::string, ThemeVariant> themes_;
    std::string current_theme_;
    std::vector<StyleRule> style_rules_;
    std::unordered_map<std::string, std::vector<StyleRule>> element_styles_;

    // Performance tracking
    int total_properties_;
    int active_rules_;
    double last_render_time_ms_;
    std::chrono::steady_clock::time_point last_update_time_;

public:
    Impl() : total_properties_(0), active_rules_(0), last_render_time_ms_(0.0) {
        initialize_default_themes();
        initialize_default_styles();
        current_theme_ = "default";
        last_update_time_ = std::chrono::steady_clock::now();
    }

    void initialize_default_themes() {
        // Light theme
        ThemeVariant light_theme;
        light_theme.name = "Light";
        light_theme.description = "Clean and bright theme";
        light_theme.is_dark_theme = false;
        light_theme.primary_color = {0.2f, 0.4f, 0.8f, 1.0f};
        light_theme.secondary_color = {0.6f, 0.8f, 1.0f, 1.0f};
        light_theme.accent_color = {0.9f, 0.5f, 0.1f, 1.0f};
        light_theme.background_color = {0.98f, 0.98f, 0.98f, 1.0f};
        light_theme.surface_color = {1.0f, 1.0f, 1.0f, 1.0f};
        light_theme.text_color = {0.1f, 0.1f, 0.1f, 1.0f};
        light_theme.text_secondary_color = {0.4f, 0.4f, 0.4f, 1.0f};

        // Dark theme
        ThemeVariant dark_theme;
        dark_theme.name = "Dark";
        dark_theme.description = "Modern dark theme";
        dark_theme.is_dark_theme = true;
        dark_theme.primary_color = {0.3f, 0.5f, 0.9f, 1.0f};
        dark_theme.secondary_color = {0.4f, 0.6f, 0.8f, 1.0f};
        dark_theme.accent_color = {1.0f, 0.6f, 0.2f, 1.0f};
        dark_theme.background_color = {0.1f, 0.1f, 0.1f, 1.0f};
        dark_theme.surface_color = {0.15f, 0.15f, 0.15f, 1.0f};
        dark_theme.text_color = {0.95f, 0.95f, 0.95f, 1.0f};
        dark_theme.text_secondary_color = {0.7f, 0.7f, 0.7f, 1.0f};

        // Ocean theme
        ThemeVariant ocean_theme;
        ocean_theme.name = "Ocean";
        ocean_theme.description = "Calming ocean-inspired theme";
        ocean_theme.is_dark_theme = false;
        ocean_theme.primary_color = {0.2f, 0.5f, 0.8f, 1.0f};
        ocean_theme.secondary_color = {0.4f, 0.7f, 0.9f, 1.0f};
        ocean_theme.accent_color = {0.1f, 0.8f, 0.6f, 1.0f};
        ocean_theme.background_color = {0.95f, 0.98f, 1.0f, 1.0f};
        ocean_theme.surface_color = {1.0f, 1.0f, 1.0f, 1.0f};
        ocean_theme.text_color = {0.1f, 0.2f, 0.4f, 1.0f};
        ocean_theme.text_secondary_color = {0.3f, 0.5f, 0.7f, 1.0f};

        // Forest theme
        ThemeVariant forest_theme;
        forest_theme.name = "Forest";
        forest_theme.description = "Natural forest-inspired theme";
        forest_theme.is_dark_theme = false;
        forest_theme.primary_color = {0.2f, 0.6f, 0.3f, 1.0f};
        forest_theme.secondary_color = {0.4f, 0.8f, 0.5f, 1.0f};
        forest_theme.accent_color = {0.8f, 0.4f, 0.2f, 1.0f};
        forest_theme.background_color = {0.97f, 0.98f, 0.95f, 1.0f};
        forest_theme.surface_color = {1.0f, 1.0f, 1.0f, 1.0f};
        forest_theme.text_color = {0.1f, 0.3f, 0.1f, 1.0f};
        forest_theme.text_secondary_color = {0.4f, 0.6f, 0.4f, 1.0f};

        // Neon theme
        ThemeVariant neon_theme;
        neon_theme.name = "Neon";
        neon_theme.description = "Vibrant neon cyberpunk theme";
        neon_theme.is_dark_theme = true;
        neon_theme.primary_color = {1.0f, 0.0f, 0.8f, 1.0f};
        neon_theme.secondary_color = {0.0f, 1.0f, 0.8f, 1.0f};
        neon_theme.accent_color = {1.0f, 0.8f, 0.0f, 1.0f};
        neon_theme.background_color = {0.05f, 0.05f, 0.1f, 1.0f};
        neon_theme.surface_color = {0.1f, 0.1f, 0.2f, 1.0f};
        neon_theme.text_color = {1.0f, 1.0f, 1.0f, 1.0f};
        neon_theme.text_secondary_color = {0.8f, 0.8f, 0.8f, 1.0f};

        themes_["light"] = light_theme;
        themes_["dark"] = dark_theme;
        themes_["ocean"] = ocean_theme;
        themes_["forest"] = forest_theme;
        themes_["neon"] = neon_theme;
    }

    void initialize_default_styles() {
        // Window styles
        create_style_rule("window", {
            {"background-color", create_color_property({0.9f, 0.9f, 0.9f, 1.0f})},
            {"border-radius", create_float_property(8.0f)},
            {"border-width", create_float_property(1.0f)},
            {"border-color", create_color_property({0.7f, 0.7f, 0.7f, 1.0f})},
            {"shadow-blur", create_float_property(10.0f)},
            {"shadow-color", create_color_property({0.0f, 0.0f, 0.0f, 0.3f})},
            {"shadow-offset-x", create_float_property(0.0f)},
            {"shadow-offset-y", create_float_property(2.0f)}
        });

        // Button styles
        create_style_rule("button", {
            {"background-color", create_color_property({0.8f, 0.8f, 0.8f, 1.0f})},
            {"border-radius", create_float_property(4.0f)},
            {"padding", create_vector2_property({8.0f, 4.0f})},
            {"font-size", create_float_property(14.0f)},
            {"font-weight", create_string_property("normal")},
            {"text-color", create_color_property({0.2f, 0.2f, 0.2f, 1.0f})},
            {"hover-background-color", create_color_property({0.7f, 0.7f, 0.7f, 1.0f})},
            {"active-background-color", create_color_property({0.6f, 0.6f, 0.6f, 1.0f})},
            {"transition-duration", create_float_property(0.2f)}
        });

        // Taskbar styles
        create_style_rule("taskbar", {
            {"background-color", create_color_property({0.95f, 0.95f, 0.95f, 0.9f})},
            {"height", create_float_property(40.0f)},
            {"border-radius", create_float_property(0.0f)},
            {"backdrop-blur", create_float_property(20.0f)},
            {"position", create_string_property("bottom")}
        });

        // Menu styles
        create_style_rule("menu", {
            {"background-color", create_color_property({1.0f, 1.0f, 1.0f, 0.95f})},
            {"border-radius", create_float_property(6.0f)},
            {"border-width", create_float_property(1.0f)},
            {"border-color", create_color_property({0.8f, 0.8f, 0.8f, 1.0f})},
            {"shadow-blur", create_float_property(8.0f)},
            {"shadow-color", create_color_property({0.0f, 0.0f, 0.0f, 0.2f})},
            {"item-height", create_float_property(32.0f)},
            {"item-padding", create_vector2_property({12.0f, 8.0f})},
            {"separator-color", create_color_property({0.9f, 0.9f, 0.9f, 1.0f})}
        });

        // Desktop styles
        create_style_rule("desktop", {
            {"background-type", create_string_property("gradient")},
            {"background-gradient-start", create_color_property({0.8f, 0.9f, 1.0f, 1.0f})},
            {"background-gradient-end", create_color_property({0.6f, 0.8f, 1.0f, 1.0f})},
            {"background-gradient-angle", create_float_property(45.0f)},
            {"icon-size", create_float_property(64.0f)},
            {"icon-spacing", create_float_property(20.0f)},
            {"grid-snap", create_bool_property(true)},
            {"show-hidden-files", create_bool_property(false)}
        });

        // Animation styles
        create_style_rule("animation", {
            {"window-open-duration", create_float_property(0.3f)},
            {"window-close-duration", create_float_property(0.2f)},
            {"window-minimize-duration", create_float_property(0.15f)},
            {"button-hover-duration", create_float_property(0.1f)},
            {"menu-open-duration", create_float_property(0.2f)},
            {"easing-function", create_string_property("ease-out")},
            {"animation-quality", create_string_property("high")}
        });
    }

    StyleProperty create_color_property(const Vec4& color) {
        StyleProperty prop;
        prop.type = StyleProperty::COLOR;
        prop.color_value = color;
        return prop;
    }

    StyleProperty create_float_property(float value) {
        StyleProperty prop;
        prop.type = StyleProperty::FLOAT;
        prop.float_value = value;
        return prop;
    }

    StyleProperty create_int_property(int value) {
        StyleProperty prop;
        prop.type = StyleProperty::INT;
        prop.int_value = value;
        return prop;
    }

    StyleProperty create_string_property(const std::string& value) {
        StyleProperty prop;
        prop.type = StyleProperty::STRING;
        prop.string_value = value;
        return prop;
    }

    StyleProperty create_vector2_property(const Vec2& value) {
        StyleProperty prop;
        prop.type = StyleProperty::VECTOR2;
        prop.vector2_value = value;
        return prop;
    }

    StyleProperty create_bool_property(bool value) {
        StyleProperty prop;
        prop.type = StyleProperty::INT;
        prop.int_value = value ? 1 : 0;
        return prop;
    }

    void create_style_rule(const std::string& selector, const std::unordered_map<std::string, StyleProperty>& properties) {
        StyleRule rule;
        rule.selector = selector;
        rule.properties = properties;
        rule.specificity = calculate_specificity(selector);
        rule.is_active = true;
        rule.source_stylesheet = "default";

        style_rules_.push_back(rule);
        element_styles_[selector].push_back(rule);
        active_rules_++;
    }

    int calculate_specificity(const std::string& selector) {
        int specificity = 0;
        if (selector.find("#") != std::string::npos) specificity += 100; // ID
        if (selector.find(".") != std::string::npos) specificity += 10;  // Class
        if (selector.find(" ") != std::string::npos) specificity += 1;   // Descendant
        return specificity;
    }

    void apply_theme(const std::string& theme_name) {
        if (themes_.find(theme_name) == themes_.end()) return;

        current_theme_ = theme_name;
        const auto& theme = themes_[theme_name];

        // Update global color variables
        update_global_colors(theme);

        // Update element styles based on theme
        update_element_styles(theme);

        last_update_time_ = std::chrono::steady_clock::now();
    }

    void update_global_colors(const ThemeVariant& theme) {
        // Update CSS custom properties equivalent
        set_global_property("--primary-color", theme.primary_color);
        set_global_property("--secondary-color", theme.secondary_color);
        set_global_property("--accent-color", theme.accent_color);
        set_global_property("--background-color", theme.background_color);
        set_global_property("--surface-color", theme.surface_color);
        set_global_property("--text-color", theme.text_color);
        set_global_property("--text-secondary-color", theme.text_secondary_color);
    }

    void update_element_styles(const ThemeVariant& theme) {
        // Update window styles
        if (element_styles_.find("window") != element_styles_.end()) {
            auto& window_rules = element_styles_["window"];
            for (auto& rule : window_rules) {
                if (rule.properties.find("background-color") != rule.properties.end()) {
                    rule.properties["background-color"].color_value = theme.surface_color;
                }
                if (rule.properties.find("border-color") != rule.properties.end()) {
                    Vec4 border_color = theme.is_dark_theme ?
                        Vec4{0.3f, 0.3f, 0.3f, 1.0f} : Vec4{0.8f, 0.8f, 0.8f, 1.0f};
                    rule.properties["border-color"].color_value = border_color;
                }
            }
        }

        // Update button styles
        if (element_styles_.find("button") != element_styles_.end()) {
            auto& button_rules = element_styles_["button"];
            for (auto& rule : button_rules) {
                if (rule.properties.find("background-color") != rule.properties.end()) {
                    rule.properties["background-color"].color_value = theme.secondary_color;
                }
                if (rule.properties.find("text-color") != rule.properties.end()) {
                    rule.properties["text-color"].color_value = theme.text_color;
                }
            }
        }
    }

    void set_global_property(const std::string& name, const Vec4& value) {
        // Store global CSS custom properties
        global_properties_[name] = value;
    }

    Vec4 get_global_property(const std::string& name) const {
        auto it = global_properties_.find(name);
        return it != global_properties_.end() ? it->second : Vec4{0.0f, 0.0f, 0.0f, 1.0f};
    }

    void create_custom_theme(const std::string& name, const ThemeVariant& base_theme) {
        ThemeVariant custom_theme = base_theme;
        custom_theme.name = name;
        custom_theme.created = std::chrono::steady_clock::now();
        custom_theme.last_modified = std::chrono::steady_clock::now();

        themes_[name] = custom_theme;
    }

    void modify_theme_property(const std::string& theme_name, const std::string& property, const Vec4& value) {
        if (themes_.find(theme_name) == themes_.end()) return;

        auto& theme = themes_[theme_name];

        if (property == "primary-color") theme.primary_color = value;
        else if (property == "secondary-color") theme.secondary_color = value;
        else if (property == "accent-color") theme.accent_color = value;
        else if (property == "background-color") theme.background_color = value;
        else if (property == "surface-color") theme.surface_color = value;
        else if (property == "text-color") theme.text_color = value;
        else if (property == "text-secondary-color") theme.text_secondary_color = value;

        theme.last_modified = std::chrono::steady_clock::now();

        if (theme_name == current_theme_) {
            apply_theme(theme_name);
        }
    }

    std::vector<std::string> get_available_themes() const {
        std::vector<std::string> theme_names;
        for (const auto& theme : themes_) {
            theme_names.push_back(theme.first);
        }
        return theme_names;
    }

    ThemeVariant get_current_theme() const {
        if (themes_.find(current_theme_) != themes_.end()) {
            return themes_.at(current_theme_);
        }
        return themes_.at("default");
    }

    void create_gradient(const std::string& name, const std::vector<Vec4>& colors,
                        const std::vector<float>& positions, Gradient::Type type) {
        Gradient gradient;
        gradient.colors = colors;
        gradient.positions = positions;
        gradient.gradient_type = type;
        gradient.start_point = {0.0f, 0.0f};
        gradient.end_point = {1.0f, 1.0f};
        gradient.angle = 0.0f;

        gradients_[name] = gradient;
    }

    Vec4 sample_gradient(const std::string& name, float t) const {
        auto it = gradients_.find(name);
        if (it == gradients_.end()) return Vec4{0.0f, 0.0f, 0.0f, 1.0f};

        const auto& gradient = it->second;
        if (gradient.colors.empty()) return Vec4{0.0f, 0.0f, 0.0f, 1.0f};

        // Clamp t to [0, 1]
        t = std::max(0.0f, std::min(1.0f, t));

        // Find the two colors to interpolate between
        size_t i = 0;
        for (; i < gradient.positions.size() - 1; ++i) {
            if (t <= gradient.positions[i + 1]) break;
        }

        if (i >= gradient.colors.size() - 1) return gradient.colors.back();

        float local_t = (t - gradient.positions[i]) / (gradient.positions[i + 1] - gradient.positions[i]);
        local_t = std::max(0.0f, std::min(1.0f, local_t));

        const Vec4& color1 = gradient.colors[i];
        const Vec4& color2 = gradient.colors[i + 1];

        return {
            color1.x + (color2.x - color1.x) * local_t,
            color1.y + (color2.y - color1.y) * local_t,
            color1.z + (color2.z - color1.z) * local_t,
            color1.w + (color2.w - color1.w) * local_t
        };
    }

private:
    std::unordered_map<std::string, Vec4> global_properties_;
    std::unordered_map<std::string, Gradient> gradients_;
};

AdvancedStylingSystem::AdvancedStylingSystem() : impl_(std::make_unique<Impl>()) {}

AdvancedStylingSystem::~AdvancedStylingSystem() = default;

bool AdvancedStylingSystem::initialize() {
    return true;
}

void AdvancedStylingSystem::shutdown() {
    // Cleanup resources
}

void AdvancedStylingSystem::apply_theme(const std::string& theme_name) {
    impl_->apply_theme(theme_name);
}

std::vector<std::string> AdvancedStylingSystem::get_available_themes() const {
    return impl_->get_available_themes();
}

void AdvancedStylingSystem::create_custom_theme(const std::string& name, const std::string& base_theme) {
    if (impl_->themes_.find(base_theme) != impl_->themes_.end()) {
        impl_->create_custom_theme(name, impl_->themes_[base_theme]);
    }
}

void AdvancedStylingSystem::modify_theme_color(const std::string& theme_name, const std::string& property, const Vec4& color) {
    impl_->modify_theme_property(theme_name, property, color);
}

Vec4 AdvancedStylingSystem::get_theme_color(const std::string& theme_name, const std::string& property) const {
    if (impl_->themes_.find(theme_name) != impl_->themes_.end()) {
        const auto& theme = impl_->themes_.at(theme_name);

        if (property == "primary") return theme.primary_color;
        if (property == "secondary") return theme.secondary_color;
        if (property == "accent") return theme.accent_color;
        if (property == "background") return theme.background_color;
        if (property == "surface") return theme.surface_color;
        if (property == "text") return theme.text_color;
        if (property == "text-secondary") return theme.text_secondary_color;
    }

    return {0.0f, 0.0f, 0.0f, 1.0f};
}

void AdvancedStylingSystem::create_gradient(const std::string& name, const std::vector<Vec4>& colors,
                                          const std::vector<float>& positions, const std::string& type) {
    AdvancedStylingSystem::Impl::Gradient::Type gradient_type = AdvancedStylingSystem::Impl::Gradient::LINEAR;
    if (type == "radial") gradient_type = AdvancedStylingSystem::Impl::Gradient::RADIAL;
    else if (type == "angular") gradient_type = AdvancedStylingSystem::Impl::Gradient::ANGULAR;

    impl_->create_gradient(name, colors, positions, gradient_type);
}

Vec4 AdvancedStylingSystem::sample_gradient(const std::string& name, float t) const {
    return impl_->sample_gradient(name, t);
}

} // namespace SU1
