#include "su1/fluid_animation_controller.hpp"
#include "su1/core.hpp"
#include <vector>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cmath>
#include <chrono>

namespace SU1 {

class FluidAnimationController::Impl {
public:
    struct Animation {
        std::string id;
        std::string target_element;
        std::string property;
        enum Type { POSITION, SCALE, ROTATION, OPACITY, COLOR, SIZE } type;

        union {
            Vec2 start_position;
            Vec2 end_position;
            float start_scale;
            float end_scale;
            float start_rotation;
            float end_rotation;
            float start_opacity;
            float end_opacity;
            Vec4 start_color;
            Vec4 end_color;
            Vec2 start_size;
            Vec2 end_size;
        };

        float duration;
        float current_time;
        bool is_active;
        bool is_looping;
        bool is_reversed;
        std::string easing_function;
        std::function<void()> on_complete_callback;
        std::function<void(float)> on_progress_callback;

        std::chrono::steady_clock::time_point start_time;
        std::chrono::steady_clock::time_point last_update_time;
    };

    struct AnimationGroup {
        std::string id;
        std::vector<std::string> animation_ids;
        bool play_sequentially;
        bool is_active;
        size_t current_animation_index;
        std::function<void()> on_group_complete_callback;
        std::chrono::steady_clock::time_point start_time;
    };

    struct SpringAnimation {
        std::string id;
        std::string target_element;
        std::string property;
        float target_value;
        float current_value;
        float velocity;
        float spring_constant;
        float damping_ratio;
        float mass;
        bool is_active;
        std::function<void()> on_settle_callback;

        std::chrono::steady_clock::time_point last_update_time;
    };

    struct GestureAnimation {
        std::string id;
        std::string gesture_type;
        std::vector<Vec2> gesture_path;
        float gesture_pressure;
        bool is_active;
        std::chrono::steady_clock::time_point gesture_start_time;
        std::function<void()> on_gesture_complete_callback;
    };

    std::vector<Animation> animations_;
    std::vector<AnimationGroup> animation_groups_;
    std::vector<SpringAnimation> spring_animations_;
    std::vector<GestureAnimation> gesture_animations_;

    std::atomic<bool> active_{true};
    std::thread animation_thread_;
    std::thread spring_thread_;
    std::thread gesture_thread_;

    // Performance settings
    float target_fps_;
    float frame_time_ms_;
    bool vsync_enabled_;
    bool high_precision_timing_;
    bool adaptive_quality_;
    int max_animations_per_frame_;
    float min_fps_threshold_;
    float max_fps_cap_;

    // Quality settings
    float animation_quality_;
    bool enable_subpixel_rendering_;
    bool enable_motion_blur_;
    float motion_blur_strength_;
    bool enable_high_precision_timing_;
    bool enable_gpu_acceleration_;

    // Global animation settings
    float global_animation_speed_;
    bool reduce_motion_;
    std::string preferred_easing_function_;
    float timing_precision_microseconds_;

    // Performance metrics
    int active_animations_count_;
    int total_animations_processed_;
    double average_frame_time_ms_;
    double max_frame_time_ms_;
    double min_frame_time_ms_;
    std::chrono::steady_clock::time_point last_performance_update_;
    std::vector<double> frame_time_history_;

public:
    Impl()
        : target_fps_(540.0f)  // Ultra-high refresh rate support
        , frame_time_ms_(1000.0f / 540.0f)  // ~1.85ms per frame
        , vsync_enabled_(false)  // Disabled for raw performance
        , high_precision_timing_(true)
        , adaptive_quality_(true)  // Adaptive quality for different hardware
        , max_animations_per_frame_(1000)  // Much higher limit for high FPS
        , min_fps_threshold_(30.0f)  // Minimum acceptable FPS
        , max_fps_cap_(1000.0f)  // Support up to 1000Hz displays
        , animation_quality_(1.0f)
        , enable_subpixel_rendering_(true)
        , enable_motion_blur_(false)
        , motion_blur_strength_(0.5f)
        , enable_high_precision_timing_(true)  // Microsecond precision
        , enable_gpu_acceleration_(true)  // GPU acceleration enabled
        , global_animation_speed_(1.0f)
        , reduce_motion_(false)
        , preferred_easing_function_("ease-out")
        , timing_precision_microseconds_(1.0f)  // 1 microsecond precision
        , active_animations_count_(0)
        , total_animations_processed_(0)
        , average_frame_time_ms_(0.0)
        , max_frame_time_ms_(0.0)
        , min_frame_time_ms_(1000.0)
        , last_performance_update_() {

        frame_time_history_.reserve(1000);  // Reserve space for frame time history
        last_performance_update_ = std::chrono::steady_clock::now();
    }

