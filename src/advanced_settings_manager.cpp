#include "su1/advanced_settings_manager.hpp"
#include "su1/core.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <thread>
#include <atomic>
#include <chrono>

namespace SU1 {

class AdvancedSettingsManager::Impl {
public:
    struct SettingsCategory {
        std::string category_name;
        std::unordered_map<std::string, SettingValue> settings;
        std::vector<std::string> setting_order;
        bool is_expanded;
        std::string description;
    };

    struct SettingValue {
        enum Type { INT, FLOAT, BOOL, STRING, COLOR, VECTOR2, VECTOR3, VECTOR4 } type;
        std::string name;
        std::string description;
        std::string unit;
        bool requires_restart;
        std::chrono::steady_clock::time_point last_modified;

        // Value storage
        union {
            int int_value;
            float float_value;
            bool bool_value;
        };
        std::string string_value;
        Vec4 color_value;
        Vec2 vector2_value;
        Vec3 vector3_value;
        Vec4 vector4_value;

        // Constraints
        union {
            int int_min;
            float float_min;
        };
        union {
            int int_max;
            float float_max;
        };
        std::vector<std::string> string_options;
        bool has_constraints;

        SettingValue() : type(INT), int_value(0), has_constraints(false) {}
    };

    struct SettingsProfile {
        std::string profile_name;
        std::string description;
        std::unordered_map<std::string, std::unordered_map<std::string, SettingValue>> profile_settings;
        std::chrono::steady_clock::time_point created;
        std::chrono::steady_clock::time_point last_used;
        bool is_default;
    };

    std::unordered_map<std::string, SettingsCategory> categories_;
    std::vector<std::string> category_order_;
    std::vector<SettingsProfile> profiles_;
    std::string current_profile_;
    std::string settings_file_path_;

    std::atomic<bool> auto_save_enabled_;
    std::atomic<int> auto_save_interval_seconds_;
    std::thread auto_save_thread_;
    std::atomic<bool> active_;

    std::vector<std::function<void(const std::string&, const std::string&)>> setting_change_callbacks_;
    std::vector<std::function<void(const std::string&)>> profile_change_callbacks_;

    // Performance metrics
    int total_settings_count_;
    int modified_settings_count_;
    double average_load_time_ms_;
    double average_save_time_ms_;
    std::chrono::steady_clock::time_point last_save_time_;
    std::chrono::steady_clock::time_point last_load_time_;

public:
    Impl() : auto_save_enabled_(true), auto_save_interval_seconds_(30), active_(true),
             total_settings_count_(0), modified_settings_count_(0),
             average_load_time_ms_(0.0), average_save_time_ms_(0.0) {
        settings_file_path_ = "su1_settings.json";
        initialize_default_settings();
        initialize_default_profiles();

        auto_save_thread_ = std::thread([this]() {
            auto_save_loop();
        });
    }

    ~Impl() {
        active_ = false;
        if (auto_save_thread_.joinable()) {
            auto_save_thread_.join();
        }
    }

