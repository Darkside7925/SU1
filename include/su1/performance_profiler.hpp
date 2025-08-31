#pragma once

#include "su1/core.hpp"

namespace su1 {

class PerformanceProfiler {
public:
    PerformanceProfiler();
    ~PerformanceProfiler();

    void start_profiling();
    void stop_profiling();
    void reset();

    f64 get_average_frame_time() const;
    f64 get_fps() const;
    usize get_frame_count() const;

    void log_performance_stats();

private:
    Timer timer_;
    Vector<f64> frame_times_;
    usize frame_count_;
    f64 total_time_;
    bool profiling_;
};

} // namespace su1
