#pragma once

#include "su1/core.hpp"
#include <vector>
#include <string>
#include <unordered_map>
#include <chrono>
#include <memory>

namespace SU1 {

class PerformanceProfiler {
public:
    struct ProfileSample {
        std::string name;
        std::string category;
        std::string thread_id;
        std::chrono::microseconds duration;
        std::unordered_map<std::string, std::string> metadata;
        bool is_complete;
    };

    struct PerformanceCounter {
        std::string name;
        std::string category;
        std::string unit;
        double value;
        double min_value;
        double max_value;
        double average_value;
        int sample_count;
        std::vector<double> recent_samples;
    };

    struct PerformanceReport {
        std::string report_name;
        std::chrono::steady_clock::time_point generated_time;
        std::unordered_map<std::string, double> metrics;
        std::vector<std::string> bottlenecks;
        std::vector<std::string> recommendations;
        double overall_score;
        std::string performance_rating;
    };

    PerformanceProfiler();
    ~PerformanceProfiler();

    bool initialize();
    void shutdown();

    // Profiling API
    void begin_sample(const std::string& name, const std::string& category = "general");
    void end_sample(const std::string& name);

    void add_counter_sample(const std::string& name, double value,
                           const std::string& category = "general",
                           const std::string& unit = "");

    // Reporting
    PerformanceReport generate_report(const std::string& report_name = "performance_report");

    // Data access
    std::vector<ProfileSample> get_recent_samples(int count = 100) const;
    std::vector<std::string> get_active_counters() const;
    PerformanceCounter get_counter_info(const std::string& name) const;

    // Configuration
    void set_profiling_enabled(bool enabled);
    void set_max_samples(int max_samples);
    void set_cleanup_interval(int seconds);
    void set_detailed_logging(bool enabled);

    // Utility functions
    void reset_statistics();
    void export_profile_data(const std::string& filename);
    void import_profile_data(const std::string& filename);

    // Statistics
    int get_total_samples_collected() const;
    int get_active_counters_count() const;
    double get_average_sample_duration_ms() const;
    std::chrono::steady_clock::time_point get_profiling_start_time() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace SU1