    void initialize_default_settings() {
        // Appearance Settings
        create_category("appearance", "Visual appearance and theming settings");

        add_setting("appearance", "theme_mode", SettingValue::STRING, "Theme mode (light/dark/auto)", "light");
        get_setting("appearance", "theme_mode").string_options = {"light", "dark", "auto"};

        add_setting("appearance", "accent_color", SettingValue::COLOR, "Primary accent color", Vec4{0.3f, 0.6f, 1.0f, 1.0f});
        add_setting("appearance", "secondary_color", SettingValue::COLOR, "Secondary accent color", Vec4{0.8f, 0.4f, 0.2f, 1.0f});

        add_setting("appearance", "window_opacity", SettingValue::FLOAT, "Default window opacity", 0.95f);
        get_setting("appearance", "window_opacity").float_min = 0.1f;
        get_setting("appearance", "window_opacity").float_max = 1.0f;
        get_setting("appearance", "window_opacity").has_constraints = true;

        add_setting("appearance", "blur_strength", SettingValue::FLOAT, "Background blur strength", 0.8f);
        get_setting("appearance", "blur_strength").float_min = 0.0f;
        get_setting("appearance", "blur_strength").float_max = 2.0f;
        get_setting("appearance", "blur_strength").has_constraints = true;

        add_setting("appearance", "animation_speed", SettingValue::FLOAT, "Global animation speed multiplier", 1.0f);
        get_setting("appearance", "animation_speed").float_min = 0.1f;
        get_setting("appearance", "animation_speed").float_max = 3.0f;
        get_setting("appearance", "animation_speed").has_constraints = true;

        // Performance Settings
        create_category("performance", "Performance and optimization settings");

        add_setting("performance", "vsync_enabled", SettingValue::BOOL, "Enable vertical synchronization", true);
        add_setting("performance", "fps_limit", SettingValue::INT, "Maximum frame rate limit", 144);
        get_setting("performance", "fps_limit").int_min = 30;
        get_setting("performance", "fps_limit").int_max = 300;
        get_setting("performance", "fps_limit").has_constraints = true;

        add_setting("performance", "gpu_memory_limit", SettingValue::INT, "GPU memory usage limit (MB)", 1024);
        get_setting("performance", "gpu_memory_limit").int_min = 256;
        get_setting("performance", "gpu_memory_limit").int_max = 8192;
        get_setting("performance", "gpu_memory_limit").has_constraints = true;

        add_setting("performance", "cpu_threads", SettingValue::INT, "Number of CPU threads to use", 8);
        get_setting("performance", "cpu_threads").int_min = 1;
        get_setting("performance", "cpu_threads").int_max = 32;
        get_setting("performance", "cpu_threads").has_constraints = true;

        add_setting("performance", "texture_quality", SettingValue::STRING, "Texture quality level", "high");
        get_setting("performance", "texture_quality").string_options = {"low", "medium", "high", "ultra"};

        // Window Management Settings
        create_category("window_management", "Window behavior and management");

        add_setting("window_management", "window_snapping", SettingValue::BOOL, "Enable window snapping", true);
        add_setting("window_management", "snap_distance", SettingValue::INT, "Snap distance in pixels", 20);
        get_setting("window_management", "snap_distance").int_min = 0;
        get_setting("window_management", "snap_distance").int_max = 100;
        get_setting("window_management", "snap_distance").has_constraints = true;

        add_setting("window_management", "auto_maximize", SettingValue::BOOL, "Auto-maximize windows on creation", false);
        add_setting("window_management", "window_shadows", SettingValue::BOOL, "Enable window shadows", true);
        add_setting("window_management", "shadow_blur", SettingValue::FLOAT, "Shadow blur radius", 10.0f);
        get_setting("window_management", "shadow_blur").float_min = 0.0f;
        get_setting("window_management", "shadow_blur").float_max = 50.0f;
        get_setting("window_management", "shadow_blur").has_constraints = true;

        add_setting("window_management", "border_radius", SettingValue::FLOAT, "Window border radius", 8.0f);
        get_setting("window_management", "border_radius").float_min = 0.0f;
        get_setting("window_management", "border_radius").float_max = 25.0f;
        get_setting("window_management", "border_radius").has_constraints = true;

        // Input Settings
        create_category("input", "Input device and gesture settings");

        add_setting("input", "mouse_sensitivity", SettingValue::FLOAT, "Mouse sensitivity multiplier", 1.0f);
        get_setting("input", "mouse_sensitivity").float_min = 0.1f;
        get_setting("input", "mouse_sensitivity").float_max = 5.0f;
        get_setting("input", "mouse_sensitivity").has_constraints = true;

        add_setting("input", "touch_sensitivity", SettingValue::FLOAT, "Touch sensitivity multiplier", 1.0f);
        get_setting("input", "touch_sensitivity").float_min = 0.1f;
        get_setting("input", "touch_sensitivity").float_max = 3.0f;
        get_setting("input", "touch_sensitivity").has_constraints = true;

        add_setting("input", "gesture_recognition", SettingValue::BOOL, "Enable advanced gesture recognition", true);
        add_setting("input", "multi_touch_enabled", SettingValue::BOOL, "Enable multi-touch gestures", true);

        add_setting("input", "keyboard_repeat_delay", SettingValue::INT, "Keyboard repeat delay (ms)", 300);
        get_setting("input", "keyboard_repeat_delay").int_min = 100;
        get_setting("input", "keyboard_repeat_delay").int_max = 1000;
        get_setting("input", "keyboard_repeat_delay").has_constraints = true;

        // Accessibility Settings
        create_category("accessibility", "Accessibility and usability features");

        add_setting("accessibility", "high_contrast_mode", SettingValue::BOOL, "Enable high contrast mode", false);
        add_setting("accessibility", "large_text", SettingValue::BOOL, "Use large text size", false);
        add_setting("accessibility", "reduce_motion", SettingValue::BOOL, "Reduce motion and animations", false);
        add_setting("accessibility", "screen_reader", SettingValue::BOOL, "Enable screen reader support", false);

        add_setting("accessibility", "color_blind_mode", SettingValue::STRING, "Color blindness correction mode", "none");
        get_setting("accessibility", "color_blind_mode").string_options = {"none", "protanopia", "deuteranopia", "tritanopia"};

        add_setting("accessibility", "zoom_factor", SettingValue::FLOAT, "Screen zoom factor", 1.0f);
        get_setting("accessibility", "zoom_factor").float_min = 0.5f;
        get_setting("accessibility", "zoom_factor").float_max = 3.0f;
        get_setting("accessibility", "zoom_factor").has_constraints = true;

        // Advanced Settings
        create_category("advanced", "Advanced system settings");

        add_setting("advanced", "debug_mode", SettingValue::BOOL, "Enable debug mode", false);
        add_setting("advanced", "log_level", SettingValue::STRING, "Logging level", "info");
        get_setting("advanced", "log_level").string_options = {"error", "warning", "info", "debug", "trace"};

        add_setting("advanced", "quantum_optimization", SettingValue::BOOL, "Enable quantum optimization", true);
        add_setting("advanced", "fractal_complexity", SettingValue::FLOAT, "Fractal rendering complexity", 0.8f);
        get_setting("advanced", "fractal_complexity").float_min = 0.1f;
        get_setting("advanced", "fractal_complexity").float_max = 1.0f;
        get_setting("advanced", "fractal_complexity").has_constraints = true;

        add_setting("advanced", "consciousness_simulation", SettingValue::BOOL, "Enable consciousness simulation", true);
        add_setting("advanced", "neural_learning_rate", SettingValue::FLOAT, "Neural network learning rate", 0.001f);
        get_setting("advanced", "neural_learning_rate").float_min = 0.0001f;
        get_setting("advanced", "neural_learning_rate").float_max = 0.01f;
        get_setting("advanced", "neural_learning_rate").has_constraints = true;

        // High-Performance Animation Settings
        create_category("high_performance", "High-performance animation and rendering settings");

        add_setting("high_performance", "target_fps", SettingValue::INT, "Target frame rate (FPS)", 540);
        get_setting("high_performance", "target_fps").int_min = 30;
        get_setting("high_performance", "target_fps").int_max = 1000;
        get_setting("high_performance", "target_fps").has_constraints = true;

        add_setting("high_performance", "max_fps_cap", SettingValue::INT, "Maximum FPS cap", 1000);
        get_setting("high_performance", "max_fps_cap").int_min = 60;
        get_setting("high_performance", "max_fps_cap").int_max = 2000;
        get_setting("high_performance", "max_fps_cap").has_constraints = true;

        add_setting("high_performance", "min_fps_threshold", SettingValue::INT, "Minimum FPS threshold for quality adjustment", 30);
        get_setting("high_performance", "min_fps_threshold").int_min = 10;
        get_setting("high_performance", "min_fps_threshold").int_max = 120;
        get_setting("high_performance", "min_fps_threshold").has_constraints = true;

        add_setting("high_performance", "high_precision_timing", SettingValue::BOOL, "Enable microsecond-precision timing", true);
        add_setting("high_performance", "gpu_acceleration", SettingValue::BOOL, "Enable GPU acceleration for animations", true);
        add_setting("high_performance", "adaptive_quality", SettingValue::BOOL, "Enable adaptive quality based on hardware", true);

        add_setting("high_performance", "timing_precision", SettingValue::FLOAT, "Timing precision in microseconds", 1.0f);
        get_setting("high_performance", "timing_precision").float_min = 0.1f;
        get_setting("high_performance", "timing_precision").float_max = 100.0f;
        get_setting("high_performance", "timing_precision").has_constraints = true;

        add_setting("high_performance", "vsync_override", SettingValue::BOOL, "Override V-sync for raw performance", true);
        add_setting("high_performance", "motion_blur", SettingValue::BOOL, "Enable motion blur for fluidity", true);
        add_setting("high_performance", "subpixel_rendering", SettingValue::BOOL, "Enable subpixel rendering", true);

        add_setting("high_performance", "max_animations_per_frame", SettingValue::INT, "Maximum animations processed per frame", 1000);
        get_setting("high_performance", "max_animations_per_frame").int_min = 50;
        get_setting("high_performance", "max_animations_per_frame").int_max = 5000;
        get_setting("high_performance", "max_animations_per_frame").has_constraints = true;

        update_total_settings_count();
    }

