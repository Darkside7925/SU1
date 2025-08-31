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

using TaskbarItemId = u64;

struct Vec2 {
    f32 x, y;
};

struct Vec4 {
    f32 x, y, z, w;
};

struct Rect {
    f32 x, y, width, height;
};

enum class TaskbarPosition {
    Top,
    Bottom,
    Left,
    Right
};

enum class TaskbarItemType {
    Application,
    Group,
    Shortcut,
    SystemItem,
    Custom
};

enum class TaskbarItemState {
    Normal,
    Active,
    Minimized,
    Hidden,
    Pinned,
    Attention
};

enum class TaskbarWidgetType {
    Clock,
    NotificationArea,
    SystemTray,
    VolumeControl,
    NetworkStatus,
    BatteryStatus,
    Search,
    WorkspaceIndicator,
    Custom
};

enum class NotificationType {
    Info,
    Warning,
    Error,
    Success
};

enum class NetworkStatus {
    Disconnected,
    Ethernet,
    WiFi
};

enum class AnimationProperty {
    Scale,
    Opacity,
    PositionX,
    PositionY,
    GlowIntensity,
    Color
};

struct TaskbarItem {
    TaskbarItemId id;
    String application_id;
    String title;
    String icon_path;
    TaskbarItemType type;
    TaskbarItemState state;
    
    Vec2 position;
    Vec2 size;
    f32 scale;
    f32 opacity;
    f32 hover_progress;
    f32 animation_progress;
    f32 glow_intensity;
    
    bool is_visible;
    bool is_pinned;
    bool is_hovered;
    bool is_pressed;
    bool is_grouped;
    bool has_windows;
    bool needs_layout_update;
    
    u32 window_count;
    u32 group_members;
    u32 badge_count;
    
    Vector<TaskbarItemId> grouped_items;
    
    Vec4 color;
    Vec4 hover_color;
    Vec4 active_color;
    Vec4 badge_color;
    
    String tooltip_text;
    f32 tooltip_delay;
    
    std::function<void()> click_callback;
    std::function<void()> right_click_callback;
};

struct TaskbarWidget {
    TaskbarItemId id;
    TaskbarWidgetType type;
    String title;
    String icon_path;
    
    Vec2 position;
    Vec2 size;
    f32 scale;
    f32 opacity;
    f32 hover_progress;
    f32 focus_progress;
    f32 glow_intensity;
    
    bool is_visible;
    bool is_interactive;
    bool is_hovered;
    bool has_focus;
    bool has_badge;
    bool is_muted;
    bool is_charging;
    bool show_percentage;
    
    u32 badge_count;
    u32 visible_icon_count;
    u32 max_visible_icons;
    u32 overflow_count;
    
    f32 progress_value;
    f32 battery_level;
    f32 signal_strength;
    f32 update_interval;
    
    String text_content;
    String placeholder_text;
    String format_string;
    
    Vec4 background_color;
    Vec4 text_color;
    Vec4 placeholder_color;
    Vec4 border_color;
    Vec4 focus_color;
    Vec4 hover_color;
    Vec4 badge_color;
    Vec4 status_color;
    
    f32 corner_radius;
    f32 border_width;
    f32 font_size;
    u32 font_weight;
    
    NetworkStatus network_status;
    
    std::chrono::steady_clock::time_point last_update_time;
    
    std::function<void()> click_callback;
};

struct TaskbarAnimation {
    TaskbarItemId target_item_id;
    AnimationProperty property;
    f32 start_value;
    f32 end_value;
    f32 current_value;
    f32 progress;
    f32 duration;
    bool is_active;
    std::function<f32(f32)> easing_function;
};

struct TaskbarNotification {
    TaskbarItemId id;
    String title;
    String message;
    String icon_path;
    NotificationType type;
    Vec4 color;
    std::chrono::steady_clock::time_point timestamp;
    bool is_visible;
    bool is_persistent;
    f32 opacity;
    f32 scale;
    std::function<void()> click_callback;
};

struct SystemTrayIcon {
    TaskbarItemId id;
    String application_id;
    String tooltip_text;
    String icon_path;
    Vec2 position;
    Vec2 size;
    bool is_visible;
    bool is_interactive;
    f32 opacity;
    std::function<void()> click_callback;
    std::function<void()> right_click_callback;
};

struct QuickAction {
    TaskbarItemId id;
    String title;
    String icon_path;
    String command;
    Vec2 position;
    Vec2 size;
    bool is_visible;
    std::function<void()> execute_callback;
};

struct WorkspaceIndicator {
    u32 workspace_id;
    Vec2 position;
    Vec2 size;
    bool is_active;
    bool is_visible;
    f32 opacity;
    f32 scale;
};

struct TaskbarTheme {
    String name;
    Vec4 background_color;
    Vec4 item_color;
    Vec4 hover_color;
    Vec4 active_color;
    Vec4 text_color;
    Vec4 accent_color;
    Vec4 border_color;
    Vec4 glow_color;
    
    f32 corner_radius;
    f32 blur_radius;
    f32 glow_intensity;
    f32 opacity;
    f32 item_spacing;
    f32 item_padding;
    
    bool enable_blur;
    bool enable_glow;
    bool enable_animations;
    bool enable_transparency;
    bool enable_shadows;
    bool enable_glass_effect;
};

