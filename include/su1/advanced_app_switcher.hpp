#pragma once

#include "su1/core.hpp"
#include <vulkan/vulkan.h>
#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <chrono>
#include <functional>
#include <unordered_map>
#include <random>

namespace SU1 {

struct Vec2 {
    f32 x, y;
};

struct Vec4 {
    f32 x, y, z, w;
};

struct Rect {
    f32 x, y, width, height;
};

enum class SwitcherStyle {
    Classic,
    Modern,
    Gaming,
    Minimal
};

enum class SwitcherLayout {
    Grid,
    List,
    Carousel,
    CoverFlow
};

enum class Key {
    Tab,
    Escape,
    Enter,
    Space,
    Left,
    Right,
    Up,
    Down
};

enum class KeyModifier {
    None = 0,
    Shift = 1,
    Ctrl = 2,
    Alt = 4,
    Super = 8
};

struct KeyBinding {
    KeyModifier modifier;
    Key key;
    String action;
};

struct AppWindow {
    u64 window_id;
    String title;
    String application_name;
    String icon_path;
    bool is_minimized;
    bool is_visible;
    u32 workspace_id;
    u64 last_focus_time;
    
    Vec2 position;
    f32 scale;
    f32 opacity;
    f32 rotation;
    f32 animation_progress;
};

struct WindowPreview {
    u64 window_id;
    Vec2 position;
    Vec2 size;
    f32 scale;
    f32 opacity;
    f32 blur_radius;
    f32 glow_intensity;
    f32 border_width;
    f32 corner_radius;
    bool is_selected;
    VkImageView thumbnail_texture;
};

struct SwitcherAnimation {
    enum class Type {
        FadeIn,
        FadeOut,
        SlideIn,
        SlideOut,
        Scale,
        Rotation,
        Selection,
        FadeSlide,
        Elastic,
        Bounce
    };
    
    Type type;
    f32 duration;
    f32 start_value;
    f32 end_value;
    f32 current_value;
    f32 progress;
    bool is_active;
    size_t target_index;
    std::function<f32(f32)> easing_function;
    std::function<void()> completion_callback;
};

struct TransitionEffect {
    Vec2 position;
    Vec2 size;
    Vec4 color;
    f32 intensity;
    f32 duration;
    f32 progress;
    f32 start_value;
    f32 end_value;
    f32 current_value;
    bool is_active;
};

struct WindowThumbnail {
    u64 window_id;
    VkImageView image_view;
    std::chrono::steady_clock::time_point creation_time;
    std::chrono::steady_clock::time_point last_access_time;
    bool is_valid;
};

struct SwitcherData {
    Vec2 position;
    Vec2 size;
    f32 opacity;
    Vec4 background_color;
    bool is_visible;
};

struct AppSwitcherConfig {
    bool enable_animations;
    bool enable_previews;
    bool enable_blur_background;
    bool enable_3d_effects;
    bool enable_live_previews;
    bool enable_sound_effects;
    SwitcherStyle default_style;
    SwitcherLayout default_layout;
    f32 animation_speed;
    u32 max_visible_windows;
};

struct AppSwitcherStats {
    std::atomic<u64> switches_performed;
    std::atomic<u64> animations_rendered;
    std::atomic<u64> previews_generated;
    std::atomic<f64> render_time_ms;
    std::atomic<f64> animation_time_ms;
    std::atomic<f64> preview_time_ms;
    size_t active_windows;
    size_t cached_thumbnails;
    f64 memory_usage_mb;
};

class AdvancedAppSwitcher {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    AdvancedAppSwitcher();
    ~AdvancedAppSwitcher();

    bool initialize(VkDevice device, VkPhysicalDevice physical_device, const AppSwitcherConfig& config);
    void shutdown();

    void show();
    void hide();
    void next_window();
    void previous_window();
    void select_window();

    void set_style(SwitcherStyle style);
    void set_layout(SwitcherLayout layout);
    void set_animation_type(SwitcherAnimation::Type type);

    AppSwitcherStats get_performance_stats() const;

private:
    bool create_vulkan_resources();
    bool create_switcher_buffers();
    bool create_buffer(size_t size, VkBufferUsageFlags usage, VkBuffer* buffer, VkDeviceMemory* memory);
    
    bool initialize_default_settings();
    bool setup_key_bindings();
    
    void start_update_threads();
    void stop_update_threads();
    
    void update_loop();
    void animation_loop();
    void preview_loop();
    
    void refresh_window_list();
    void sort_windows_by_recent_usage();
    void group_windows_by_application();
    void generate_window_previews();
    Vec2 calculate_preview_position(size_t index);
    
    VkImageView get_or_create_thumbnail(u64 window_id);
    VkImageView create_window_thumbnail(u64 window_id);
    void cleanup_thumbnail_cache();
    
    void reset_selection();
    void start_show_animation();
    void start_hide_animation();
    void start_selection_animation();
    
    void update_window_list();
    void update_selection();
    void update_selection_effects();
    void update_layout();
    void update_input_handling();
    void handle_keyboard_input();
    void handle_mouse_input();
    void check_mouse_hover();
    void handle_mouse_clicks();
    bool is_point_in_rect(const Vec2& point, const Vec2& rect_pos, const Vec2& rect_size);
    
    void update_animations();
    void update_animation_progress(SwitcherAnimation& animation, f32 dt);
    void apply_animation_effects(const SwitcherAnimation& animation);
    void cleanup_finished_animations();
    
    void update_transitions();
    void update_transition_effect(TransitionEffect& effect);
    void cleanup_finished_transitions();
    
    void update_visual_effects();
    void update_glow_effects();
    void render_preview_glow(const WindowPreview& preview);
    void update_blur_effects();
    void render_background_blur();
    void update_depth_effects();
    void apply_3d_transformations();
    
    void update_window_previews();
    void update_live_thumbnail(WindowPreview& preview);
    void update_thumbnail_cache();
    void update_window_focus();
    
    void update_style_settings();
    
    f64 calculate_memory_usage() const;
    
    Vector<u64> get_active_window_list();
    String get_window_title(u64 window_id);
    String get_window_application_name(u64 window_id);
    String get_window_icon_path(u64 window_id);
    bool is_window_minimized(u64 window_id);
    bool is_window_visible(u64 window_id);
    u32 get_window_workspace(u64 window_id);
    u64 get_window_last_focus_time(u64 window_id);
    void activate_window(u64 window_id);
    void preview_window_focus(u64 window_id);
    void play_sound_effect(const String& sound_file);
    
    void cleanup_vulkan_resources();
};

} // namespace SU1

