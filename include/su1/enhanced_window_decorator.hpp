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

namespace SU1 {

using WindowDecorationId = u64;

struct Vec2 {
    f32 x, y;
};

struct Vec4 {
    f32 x, y, z, w;
};

struct Rect {
    f32 x, y, width, height;
};

enum class WindowButtonType {
    Close,
    Maximize,
    Minimize,
    Menu,
    Settings,
    Help,
    Custom
};

enum class MaterialType {
    Solid,
    Glass,
    Metal,
    Plastic,
    Paper,
    Carbon,
    Ceramic,
    Hologram,
    Liquid,
    Crystal,
    Fabric,
    Stone
};

enum class BlurType {
    Gaussian,
    Bokeh,
    Motion,
    Radial,
    Zoom,
    Surface
};

struct WindowButton {
    WindowButtonType type;
    Vec2 position;
    Vec2 size;
    Rect bounds;
    String icon_path;
    String tooltip_text;
    
    Vec4 normal_color;
    Vec4 hover_color;
    Vec4 pressed_color;
    Vec4 disabled_color;
    Vec4 current_color;
    
    bool is_enabled;
    bool is_visible;
    bool is_hovered;
    bool is_pressed;
    bool was_hovered;
    bool was_pressed;
    
    f32 animation_duration;
    f32 hover_scale;
    f32 pressed_scale;
    f32 current_scale;
    f32 current_opacity;
    
    f32 glow_intensity;
    f32 press_effect_intensity;
    
    u64 hover_start_time;
    u64 press_start_time;
    
    std::function<void()> click_callback;
};

struct WindowFrame {
    String name;
    f32 border_width;
    f32 corner_radius;
    f32 shadow_blur;
    Vec2 shadow_offset;
    Vec4 shadow_color;
    Vec4 border_color;
    Vec4 background_color;
    f32 resize_handle_size;
    
    bool enable_resize;
    bool enable_shadow;
    bool enable_border;
    bool enable_corner_rounding;
};

struct MenuItem {
    String text;
    bool is_enabled;
    bool is_separator;
    std::function<void()> callback;
};

struct WindowMenu {
    String name;
    Vec2 position;
    Vec2 size;
    Vec4 background_color;
    Vec4 border_color;
    Vec4 text_color;
    f32 corner_radius;
    f32 border_width;
    f32 shadow_blur;
    Vec4 shadow_color;
    bool is_visible;
    f32 animation_duration;
    Vector<MenuItem> items;
};

struct WindowIcon {
    String name;
    Vec2 size;
    Vec2 position;
    String texture_path;
    bool is_visible;
    f32 animation_duration;
    f32 hover_scale;
    f32 pressed_scale;
};

struct ShadowEffect {
    Rect source_rect;
    Vec2 offset;
    f32 blur_radius;
    f32 base_blur_radius;
    Vec4 color;
    f32 opacity;
    f32 base_opacity;
    bool is_enabled;
};

struct BlurEffect {
    Rect source_rect;
    f32 blur_radius;
    f32 base_blur_radius;
    f32 opacity;
    bool is_enabled;
    BlurType blur_type;
    u32 sample_count;
};

struct GlowEffect {
    Rect source_rect;
    f32 glow_radius;
    Vec4 color;
    f32 intensity;
    f32 base_intensity;
    bool is_enabled;
};

struct WindowAnimation {
    f32 duration;
    f32 progress;
    f32 start_value;
    f32 end_value;
    bool is_active;
    std::function<f32(f32)> easing_function;
};

struct WindowTheme {
    String name;
    String display_name;
    String description;
    
    f32 titlebar_height;
    f32 border_width;
    f32 corner_radius;
    f32 shadow_blur_radius;
    Vec2 shadow_offset;
    f32 shadow_opacity;
    
    Vec4 background_color;
    Vec4 titlebar_color;
    Vec4 border_color;
    Vec4 text_color;
    Vec4 accent_color;
    Vec4 shadow_color;
    