struct TaskbarData {
    Rect bounds;
    TaskbarPosition position;
    f32 height;
    f32 opacity;
    Vec4 background_color;
    bool auto_hide;
    bool is_visible;
};

struct TaskbarEffect {
    TaskbarItemId target_id;
    Vec2 position;
    Vec2 size;
    Vec4 color;
    f32 intensity;
    f32 radius;
    bool is_active;
};

struct TaskbarConfig {
    bool enable_hardware_acceleration;
    bool enable_multi_threading;
    bool enable_auto_hide;
    bool enable_labels;
    bool enable_previews;
    bool enable_animations;
    bool enable_sound_effects;
    bool enable_haptic_feedback;
    f32 height;
    f32 opacity;
    TaskbarPosition position;
    String default_theme;
    u32 max_items;
    u32 max_notifications;
};

struct TaskbarStats {
    std::atomic<u64> items_rendered;
    std::atomic<u64> animations_processed;
    std::atomic<u64> notifications_shown;
    std::atomic<f64> render_time_ms;
    std::atomic<f64> animation_time_ms;
    std::atomic<f64> update_time_ms;
    size_t active_items;
    size_t active_widgets;
    size_t active_notifications;
    f64 memory_usage_mb;
};

class EnhancedTaskbar {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    EnhancedTaskbar();
    ~EnhancedTaskbar();

    bool initialize(VkDevice device, VkPhysicalDevice physical_device, const TaskbarConfig& config);
    void shutdown();

    TaskbarItemId add_application(const String& app_id, const String& title, const String& icon_path);
    void remove_application(TaskbarItemId item_id);
    void set_application_state(TaskbarItemId item_id, TaskbarItemState state);

    void show_notification(const String& title, const String& message, NotificationType type);

    void set_position(TaskbarPosition position);
    void set_theme(const TaskbarTheme& theme);
    void set_auto_hide(bool enabled);
    void set_show_labels(bool enabled);
    void set_opacity(f32 opacity);

    TaskbarStats get_performance_stats() const;

private:
    bool create_vulkan_resources();
    bool create_taskbar_buffers();
    bool create_buffer(size_t size, VkBufferUsageFlags usage, VkBuffer* buffer, VkDeviceMemory* memory);
    
    bool initialize_default_theme();
    bool setup_default_layout();
    void setup_pinned_applications();
    void setup_system_area();
    void setup_workspace_indicators();
    
    bool create_system_widgets();
    void create_clock_widget();
    void create_notification_widget();
    void create_system_tray_widget();
    void create_search_widget();
    void create_volume_widget();
    void create_network_widget();
    void create_battery_widget();
    
    void start_taskbar_threads();
    void stop_taskbar_threads();
    
    void taskbar_update_loop();
    void animation_update_loop();
    void notification_update_loop();
    
    void update_taskbar_items();
    void update_item_state(TaskbarItem& item);
    void update_item_position(TaskbarItem& item);
    void update_item_appearance(TaskbarItem& item);
    void update_window_grouping();
    void create_application_group(const String& app_id, const Vector<TaskbarItem*>& items);
    void cleanup_inactive_items();
    
    void update_widgets();
    void update_widget_content(TaskbarWidget& widget);
    void update_clock_widget(TaskbarWidget& widget);
    void update_notification_widget(TaskbarWidget& widget);
    void update_system_tray_widget(TaskbarWidget& widget);
    void update_volume_widget(TaskbarWidget& widget);
    void update_network_widget(TaskbarWidget& widget);
    void update_battery_widget(TaskbarWidget& widget);
    void update_search_widget(TaskbarWidget& widget);
    void update_widget_state(TaskbarWidget& widget);
    
    void update_layout();
    void calculate_item_layout();
    f32 calculate_item_width(const TaskbarItem& item);
    f32 estimate_text_width(const String& text);
    void calculate_widget_layout();
    void update_taskbar_bounds();
    
    void update_visual_effects();
    void update_background_effects();
    void render_background_blur();
    void render_background_glow();
    void render_taskbar_shadow();
    
    void update_item_effects();
    void update_item_glow(TaskbarItem& item);
    void render_item_glow(const TaskbarItem& item);
    void update_item_indicator(TaskbarItem& item);
    void render_activity_indicator(const TaskbarItem& item);
    void update_item_badge(TaskbarItem& item);
    void render_item_badge(const TaskbarItem& item);
    
    void update_widget_effects();
    void update_widget_glow(TaskbarWidget& widget);
    void render_widget_glow(const TaskbarWidget& widget);
    void update_widget_badge(TaskbarWidget& widget);
    void render_widget_badge(const TaskbarWidget& widget);
    
    void update_item_animations();
    void update_animation_progress(TaskbarAnimation& animation);
    void apply_animation_to_item(const TaskbarAnimation& animation);
    void cleanup_finished_animations();
    void update_hover_effects();
    void update_transition_effects();
    
    void update_notifications();
    void update_notification_animation(TaskbarNotification& notification);
    void update_system_tray();
    void update_status_indicators();
    
    void update_theme_colors();
    
    TaskbarItemId generate_item_id();
    f64 calculate_memory_usage() const;
    
    f32 get_system_volume();
    bool is_system_muted();
    NetworkStatus get_network_status();
    f32 get_signal_strength();
    f32 get_battery_level();
    bool is_battery_charging();
    void perform_instant_search(const String& query);
    
    void cleanup_vulkan_resources();
};

} // namespace SU1