    void initialize_default_profiles() {
        profiles_.clear();

        // Default profile
        SettingsProfile default_profile;
        default_profile.profile_name = "Default";
        default_profile.description = "Standard settings for optimal performance";
        default_profile.created = std::chrono::steady_clock::now();
        default_profile.last_used = std::chrono::steady_clock::now();
        default_profile.is_default = true;

        // Gaming profile
        SettingsProfile gaming_profile;
        gaming_profile.profile_name = "Gaming";
        gaming_profile.description = "Optimized for gaming performance";
        gaming_profile.created = std::chrono::steady_clock::now();
        gaming_profile.is_default = false;

        // Add gaming-specific settings
        gaming_profile.profile_settings["performance"]["vsync_enabled"] = create_setting_value(SettingValue::BOOL, false);
        gaming_profile.profile_settings["performance"]["fps_limit"] = create_setting_value(SettingValue::INT, 240);
        gaming_profile.profile_settings["appearance"]["animation_speed"] = create_setting_value(SettingValue::FLOAT, 0.8f);
        gaming_profile.profile_settings["window_management"]["window_shadows"] = create_setting_value(SettingValue::BOOL, false);

        // Productivity profile
        SettingsProfile productivity_profile;
        productivity_profile.profile_name = "Productivity";
        productivity_profile.description = "Optimized for work and productivity";
        productivity_profile.created = std::chrono::steady_clock::now();
        productivity_profile.is_default = false;

        // Add productivity-specific settings
        productivity_profile.profile_settings["appearance"]["theme_mode"] = create_setting_value(SettingValue::STRING, "light");
        productivity_profile.profile_settings["accessibility"]["large_text"] = create_setting_value(SettingValue::BOOL, true);
        productivity_profile.profile_settings["window_management"]["auto_maximize"] = create_setting_value(SettingValue::BOOL, true);

        // Minimal profile
        SettingsProfile minimal_profile;
        minimal_profile.profile_name = "Minimal";
        minimal_profile.description = "Minimal interface for distraction-free work";
        minimal_profile.created = std::chrono::steady_clock::now();
        minimal_profile.is_default = false;

        // Add minimal-specific settings
        minimal_profile.profile_settings["appearance"]["blur_strength"] = create_setting_value(SettingValue::FLOAT, 0.0f);
        minimal_profile.profile_settings["appearance"]["window_opacity"] = create_setting_value(SettingValue::FLOAT, 1.0f);
        minimal_profile.profile_settings["window_management"]["window_shadows"] = create_setting_value(SettingValue::BOOL, false);

        profiles_.push_back(default_profile);
        profiles_.push_back(gaming_profile);
        profiles_.push_back(productivity_profile);
        profiles_.push_back(minimal_profile);

        current_profile_ = "Default";
    }

