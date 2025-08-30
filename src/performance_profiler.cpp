#include "su1/performance_profiler.hpp"
#include "su1/core.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <memory>

namespace SU1 {

class PerformanceProfiler::Impl {
public:
    struct ProfileSample {
        std::string name;
        std::chrono::steady_clock::time_point start_time;
        std::chrono::steady_clock::time_point end_time;
        std::chrono::microseconds duration;
        std::string category;
        std::string thread_id;
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
        std::chrono::steady_clock::time_point last_updated;
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

    std::vector<ProfileSample> samples_;
    std::unordered_map<std::string, PerformanceCounter> counters_;
    std::vector<PerformanceReport> reports_;

    std::mutex samples_mutex_;
    std::mutex counters_mutex_;
    std::atomic<bool> profiling_enabled_;
    std::atomic<bool> active_;
    std::thread cleanup_thread_;

    // Configuration
    int max_samples_;
    int max_counter_history_;
    int cleanup_interval_seconds_;
    bool enable_detailed_logging_;

    // Statistics
    int total_samples_collected_;
    int total_counters_active_;
    double average_sample_duration_ms_;
    std::chrono::steady_clock::time_point profiling_start_time_;

public:
    Impl()
        : profiling_enabled_(true)
        , active_(true)
        , max_samples_(10000)
        , max_counter_history_(100)
        , cleanup_interval_seconds_(300) // 5 minutes
        , enable_detailed_logging_(false)
        , total_samples_collected_(0)
        , total_counters_active_(0)
        , average_sample_duration_ms_(0.0) {

        profiling_start_time_ = std::chrono::steady_clock::now();
        start_cleanup_thread();
    }

    ~Impl() {
        active_ = false;
        if (cleanup_thread_.joinable()) {
            cleanup_thread_.join();
        }
    }

    void start_cleanup_thread() {
        cleanup_thread_ = std::thread([this]() {
            while (active_) {
                std::this_thread::sleep_for(std::chrono::seconds(cleanup_interval_seconds_));
                cleanup_old_samples();
            }
        });
    }

    void begin_sample(const std::string& name, const std::string& category) {
        if (!profiling_enabled_) return;

        ProfileSample sample;
        sample.name = name;
        sample.category = category;
        sample.start_time = std::chrono::steady_clock::now();
        sample.is_complete = false;
        sample.thread_id = std::to_string(std::hash<std::thread::id>{}(std::this_thread::get_id()));

        std::lock_guard<std::mutex> lock(samples_mutex_);
        samples_.push_back(sample);
    }

    void end_sample(const std::string& name) {
        if (!profiling_enabled_) return;

        auto end_time = std::chrono::steady_clock::now();

        std::lock_guard<std::mutex> lock(samples_mutex_);
        for (auto& sample : samples_) {
            if (sample.name == name && !sample.is_complete) {
                sample.end_time = end_time;
                sample.duration = std::chrono::duration_cast<std::chrono::microseconds>(
                    end_time - sample.start_time);
                sample.is_complete = true;
                total_samples_collected_++;
                break;
            }
        }
    }

    void add_counter_sample(const std::string& name, double value, const std::string& category, const std::string& unit) {
        if (!profiling_enabled_) return;

        std::lock_guard<std::mutex> lock(counters_mutex_);

        if (counters_.find(name) == counters_.end()) {
            PerformanceCounter counter;
            counter.name = name;
            counter.category = category;
            counter.unit = unit;
            counter.min_value = value;
            counter.max_value = value;
            counter.average_value = value;
            counter.sample_count = 1;
            counter.recent_samples.reserve(max_counter_history_);
            counters_[name] = counter;
            total_counters_active_++;
        }

        auto& counter = counters_[name];
        counter.value = value;
        counter.last_updated = std::chrono::steady_clock::now();

        // Update statistics
        if (value < counter.min_value) counter.min_value = value;
        if (value > counter.max_value) counter.max_value = value;

        counter.recent_samples.push_back(value);
        if (counter.recent_samples.size() > max_counter_history_) {
            counter.recent_samples.erase(counter.recent_samples.begin());
        }

        counter.sample_count++;

        // Calculate running average
        if (counter.recent_samples.size() == 1) {
            counter.average_value = value;
        } else {
            double sum = 0.0;
            for (double sample : counter.recent_samples) {
                sum += sample;
            }
            counter.average_value = sum / counter.recent_samples.size();
        }
    }