    Vec4 inactive_background_color;
    Vec4 inactive_titlebar_color;
    Vec4 inactive_border_color;
    Vec4 inactive_text_color;
    
    f32 hover_scale;
    f32 pressed_scale;
    f32 animation_duration;
    f32 blur_radius;
    f32 glow_intensity;
    
    bool enable_blur;
    bool enable_shadows;
    bool enable_glow;
    bool enable_animations;
    bool enable_transparency;
    bool enable_glass_effect;
    bool enable_depth_effects;
    bool enable_rgb_effects;
    bool enable_pulsing_glow;
    bool enable_focus_indicators;
    bool enable_warm_glow;
    bool enable_soft_shadows;
    bool enable_high_contrast_mode;
    bool enable_holographic_effects;
    bool enable_color_shifting;
    bool enable_particle_effects;
    bool enable_liquid_effects;
    bool enable_morphing;
    bool enable_fluid_dynamics;
    
    MaterialType material_type;
    f32 surface_roughness;
    f32 metallic_factor;
    f32 reflectance;
    f32 subsurface_scattering;
    f32 iridescence;
    f32 viscosity;
    f32 surface_tension;
};

struct WindowDecoration {
    void* window_handle;
    String theme_name;
    
    Rect window_rect;
    Rect titlebar_rect;
    Rect content_rect;
    Rect resize_handles[8];
    
    Vec4 current_background_color;
    Vec4 current_titlebar_color;
    Vec4 current_border_color;
    Vec4 current_text_color;
    
    bool is_visible;
    bool is_focused;
    f32 opacity;
    f32 scale;
    f32 hover_progress;
    f32 focus_progress;
    
    ShadowEffect shadow_effect;
    BlurEffect blur_effect;
    GlowEffect glow_effect;
    
    Vector<WindowAnimation> active_animations;
};

struct MaterialProperty {
    String name;
    Vec4 albedo;
    f32 roughness;
    f32 metallic;
    f32 reflectance;
    f32 transparency;
    f32 refraction_index;
    f32 subsurface_scattering;
    Vec4 emission;
};

struct LightingModel {
    String name;
    String description;
    bool use_metallic_workflow;
    bool use_specular_workflow;
    bool enable_ibl;
    bool enable_shadows;
    bool enable_reflections;
    bool enable_subsurface_scattering;
    bool enable_volumetric_lighting;
    bool enable_rim_lighting;
    bool enable_cel_shading;
};

struct CustomShader {
    String name;
    String vertex_shader_path;
    String fragment_shader_path;
    String compute_shader_path;
    bool is_enabled;
    std::unordered_map<String, f32> parameters;
};

struct WindowControl {
    String name;
    Vec2 position;
    Vec2 size;
    bool is_enabled;
    bool is_visible;
    std::function<void()> callback;
};

struct WindowDecoratorConfig {
    bool enable_hardware_acceleration;
    bool enable_multi_threading;
    bool enable_gpu_effects;
    bool enable_real_time_updates;
    f32 target_framerate;
    u32 max_decorations;
    u32 max_effects;
    String default_theme;
};

struct WindowDecoratorStats {
    std::atomic<u64> decorations_rendered;
    std::atomic<u64> shadows_rendered;
    std::atomic<u64> blur_operations;
    std::atomic<f64> decoration_time_ms;
    std::atomic<f64> shadow_time_ms;
    std::atomic<f64> blur_time_ms;
    size_t active_decorations;
    size_t active_themes;
    f64 memory_usage_mb;
};

class EnhancedWindowDecorator {
private:
    class Impl;
    std::unique_ptr<Impl> impl_;

public:
    EnhancedWindowDecorator();
    ~EnhancedWindowDecorator();

    bool initialize(VkDevice device, VkPhysicalDevice physical_device, const WindowDecoratorConfig& config);
    void shutdown();