    ~Impl() {
        active_ = false;
        if (animation_thread_.joinable()) animation_thread_.join();
        if (spring_thread_.joinable()) spring_thread_.join();
        if (gesture_thread_.joinable()) gesture_thread_.join();
    }

    void create_animation(const std::string& id, const std::string& target_element,
                         const std::string& property, float duration,
                         const std::string& easing = "ease-out") {
        Animation anim;
        anim.id = id;
        anim.target_element = target_element;
        anim.property = property;
        anim.duration = duration * global_animation_speed_;
        anim.current_time = 0.0f;
        anim.is_active = false;
        anim.is_looping = false;
        anim.is_reversed = false;
        anim.easing_function = easing.empty() ? preferred_easing_function_ : easing;

        // Determine animation type based on property
        if (property.find("position") != std::string::npos) {
            anim.type = Animation::POSITION;
        } else if (property.find("scale") != std::string::npos) {
            anim.type = Animation::SCALE;
        } else if (property.find("rotation") != std::string::npos) {
            anim.type = Animation::ROTATION;
        } else if (property.find("opacity") != std::string::npos) {
            anim.type = Animation::OPACITY;
        } else if (property.find("color") != std::string::npos) {
            anim.type = Animation::COLOR;
        } else if (property.find("size") != std::string::npos) {
            anim.type = Animation::SIZE;
        }

        animations_.push_back(anim);
    }

    void set_animation_values(Animation& anim, const Vec2& start, const Vec2& end) {
        if (anim.type == Animation::POSITION) {
            anim.start_position = start;
            anim.end_position = end;
        } else if (anim.type == Animation::SIZE) {
            anim.start_size = start;
            anim.end_size = end;
        }
    }

    void set_animation_values(Animation& anim, float start, float end) {
        if (anim.type == Animation::SCALE) {
            anim.start_scale = start;
            anim.end_scale = end;
        } else if (anim.type == Animation::ROTATION) {
            anim.start_rotation = start;
            anim.end_rotation = end;
        } else if (anim.type == Animation::OPACITY) {
            anim.start_opacity = start;
            anim.end_opacity = end;
        }
    }

    void set_animation_values(Animation& anim, const Vec4& start, const Vec4& end) {
        if (anim.type == Animation::COLOR) {
            anim.start_color = start;
            anim.end_color = end;
        }
    }

    void start_animation(const std::string& id) {
        for (auto& anim : animations_) {
            if (anim.id == id) {
                anim.is_active = true;
                anim.start_time = std::chrono::steady_clock::now();
                anim.last_update_time = anim.start_time;
                active_animations_count_++;
                break;
            }
        }
    }

    void stop_animation(const std::string& id) {
        for (auto& anim : animations_) {
            if (anim.id == id) {
                anim.is_active = false;
                active_animations_count_--;
                if (anim.on_complete_callback) {
                    anim.on_complete_callback();
                }
                break;
            }
        }
    }

    void update_animations(double delta_time) {
        auto current_time = std::chrono::steady_clock::now();

        for (auto& anim : animations_) {
            if (!anim.is_active) continue;

            anim.current_time += delta_time * global_animation_speed_;

            float progress = std::min(anim.current_time / anim.duration, 1.0f);
            float eased_progress = apply_easing(progress, anim.easing_function);

            // Apply animation values based on type
            apply_animation_values(anim, eased_progress);

            // Call progress callback
            if (anim.on_progress_callback) {
                anim.on_progress_callback(eased_progress);
            }

            // Check if animation is complete
            if (progress >= 1.0f) {
                if (anim.is_looping) {
                    anim.current_time = 0.0f;
                    if (anim.is_reversed) {
                        std::swap(anim.start_position, anim.end_position);
                    }
                } else {
                    anim.is_active = false;
                    active_animations_count_--;
                    if (anim.on_complete_callback) {
                        anim.on_complete_callback();
                    }
                }
            }

            anim.last_update_time = current_time;
        }
    }

