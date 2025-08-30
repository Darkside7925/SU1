#pragma once

#include "su1/core.hpp"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace SU1 {

class PerformanceModeManager {
public:
    PerformanceModeManager();
    ~PerformanceModeManager();

    bool initialize();
    void shutdown();

    // Mode management
    void set_performance_mode(const std::string& mode_name);
    std::string get_current_mode() const;
    std::vector<std::string> get_available_modes() const;

    // Custom mode creation
    void create_custom_mode(const std::string& name, const std::string& base_mode = "balanced");

    // Mode customization
    void update_mode_setting(const std::string& mode_name, const std::string& setting, int value);
    void update_mode_setting(const std::string& mode_name, const std::string& setting, float value);

    // Performance monitoring
    struct PerformanceMetrics {
        float current_fps;
        float average_fps;
        float target_fps;
        float frame_time_ms;
        float gpu_usage_percent;
        float cpu_usage_percent;
        int memory_usage_mb;
        float power_consumption_watts;
        float temperature_celsius;
        int active_animations;
        int active_particles;
    };

    PerformanceMetrics get_current_metrics() const;

    // Hardware detection
    struct HardwareProfile {
        std::string gpu_name;
        std::string cpu_name;
        int gpu_vram_mb;
        int cpu_cores;
        int ram_mb;
        float gpu_score;
        float cpu_score;
        std::string performance_class;
        std::vector<std::string> supported_features;
    };

    HardwareProfile get_hardware_profile() const;

    // Power management
    void set_power_profile(const std::string& profile_name);
    std::string get_current_power_profile() const;
    std::vector<std::string> get_available_power_profiles() const;

    // Adaptive features
    void set_adaptive_mode_enabled(bool enabled);
    void set_power_management_enabled(bool enabled);
    void set_thermal_throttle_threshold(float celsius);

    // Performance settings
    void set_monitoring_interval(int milliseconds);
    void set_min_fps_threshold(int fps);
    void set_max_fps_cap(int fps);
    void set_quality_vs_performance_ratio(float ratio);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace SU1