    WindowDecorationId create_window_decoration(void* window_handle, const String& theme_name = "");
    void destroy_window_decoration(WindowDecorationId decoration_id);

    void set_window_theme(WindowDecorationId decoration_id, const String& theme_name);
    void set_window_focused(WindowDecorationId decoration_id, bool focused);
    void set_window_bounds(WindowDecorationId decoration_id, const Rect& bounds);
    void set_window_opacity(WindowDecorationId decoration_id, f32 opacity);

    Vector<String> get_available_themes() const;

    void set_global_opacity(f32 opacity);
    void set_global_blur_radius(f32 radius);
    void set_global_shadow_intensity(f32 intensity);
    void set_accent_color(const Vec4& color);

    void enable_animations(bool enabled);
    void enable_glass_effects(bool enabled);
    void enable_shadows(bool enabled);

    WindowDecoratorStats get_performance_stats() const;

private:
    bool create_vulkan_resources();
    bool create_decoration_buffers();
    bool create_buffer(size_t size, VkBufferUsageFlags usage, VkBuffer* buffer, VkDeviceMemory* memory);
    
    bool initialize_default_themes();
    void create_modern_glass_theme();
    void create_classic_theme();
    void create_minimal_theme();
    void create_gaming_theme();
    void create_productivity_theme();
    void create_dark_mode_theme();
    void create_light_mode_theme();
    void create_high_contrast_theme();
    void create_holographic_theme();
    void create_liquid_theme();
    
    bool create_window_controls();
    void create_window_buttons();
    void create_window_frames();
    void create_window_menus();
    void create_window_icons();
    
    bool setup_material_system();
    void create_material_properties();
    void create_lighting_models();
    void create_custom_shaders();
    
    void start_decoration_threads();
    void stop_decoration_threads();
    
    void decoration_update_loop();
    void animation_update_loop();
    void effects_update_loop();
    
    void update_window_decorations();
    void update_decoration_geometry(WindowDecoration& decoration);
    void update_button_positions(WindowDecoration& decoration, const WindowTheme& theme);
    void update_resize_handles(WindowDecoration& decoration, const WindowTheme& theme);
    void update_decoration_colors(WindowDecoration& decoration);
    void apply_color_animations(WindowDecoration& decoration, const WindowTheme& theme);
    void update_decoration_effects(WindowDecoration& decoration);
    void update_shadow_for_decoration(WindowDecoration& decoration, const WindowTheme& theme);
    void update_blur_for_decoration(WindowDecoration& decoration, const WindowTheme& theme);
    void update_glow_for_decoration(WindowDecoration& decoration, const WindowTheme& theme);
    
    void update_window_buttons();
    void update_button_state(WindowButton& button);
    void update_button_animation(WindowButton& button);
    void update_button_effects(WindowButton& button);
    
    void update_window_frames();
    void update_frame_geometry(WindowFrame& frame);
    void update_frame_effects(WindowFrame& frame);
    
    void update_button_animations();
    void update_hover_effects();
    void update_focus_effects();
    void update_resize_animations();
    
    void update_shadow_effects();
    void update_shadow_parameters(ShadowEffect& shadow);
    void render_shadow_effect(const ShadowEffect& shadow);
    
    void update_blur_effects();
    void update_blur_parameters(BlurEffect& blur);
    void render_blur_effect(const BlurEffect& blur);
    
    void update_glow_effects();
    void update_glow_parameters(GlowEffect& glow);
    void render_glow_effect(const GlowEffect& glow);
    
    Vec4 lerp_color(const Vec4& a, const Vec4& b, f32 t);
    Vec4 multiply_color(const Vec4& a, const Vec4& b);
    
    const WindowTheme* get_theme(const String& theme_name) const;
    
    f64 calculate_memory_usage() const;
    void cleanup_vulkan_resources();
};

} // namespace SU1