    SettingValue create_setting_value(SettingValue::Type type, auto value) {
        SettingValue sv;
        sv.type = type;
        sv.last_modified = std::chrono::steady_clock::now();

        if constexpr (std::is_same_v<decltype(value), int>) {
            sv.int_value = value;
        } else if constexpr (std::is_same_v<decltype(value), float>) {
            sv.float_value = value;
        } else if constexpr (std::is_same_v<decltype(value), bool>) {
            sv.bool_value = value;
        } else if constexpr (std::is_same_v<decltype(value), std::string>) {
            sv.string_value = value;
        }

        return sv;
    }

    void create_category(const std::string& name, const std::string& description) {
        SettingsCategory category;
        category.category_name = name;
        category.description = description;
        category.is_expanded = true;
        categories_[name] = category;
        category_order_.push_back(name);
    }

    void add_setting(const std::string& category, const std::string& name, SettingValue::Type type, const std::string& description, auto default_value) {
        SettingValue setting;
        setting.type = type;
        setting.name = name;
        setting.description = description;
        setting.last_modified = std::chrono::steady_clock::now();
        setting.requires_restart = false;

        if constexpr (std::is_same_v<decltype(default_value), int>) {
            setting.int_value = default_value;
        } else if constexpr (std::is_same_v<decltype(default_value), float>) {
            setting.float_value = default_value;
        } else if constexpr (std::is_same_v<decltype(default_value), bool>) {
            setting.bool_value = default_value;
        } else if constexpr (std::is_same_v<decltype(default_value), std::string>) {
            setting.string_value = default_value;
        } else if constexpr (std::is_same_v<decltype(default_value), Vec4>) {
            setting.color_value = default_value;
        }

        categories_[category].settings[name] = setting;
        categories_[category].setting_order.push_back(name);
    }

