#pragma once

#include "su1/core.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <functional>
#include <chrono>
#include <memory>

namespace SU1 {

struct Vec2 {
    f32 x, y;
};

struct Vec3 {
    f32 x, y, z;
};

struct Vec4 {
    f32 x, y, z, w;
};

class AdvancedSettingsManager {
public:
    AdvancedSettingsManager();
    ~AdvancedSettingsManager();

    bool initialize();
    void shutdown();

    // Settings management
    bool load_settings();
    bool save_settings();

    // Profile management
    bool create_profile(const std::string& name, const std::string& description);
    bool delete_profile(const std::string& name);
    bool switch_profile(const std::string& name);
    std::vector<std::string> get_available_profiles() const;
    std::string get_current_profile() const;

    // Setting access
    template<typename T>
    T get_setting(const std::string& category, const std::string& name) const;

    template<typename T>
    bool set_setting(const std::string& category, const std::string& name, const T& value);

    // Setting information
    std::vector<std::string> get_categories() const;
    std::vector<std::string> get_settings_in_category(const std::string& category) const;
    std::string get_setting_description(const std::string& category, const std::string& name) const;
    std::string get_setting_type(const std::string& category, const std::string& name) const;
    bool requires_restart(const std::string& category, const std::string& name) const;

    // Constraints
    template<typename T>
    T get_setting_min(const std::string& category, const std::string& name) const;

    template<typename T>
    T get_setting_max(const std::string& category, const std::string& name) const;

    std::vector<std::string> get_string_options(const std::string& category, const std::string& name) const;

    // Callbacks
    void add_setting_change_callback(std::function<void(const std::string&, const std::string&)> callback);
    void add_profile_change_callback(std::function<void(const std::string&)> callback);

    // Auto-save
    void set_auto_save_enabled(bool enabled);
    void set_auto_save_interval(int seconds);

    // Performance metrics
    int get_total_settings_count() const;
    int get_modified_settings_count() const;
    double get_average_load_time_ms() const;
    double get_average_save_time_ms() const;

    // Reset functions
    void reset_category_to_defaults(const std::string& category);
    void reset_all_to_defaults();
    void reset_setting_to_default(const std::string& category, const std::string& name);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace SU1