    float apply_easing(float t, const std::string& easing_function) {
        if (reduce_motion_) return t;

        if (easing_function == "linear") {
            return t;
        } else if (easing_function == "ease-in") {
            return t * t;
        } else if (easing_function == "ease-out") {
            return 1.0f - (1.0f - t) * (1.0f - t);
        } else if (easing_function == "ease-in-out") {
            return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
        } else if (easing_function == "bounce") {
            if (t < 1.0f / 2.75f) {
                return 7.5625f * t * t;
            } else if (t < 2.0f / 2.75f) {
                t -= 1.5f / 2.75f;
                return 7.5625f * t * t + 0.75f;
            } else if (t < 2.5f / 2.75f) {
                t -= 2.25f / 2.75f;
                return 7.5625f * t * t + 0.9375f;
            } else {
                t -= 2.625f / 2.75f;
                return 7.5625f * t * t + 0.984375f;
            }
        } else if (easing_function == "elastic") {
            const float c4 = (2.0f * 3.14159f) / 3.0f;
            return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f :
                   -std::pow(2.0f, 10.0f * t - 10.0f) * std::sin((t * 10.0f - 10.75f) * c4);
        }

        // Default to ease-out
        return 1.0f - (1.0f - t) * (1.0f - t);
    }

    void apply_animation_values(const Animation& anim, float progress) {
        switch (anim.type) {
            case Animation::POSITION: {
                Vec2 current_pos = {
                    anim.start_position.x + (anim.end_position.x - anim.start_position.x) * progress,
                    anim.start_position.y + (anim.end_position.y - anim.start_position.y) * progress
                };
                // Apply to target element
                break;
            }
            case Animation::SCALE: {
                float current_scale = anim.start_scale + (anim.end_scale - anim.start_scale) * progress;
                // Apply to target element
                break;
            }
            case Animation::ROTATION: {
                float current_rotation = anim.start_rotation + (anim.end_rotation - anim.start_rotation) * progress;
                // Apply to target element
                break;
            }
            case Animation::OPACITY: {
                float current_opacity = anim.start_opacity + (anim.end_opacity - anim.start_opacity) * progress;
                // Apply to target element
                break;
            }
            case Animation::COLOR: {
                Vec4 current_color = {
                    anim.start_color.x + (anim.end_color.x - anim.start_color.x) * progress,
                    anim.start_color.y + (anim.end_color.y - anim.start_color.y) * progress,
                    anim.start_color.z + (anim.end_color.z - anim.start_color.z) * progress,
                    anim.start_color.w + (anim.end_color.w - anim.start_color.w) * progress
                };
                // Apply to target element
                break;
            }
            case Animation::SIZE: {
                Vec2 current_size = {
                    anim.start_size.x + (anim.end_size.x - anim.start_size.x) * progress,
                    anim.start_size.y + (anim.end_size.y - anim.start_size.y) * progress
                };
                // Apply to target element
                break;
            }
        }
    }

    void create_spring_animation(const std::string& id, const std::string& target_element,
                               const std::string& property, float target_value,
                               float spring_constant = 100.0f, float damping_ratio = 0.7f) {
        SpringAnimation spring;
        spring.id = id;
        spring.target_element = target_element;
        spring.property = property;
        spring.target_value = target_value;
        spring.current_value = 0.0f; // Will be set to current value
        spring.velocity = 0.0f;
        spring.spring_constant = spring_constant;
        spring.damping_ratio = damping_ratio;
        spring.mass = 1.0f;
        spring.is_active = false;
        spring.last_update_time = std::chrono::steady_clock::now();

        spring_animations_.push_back(spring);
    }

    void update_spring_animations(double delta_time) {
        for (auto& spring : spring_animations_) {
            if (!spring.is_active) continue;

            // Hooke's law: F = -k * x
            float displacement = spring.current_value - spring.target_value;
            float force = -spring.spring_constant * displacement;

            // Damping: F_damping = -c * v
            float damping_force = -2.0f * spring.damping_ratio * std::sqrt(spring.spring_constant * spring.mass) * spring.velocity;

            // Total force
            float total_force = force + damping_force;

            // Newton's law: F = m * a => a = F / m
            float acceleration = total_force / spring.mass;

            // Update velocity and position
            spring.velocity += acceleration * delta_time;
            spring.current_value += spring.velocity * delta_time;

            // Apply to target element
            apply_spring_value(spring);

            // Check if settled
            if (std::abs(spring.velocity) < 0.01f && std::abs(displacement) < 0.01f) {
                spring.current_value = spring.target_value;
                spring.velocity = 0.0f;
                spring.is_active = false;
                if (spring.on_settle_callback) {
                    spring.on_settle_callback();
                }
            }

            spring.last_update_time = std::chrono::steady_clock::now();
        }
    }