    PerformanceReport generate_report(const std::string& report_name) {
        PerformanceReport report;
        report.report_name = report_name;
        report.generated_time = std::chrono::steady_clock::now();

        // Calculate metrics
        {
            std::lock_guard<std::mutex> lock(samples_mutex_);
            if (!samples_.empty()) {
                double total_duration = 0.0;
                int completed_samples = 0;

                for (const auto& sample : samples_) {
                    if (sample.is_complete) {
                        total_duration += sample.duration.count() / 1000.0; // Convert to milliseconds
                        completed_samples++;
                    }
                }

                if (completed_samples > 0) {
                    report.metrics["average_sample_duration_ms"] = total_duration / completed_samples;
                    report.metrics["total_samples"] = completed_samples;
                }
            }
        }

        // Counter metrics
        {
            std::lock_guard<std::mutex> lock(counters_mutex_);
            for (const auto& counter_pair : counters_) {
                const auto& counter = counter_pair.second;
                report.metrics["counter_" + counter.name + "_current"] = counter.value;
                report.metrics["counter_" + counter.name + "_average"] = counter.average_value;
                report.metrics["counter_" + counter.name + "_min"] = counter.min_value;
                report.metrics["counter_" + counter.name + "_max"] = counter.max_value;
            }
        }

        // Analyze bottlenecks
        analyze_bottlenecks(report);

        // Generate recommendations
        generate_recommendations(report);

        // Calculate overall score
        calculate_performance_score(report);

        reports_.push_back(report);
        return report;
    }

    void analyze_bottlenecks(PerformanceReport& report) {
        // Analyze samples for bottlenecks
        std::lock_guard<std::mutex> lock(samples_mutex_);

        std::unordered_map<std::string, double> category_durations;
        std::unordered_map<std::string, int> category_counts;

        for (const auto& sample : samples_) {
            if (sample.is_complete) {
                double duration_ms = sample.duration.count() / 1000.0;
                category_durations[sample.category] += duration_ms;
                category_counts[sample.category]++;
            }
        }

        // Find categories with highest average duration
        std::vector<std::pair<std::string, double>> sorted_categories;
        for (const auto& category : category_durations) {
            if (category_counts[category.first] > 0) {
                double avg_duration = category.second / category_counts[category.first];
                sorted_categories.emplace_back(category.first, avg_duration);
            }
        }

        std::sort(sorted_categories.begin(), sorted_categories.end(),
                 [](const auto& a, const auto& b) { return a.second > b.second; });

        // Top 3 bottlenecks
        for (size_t i = 0; i < std::min(size_t(3), sorted_categories.size()); ++i) {
            report.bottlenecks.push_back(sorted_categories[i].first +
                                       " (" + std::to_string(sorted_categories[i].second) + "ms avg)");
        }

        // Check for performance issues
        std::lock_guard<std::mutex> lock(counters_mutex_);
        for (const auto& counter_pair : counters_) {
            const auto& counter = counter_pair.second;

            // FPS bottleneck
            if (counter.name.find("fps") != std::string::npos && counter.average_value < 30.0) {
                report.bottlenecks.push_back("Low FPS detected: " + std::to_string(counter.average_value));
            }

            // Memory bottleneck
            if (counter.name.find("memory") != std::string::npos && counter.average_value > 80.0) {
                report.bottlenecks.push_back("High memory usage: " + std::to_string(counter.average_value) + "%");
            }

            // CPU bottleneck
            if (counter.name.find("cpu") != std::string::npos && counter.average_value > 90.0) {
                report.bottlenecks.push_back("High CPU usage: " + std::to_string(counter.average_value) + "%");
            }
        }
    }

    void generate_recommendations(PerformanceReport& report) {
        // Generate recommendations based on bottlenecks and metrics
        for (const auto& bottleneck : report.bottlenecks) {
            if (bottleneck.find("rendering") != std::string::npos) {
                report.recommendations.push_back("Consider reducing rendering quality or enabling adaptive quality");
            } else if (bottleneck.find("physics") != std::string::npos) {
                report.recommendations.push_back("Optimize physics calculations or reduce physics complexity");
            } else if (bottleneck.find("memory") != std::string::npos) {
                report.recommendations.push_back("Implement texture streaming or reduce texture resolution");
            } else if (bottleneck.find("cpu") != std::string::npos) {
                report.recommendations.push_back("Enable multi-threading or optimize CPU-intensive operations");
            } else if (bottleneck.find("fps") != std::string::npos) {
                report.recommendations.push_back("Switch to a lower performance mode or reduce visual effects");
            }
        }

        // General recommendations
        if (report.metrics.find("average_sample_duration_ms") != report.metrics.end()) {
            double avg_duration = report.metrics["average_sample_duration_ms"];
            if (avg_duration > 16.67) { // Less than 60 FPS
                report.recommendations.push_back("Performance is below 60 FPS - consider optimization");
            }
        }

        // Memory recommendations
        if (report.metrics.find("counter_memory_usage_current") != report.metrics.end()) {
            double memory_usage = report.metrics["counter_memory_usage_current"];
            if (memory_usage > 90.0) {
                report.recommendations.push_back("Memory usage is very high - consider reducing texture quality");
            }
        }
    }

