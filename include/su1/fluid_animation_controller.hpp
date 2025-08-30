#pragma once

#include "su1/core.hpp"
#include <vector>
#include <string>
#include <functional>
#include <memory>

namespace SU1 {

struct Vec2 {
    f32 x, y;
};

struct Vec4 {
    f32 x, y, z, w;
};

class FluidAnimationController {
public:
    FluidAnimationController();
    ~FluidAnimationController();

    bool initialize();
    void shutdown();

    // Basic animations
    void create_animation(const std::string& id, const std::string& target_element,
                         const std::string& property, float duration,
                         const std::string& easing = "ease-out");

    void set_animation_position_values(const std::string& id, const Vec2& start, const Vec2& end);
    void set_animation_scale_values(const std::string& id, float start, float end);
    void set_animation_rotation_values(const std::string& id, float start, float end);
    void set_animation_opacity_values(const std::string& id, float start, float end);
    void set_animation_color_values(const std::string& id, const Vec4& start, const Vec4& end);
    void set_animation_size_values(const std::string& id, const Vec2& start, const Vec2& end);

    void start_animation(const std::string& id);
    void stop_animation(const std::string& id);
    void pause_animation(const std::string& id);
    void resume_animation(const std::string& id);

    // Animation callbacks
    void set_animation_complete_callback(const std::string& id, std::function<void()> callback);
    void set_animation_progress_callback(const std::string& id, std::function<void(float)> callback);

    // Spring animations
    void create_spring_animation(const std::string& id, const std::string& target_element,
                               const std::string& property, float target_value,
                               float spring_constant = 100.0f, float damping_ratio = 0.7f);

    void start_spring_animation(const std::string& id);
    void stop_spring_animation(const std::string& id);
    void set_spring_settle_callback(const std::string& id, std::function<void()> callback);

    // Gesture-based animations
    void create_gesture_animation(const std::string& id, const std::string& gesture_type,
                                const std::vector<Vec2>& gesture_path, float pressure = 1.0f);

    void start_gesture_animation(const std::string& id);
    void set_gesture_complete_callback(const std::string& id, std::function<void()> callback);

    // Animation groups
    void create_animation_group(const std::string& id, const std::vector<std::string>& animation_ids,
                              bool sequential = false);

    void start_animation_group(const std::string& id);
    void stop_animation_group(const std::string& id);
    void set_group_complete_callback(const std::string& id, std::function<void()> callback);

    // Global settings
    void set_target_fps(float fps);
    void set_global_animation_speed(float speed);
    void set_preferred_easing_function(const std::string& easing);
    void set_reduce_motion(bool reduce);

    // Quality settings
    void set_animation_quality(float quality);
    void set_enable_subpixel_rendering(bool enable);
    void set_enable_motion_blur(bool enable);
    void set_motion_blur_strength(float strength);

    // High-performance settings
    void set_enable_high_precision_timing(bool enable);
    void set_enable_gpu_acceleration(bool enable);
    void set_adaptive_quality(bool enable);
    void set_timing_precision_microseconds(float precision);
    void set_max_fps_cap(float max_fps);
    void set_min_fps_threshold(float min_fps);

    // Performance metrics
    int get_active_animations_count() const;
    int get_total_animations_processed() const;
    double get_average_frame_time_ms() const;
    double get_max_frame_time_ms() const;
    double get_min_frame_time_ms() const;
    float get_current_fps() const;

    // Adaptive quality getters
    float get_adaptive_quality_factor() const;
    bool is_high_precision_timing_enabled() const;
    bool is_gpu_acceleration_enabled() const;
    bool is_adaptive_quality_enabled() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    void animation_update_loop();
    void spring_update_loop();
    void gesture_update_loop();
};

} // namespace SU1