    void apply_spring_value(const SpringAnimation& spring) {
        // Apply spring value to target element based on property
        if (spring.property == "position_x") {
            // Apply X position
        } else if (spring.property == "position_y") {
            // Apply Y position
        } else if (spring.property == "scale") {
            // Apply scale
        } else if (spring.property == "rotation") {
            // Apply rotation
        } else if (spring.property == "opacity") {
            // Apply opacity
        }
    }

    void create_gesture_animation(const std::string& id, const std::string& gesture_type,
                                const std::vector<Vec2>& gesture_path, float pressure) {
        GestureAnimation gesture;
        gesture.id = id;
        gesture.gesture_type = gesture_type;
        gesture.gesture_path = gesture_path;
        gesture.gesture_pressure = pressure;
        gesture.is_active = false;
        gesture.gesture_start_time = std::chrono::steady_clock::now();

        gesture_animations_.push_back(gesture);
    }

    void update_gesture_animations(double delta_time) {
        for (auto& gesture : gesture_animations_) {
            if (!gesture.is_active) continue;

            // Update gesture-based animations
            // This would typically respond to user gestures like swipes, pinches, etc.

            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                current_time - gesture.gesture_start_time).count();

            if (elapsed > 1000) { // 1 second timeout
                gesture.is_active = false;
                if (gesture.on_gesture_complete_callback) {
                    gesture.on_gesture_complete_callback();
                }
            }
        }
    }

    void create_animation_group(const std::string& id, const std::vector<std::string>& animation_ids,
                              bool sequential = false) {
        AnimationGroup group;
        group.id = id;
        group.animation_ids = animation_ids;
        group.play_sequentially = sequential;
        group.is_active = false;
        group.current_animation_index = 0;
        group.start_time = std::chrono::steady_clock::now();

        animation_groups_.push_back(group);
    }

    void start_animation_group(const std::string& id) {
        for (auto& group : animation_groups_) {
            if (group.id == id) {
                group.is_active = true;
                group.current_animation_index = 0;
                group.start_time = std::chrono::steady_clock::now();

                if (!group.animation_ids.empty()) {
                    start_animation(group.animation_ids[0]);
                }
                break;
            }
        }
    }

    void update_animation_groups() {
        for (auto& group : animation_groups_) {
            if (!group.is_active) continue;

            if (group.play_sequentially) {
                // Check if current animation is complete
                bool current_complete = true;
                if (group.current_animation_index < group.animation_ids.size()) {
                    const auto& current_anim_id = group.animation_ids[group.current_animation_index];
                    for (const auto& anim : animations_) {
                        if (anim.id == current_anim_id && anim.is_active) {
                            current_complete = false;
                            break;
                        }
                    }
                }

                if (current_complete) {
                    group.current_animation_index++;
                    if (group.current_animation_index < group.animation_ids.size()) {
                        start_animation(group.animation_ids[group.current_animation_index]);
                    } else {
                        // Group complete
                        group.is_active = false;
                        if (group.on_group_complete_callback) {
                            group.on_group_complete_callback();
                        }
                    }
                }
            } else {
                // Parallel playback - check if all animations are complete
                bool all_complete = true;
                for (const auto& anim_id : group.animation_ids) {
                    for (const auto& anim : animations_) {
                        if (anim.id == anim_id && anim.is_active) {
                            all_complete = false;
                            break;
                        }
                    }
                    if (!all_complete) break;
                }

                if (all_complete) {
                    group.is_active = false;
                    if (group.on_group_complete_callback) {
                        group.on_group_complete_callback();
                    }
                }
            }
        }
    }

    void update_performance_metrics() {
        auto current_time = std::chrono::steady_clock::now();
        auto time_since_last_update = std::chrono::duration_cast<std::chrono::seconds>(
            current_time - last_performance_update_).count();

        if (time_since_last_update >= 1.0) {
            // Update metrics
            last_performance_update_ = current_time;
        }
    }