    void calculate_performance_score(PerformanceReport& report) {
        double score = 100.0;

        // Deduct points for bottlenecks
        score -= report.bottlenecks.size() * 10.0;

        // Deduct points for low FPS
        if (report.metrics.find("counter_fps_current") != report.metrics.end()) {
            double fps = report.metrics["counter_fps_current"];
            if (fps < 30.0) score -= 30.0;
            else if (fps < 60.0) score -= 15.0;
        }

        // Deduct points for high resource usage
        if (report.metrics.find("counter_memory_usage_current") != report.metrics.end()) {
            double memory = report.metrics["counter_memory_usage_current"];
            if (memory > 90.0) score -= 20.0;
            else if (memory > 75.0) score -= 10.0;
        }

        if (report.metrics.find("counter_cpu_usage_current") != report.metrics.end()) {
            double cpu = report.metrics["counter_cpu_usage_current"];
            if (cpu > 90.0) score -= 20.0;
            else if (cpu > 75.0) score -= 10.0;
        }

        // Clamp score
        score = std::max(0.0, std::min(100.0, score));
        report.overall_score = score;

        // Determine rating
        if (score >= 90.0) report.performance_rating = "Excellent";
        else if (score >= 80.0) report.performance_rating = "Very Good";
        else if (score >= 70.0) report.performance_rating = "Good";
        else if (score >= 60.0) report.performance_rating = "Fair";
        else if (score >= 50.0) report.performance_rating = "Poor";
        else report.performance_rating = "Critical";
    }

    void cleanup_old_samples() {
        auto cutoff_time = std::chrono::steady_clock::now() - std::chrono::minutes(5);

        std::lock_guard<std::mutex> lock(samples_mutex_);
        samples_.erase(
            std::remove_if(samples_.begin(), samples_.end(),
                          [cutoff_time](const ProfileSample& sample) {
                              return sample.is_complete && sample.end_time < cutoff_time;
                          }),
            samples_.end()
        );
    }

    std::vector<ProfileSample> get_recent_samples(int count) const {
        std::lock_guard<std::mutex> lock(samples_mutex_);

        std::vector<ProfileSample> recent_samples;
        int start_index = std::max(0, static_cast<int>(samples_.size()) - count);

        for (int i = start_index; i < samples_.size(); ++i) {
            recent_samples.push_back(samples_[i]);
        }

        return recent_samples;
    }

    std::vector<std::string> get_active_counters() const {
        std::lock_guard<std::mutex> lock(counters_mutex_);

        std::vector<std::string> counter_names;
        for (const auto& counter_pair : counters_) {
            counter_names.push_back(counter_pair.first);
        }

        return counter_names;
    }

    PerformanceCounter get_counter_info(const std::string& name) const {
        std::lock_guard<std::mutex> lock(counters_mutex_);

        if (counters_.find(name) != counters_.end()) {
            return counters_.at(name);
        }

        return PerformanceCounter{};
    }

    void reset_statistics() {
        std::lock_guard<std::mutex> lock(samples_mutex_);
        std::lock_guard<std::mutex> lock2(counters_mutex_);

        samples_.clear();
        total_samples_collected_ = 0;
        average_sample_duration_ms_ = 0.0;

        for (auto& counter_pair : counters_) {
            auto& counter = counter_pair.second;
            counter.min_value = counter.value;
            counter.max_value = counter.value;
            counter.average_value = counter.value;
            counter.sample_count = 1;
            counter.recent_samples.clear();
            counter.recent_samples.push_back(counter.value);
        }

        profiling_start_time_ = std::chrono::steady_clock::now();
    }

    void export_profile_data(const std::string& filename) {
        // Export profiling data to file
        // Implementation would write samples and counters to file
    }

    void import_profile_data(const std::string& filename) {
        // Import profiling data from file
        // Implementation would read and load profiling data
    }
};

PerformanceProfiler::PerformanceProfiler() : impl_(std::make_unique<Impl>()) {}

PerformanceProfiler::~PerformanceProfiler() = default;

bool PerformanceProfiler::initialize() {
    return true;
}

void PerformanceProfiler::shutdown() {
    impl_->active_ = false;
}

void PerformanceProfiler::begin_sample(const std::string& name, const std::string& category) {
    impl_->begin_sample(name, category);
}

void PerformanceProfiler::end_sample(const std::string& name) {
    impl_->end_sample(name);
}

void PerformanceProfiler::add_counter_sample(const std::string& name, double value, const std::string& category, const std::string& unit) {
    impl_->add_counter_sample(name, value, category, unit);
}

PerformanceProfiler::PerformanceReport PerformanceProfiler::generate_report(const std::string& report_name) {
    return impl_->generate_report(report_name);
}

std::vector<PerformanceProfiler::ProfileSample> PerformanceProfiler::get_recent_samples(int count) const {
    return impl_->get_recent_samples(count);
}

std::vector<std::string> PerformanceProfiler::get_active_counters() const {
    return impl_->get_active_counters();
}

PerformanceProfiler::PerformanceCounter PerformanceProfiler::get_counter_info(const std::string& name) const {
    return impl_->get_counter_info(name);
}

void PerformanceProfiler::set_profiling_enabled(bool enabled) {
    impl_->profiling_enabled_ = enabled;
}

void PerformanceProfiler::set_max_samples(int max_samples) {
    impl_->max_samples_ = max_samples;
}

void PerformanceProfiler::set_cleanup_interval(int seconds) {
    impl_->cleanup_interval_seconds_ = seconds;
}

} // namespace SU1
