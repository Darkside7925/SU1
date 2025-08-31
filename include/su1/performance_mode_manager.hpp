#pragma once

#include "su1/core.hpp"

namespace su1 {

enum class PerformanceMode {
    PowerSaving,
    Balanced,
    HighPerformance,
    Extreme
};

class PerformanceModeManager {
public:
    PerformanceModeManager();
    ~PerformanceModeManager();

    bool initialize();
    void shutdown();

    void set_performance_mode(PerformanceMode mode);
    PerformanceMode get_current_mode() const;

    void optimize_for_mode(PerformanceMode mode);

private:
    PerformanceMode current_mode_;
    bool initialized_;
};

} // namespace su1