    void update_performance_metrics_advanced() {
        auto current_time = std::chrono::steady_clock::now();
        auto time_since_last_update = std::chrono::duration_cast<std::chrono::milliseconds>(
            current_time - last_performance_update_).count();

        if (time_since_last_update >= 100) {  // Update every 100ms for high-frequency monitoring
            // Calculate frame time statistics
            if (!frame_time_history_.empty()) {
                double sum = 0.0;
                max_frame_time_ms_ = 0.0;
                min_frame_time_ms_ = 1000.0;  // Reset min

                for (double frame_time : frame_time_history_) {
                    sum += frame_time;
                    if (frame_time > max_frame_time_ms_) max_frame_time_ms_ = frame_time;
                    if (frame_time < min_frame_time_ms_) min_frame_time_ms_ = frame_time;
                }

                average_frame_time_ms_ = sum / frame_time_history_.size();

                // Adaptive quality adjustment based on performance
                if (adaptive_quality_) {
                    double current_fps = 1000.0 / average_frame_time_ms_;
                    if (current_fps < min_fps_threshold_) {
                        // Reduce quality for better performance
                        animation_quality_ *= 0.95f;
                        animation_quality_ = std::max(0.1f, animation_quality_);
                    } else if (current_fps > target_fps_ * 0.9f && animation_quality_ < 1.0f) {
                        // Increase quality if performing well
                        animation_quality_ *= 1.02f;
                        animation_quality_ = std::min(1.0f, animation_quality_);
                    }
                }
            }

            // Keep only recent frame times (last 100 frames)
            if (frame_time_history_.size() > 100) {
                frame_time_history_.erase(frame_time_history_.begin(),
                                        frame_time_history_.begin() + frame_time_history_.size() - 100);
            }

            last_performance_update_ = current_time;
        }
    }

    void update_adaptive_quality(double delta_time) {
        // Record frame time for performance monitoring
        double frame_time_ms = delta_time * 1000.0;
        frame_time_history_.push_back(frame_time_ms);

        // Dynamic quality adjustment based on hardware capabilities
        double current_fps = 1000.0 / frame_time_ms;

        // Adjust animation quality based on performance
        if (current_fps < 120.0f) {
            // Lower quality for lower-end hardware
            animation_quality_ = std::max(0.3f, animation_quality_ * 0.98f);
            enable_motion_blur_ = false;
            enable_subpixel_rendering_ = false;
        } else if (current_fps > 240.0f) {
            // Higher quality for high-end hardware
            animation_quality_ = std::min(1.0f, animation_quality_ * 1.01f);
            enable_motion_blur_ = true;
            enable_subpixel_rendering_ = true;
        }

        // Adjust max animations per frame based on performance
        if (current_fps < 60.0f) {
            max_animations_per_frame_ = std::max(50, max_animations_per_frame_ / 2);
        } else if (current_fps > 300.0f) {
            max_animations_per_frame_ = std::min(2000, max_animations_per_frame_ * 2);
        }
    }
};

FluidAnimationController::FluidAnimationController() : impl_(std::make_unique<Impl>()) {}

FluidAnimationController::~FluidAnimationController() = default;

bool FluidAnimationController::initialize() {
    impl_->animation_thread_ = std::thread([this]() {
        animation_update_loop();
    });

    impl_->spring_thread_ = std::thread([this]() {
        spring_update_loop();
    });

    impl_->gesture_thread_ = std::thread([this]() {
        gesture_update_loop();
    });

    return true;
}

void FluidAnimationController::shutdown() {
    impl_->active_ = false;
    if (impl_->animation_thread_.joinable()) impl_->animation_thread_.join();
    if (impl_->spring_thread_.joinable()) impl_->spring_thread_.join();
    if (impl_->gesture_thread_.joinable()) impl_->gesture_thread_.join();
}

void FluidAnimationController::create_animation(const std::string& id, const std::string& target_element,
                                              const std::string& property, float duration,
                                              const std::string& easing) {
    impl_->create_animation(id, target_element, property, duration, easing);
}

void FluidAnimationController::start_animation(const std::string& id) {
    impl_->start_animation(id);
}

void FluidAnimationController::stop_animation(const std::string& id) {
    impl_->stop_animation(id);
}

void FluidAnimationController::create_spring_animation(const std::string& id, const std::string& target_element,
                                                     const std::string& property, float target_value,
                                                     float spring_constant, float damping_ratio) {
    impl_->create_spring_animation(id, target_element, property, target_value, spring_constant, damping_ratio);
}

