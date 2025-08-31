#pragma once

#include "su1/core.hpp"
#include <vulkan/vulkan.h>
#include <memory>

namespace SU1 {

class HighPerformanceQuantumRenderer {
public:
    HighPerformanceQuantumRenderer();
    ~HighPerformanceQuantumRenderer();

    bool initialize(VkInstance instance, VkPhysicalDevice physical_device, VkDevice device);
    void shutdown();

    // Performance settings
    void set_target_fps(float fps);
    void set_enable_vsync_override(bool enable);
    void set_enable_adaptive_quality(bool enable);
    void set_enable_high_precision_timing(bool enable);
    void set_enable_async_compute(bool enable);
    void set_enable_gpu_timing(bool enable);
    void set_enable_frame_pacing(bool enable);

    // Quality settings
    void set_render_quality(float quality);
    void set_enable_motion_blur(bool enable);
    void set_enable_subpixel_rendering(bool enable);
    void set_enable_hdr_rendering(bool enable);
    void set_msaa_samples(int samples);

    // Performance metrics
    float get_average_frame_time_ms() const;
    float get_max_frame_time_ms() const;
    float get_min_frame_time_ms() const;
    float get_average_gpu_time_ms() const;
    float get_current_fps() const;
    int get_frames_dropped() const;
    int get_total_frames_rendered() const;

    // Render pass management
    void create_render_pass(const std::string& name);
    void begin_render_pass(const std::string& name);
    void end_render_pass(const std::string& name);

    // Pipeline management
    void create_graphics_pipeline(const std::string& name);
    void create_compute_pipeline(const std::string& name);
    void bind_pipeline(const std::string& name);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

} // namespace SU1