    SettingValue& get_setting(const std::string& category, const std::string& name) {
        return categories_[category].settings[name];
    }

    void update_total_settings_count() {
        total_settings_count_ = 0;
        for (const auto& category : categories_) {
            total_settings_count_ += category.second.settings.size();
        }
    }

    void auto_save_loop() {
        while (active_) {
            if (auto_save_enabled_ && modified_settings_count_ > 0) {
                auto now = std::chrono::steady_clock::now();
                auto time_since_last_save = std::chrono::duration_cast<std::chrono::seconds>(
                    now - last_save_time_).count();

                if (time_since_last_save >= auto_save_interval_seconds_) {
                    save_settings();
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    bool save_settings() {
        try {
            auto start_time = std::chrono::high_resolution_clock::now();

            std::ofstream file(settings_file_path_);
            if (!file.is_open()) return false;

            // Save current profile
            file << "{\n";
            file << "  \"current_profile\": \"" << current_profile_ << "\",\n";
            file << "  \"categories\": {\n";

            for (size_t i = 0; i < category_order_.size(); ++i) {
                const auto& category_name = category_order_[i];
                const auto& category = categories_[category_name];

                file << "    \"" << category_name << "\": {\n";
                file << "      \"description\": \"" << category.description << "\",\n";
                file << "      \"expanded\": " << (category.is_expanded ? "true" : "false") << ",\n";
                file << "      \"settings\": {\n";

                for (size_t j = 0; j < category.setting_order.size(); ++j) {
                    const auto& setting_name = category.setting_order[j];
                    const auto& setting = category.settings.at(setting_name);

                    file << "        \"" << setting_name << "\": {\n";
                    file << "          \"type\": \"" << get_type_string(setting.type) << "\",\n";
                    file << "          \"description\": \"" << setting.description << "\",\n";
                    file << "          \"requires_restart\": " << (setting.requires_restart ? "true" : "false");

                    // Save value based on type
                    switch (setting.type) {
                        case SettingValue::INT:
                            file << ",\n          \"value\": " << setting.int_value;
                            break;
                        case SettingValue::FLOAT:
                            file << ",\n          \"value\": " << setting.float_value;
                            break;
                        case SettingValue::BOOL:
                            file << ",\n          \"value\": " << (setting.bool_value ? "true" : "false");
                            break;
                        case SettingValue::STRING:
                            file << ",\n          \"value\": \"" << setting.string_value << "\"";
                            break;
                        case SettingValue::COLOR:
                            file << ",\n          \"value\": [" << setting.color_value.x << ", "
                                 << setting.color_value.y << ", " << setting.color_value.z << ", "
                                 << setting.color_value.w << "]";
                            break;
                    }

                    file << "\n        }";

                    if (j < category.setting_order.size() - 1) {
                        file << ",";
                    }
                    file << "\n";
                }

                file << "      }\n    }";

                if (i < category_order_.size() - 1) {
                    file << ",";
                }
                file << "\n";
            }

            file << "  }\n}\n";

            auto end_time = std::chrono::high_resolution_clock::now();
            auto save_duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
            average_save_time_ms_ = (average_save_time_ms_ + save_duration.count() / 1000.0) / 2.0;

            last_save_time_ = std::chrono::steady_clock::now();
            modified_settings_count_ = 0;

            return true;
        } catch (const std::exception&) {
            return false;
        }
    }

    std::string get_type_string(SettingValue::Type type) {
        switch (type) {
            case SettingValue::INT: return "int";
            case SettingValue::FLOAT: return "float";
            case SettingValue::BOOL: return "bool";
            case SettingValue::STRING: return "string";
            case SettingValue::COLOR: return "color";
            case SettingValue::VECTOR2: return "vector2";
            case SettingValue::VECTOR3: return "vector3";
            case SettingValue::VECTOR4: return "vector4";
            default: return "unknown";
        }
    }
};

AdvancedSettingsManager::AdvancedSettingsManager() : impl_(std::make_unique<Impl>()) {}

AdvancedSettingsManager::~AdvancedSettingsManager() = default;

bool AdvancedSettingsManager::initialize() {
    return impl_->load_settings();
}

void AdvancedSettingsManager::shutdown() {
    impl_->save_settings();
}

bool AdvancedSettingsManager::load_settings() {
    return impl_->load_settings();
}

bool AdvancedSettingsManager::save_settings() {
    return impl_->save_settings();
}

} // namespace SU1