void FluidAnimationController::create_animation_group(const std::string& id, const std::vector<std::string>& animation_ids,
                                                    bool sequential) {
    impl_->create_animation_group(id, animation_ids, sequential);
}

void FluidAnimationController::start_animation_group(const std::string& id) {
    impl_->start_animation_group(id);
}

void FluidAnimationController::animation_update_loop() {
    auto last_time = std::chrono::high_resolution_clock::now();

    while (impl_->active_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        double delta_time = std::chrono::duration<double>(current_time - last_time).count();

        // High-precision timing for 540Hz+ support
        if (impl_->enable_high_precision_timing_) {
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(current_time - last_time);
            delta_time = duration.count() / 1000000.0;  // Convert to seconds
        }

        // Adaptive quality based on performance
        if (impl_->adaptive_quality_) {
            update_adaptive_quality(delta_time);
        }

        impl_->update_animations(delta_time);
        impl_->update_animation_groups();
        impl_->update_performance_metrics_advanced();

        // Maintain target frame rate with microsecond precision
        double target_frame_time_seconds = 1.0 / impl_->target_fps_;
        double elapsed_seconds = delta_time;

        if (elapsed_seconds < target_frame_time_seconds) {
            double sleep_time_seconds = target_frame_time_seconds - elapsed_seconds;

            if (impl_->enable_high_precision_timing_) {
                // Use microseconds for high-precision timing
                auto sleep_duration = std::chrono::microseconds(
                    static_cast<long long>(sleep_time_seconds * 1000000.0));

                // Busy wait for last few microseconds for precision
                auto start_wait = std::chrono::high_resolution_clock::now();
                std::this_thread::sleep_for(sleep_duration * 0.9);  // Sleep for 90%

                // Busy wait for remaining time
                while (std::chrono::duration<double>(
                    std::chrono::high_resolution_clock::now() - start_wait).count() < sleep_time_seconds) {
                    // Busy wait for precision
                    std::this_thread::yield();
                }
            } else {
                std::this_thread::sleep_for(std::chrono::microseconds(
                    static_cast<long long>(sleep_time_seconds * 1000000.0)));
            }
        }

        last_time = current_time;
    }
}

void FluidAnimationController::set_enable_high_precision_timing(bool enable) {
    impl_->enable_high_precision_timing_ = enable;
}

void FluidAnimationController::set_enable_gpu_acceleration(bool enable) {
    impl_->enable_gpu_acceleration_ = enable;
}

void FluidAnimationController::set_adaptive_quality(bool enable) {
    impl_->adaptive_quality_ = enable;
}

void FluidAnimationController::set_timing_precision_microseconds(float precision) {
    impl_->timing_precision_microseconds_ = std::max(0.1f, std::min(100.0f, precision));
}

void FluidAnimationController::set_max_fps_cap(float max_fps) {
    impl_->max_fps_cap_ = std::max(60.0f, std::min(2000.0f, max_fps));
}

void FluidAnimationController::set_min_fps_threshold(float min_fps) {
    impl_->min_fps_threshold_ = std::max(10.0f, std::min(120.0f, min_fps));
}

double FluidAnimationController::get_min_frame_time_ms() const {
    return impl_->min_frame_time_ms_;
}

float FluidAnimationController::get_current_fps() const {
    if (impl_->average_frame_time_ms_ > 0.0) {
        return 1000.0f / impl_->average_frame_time_ms_;
    }
    return 0.0f;
}

float FluidAnimationController::get_adaptive_quality_factor() const {
    return impl_->animation_quality_;
}

bool FluidAnimationController::is_high_precision_timing_enabled() const {
    return impl_->enable_high_precision_timing_;
}

bool FluidAnimationController::is_gpu_acceleration_enabled() const {
    return impl_->enable_gpu_acceleration_;
}

bool FluidAnimationController::is_adaptive_quality_enabled() const {
    return impl_->adaptive_quality_;
}

void FluidAnimationController::spring_update_loop() {
    auto last_time = std::chrono::high_resolution_clock::now();

    while (impl_->active_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        double delta_time = std::chrono::duration<double>(current_time - last_time).count();
        last_time = current_time;

        impl_->update_spring_animations(delta_time);

        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
    }
}

void FluidAnimationController::gesture_update_loop() {
    while (impl_->active_) {
        impl_->update_gesture_animations(0.016); // ~60 FPS

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

} // namespace SU1
