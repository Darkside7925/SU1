//
// SU1 GPU Accelerated UI System Header
// Comprehensive GPU acceleration for all UI components
//

#pragma once

#include "su1/core.hpp"
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>

namespace SU1 {

// Forward declarations
class GPULayoutEngine;
class GPUTextRenderer;
class GPUEffectProcessor;

struct UIAccelerationConfig {
    bool extreme_acceleration = true;
    bool gpu_culling = true;
    bool gpu_layout = true;
    bool gpu_text_rendering = true;
    bool gpu_effects = true;
    bool batch_rendering = true;
    bool instanced_rendering = true;
    bool async_compute = true;
    bool multi_threaded_updates = true;
    
    // Render targets
    u32 render_width = 3840;
    u32 render_height = 2160;
    bool hdr_rendering = true;
    u32 msaa_samples = 4;
    
    // Performance limits
    u32 max_ui_elements = 100000;
    u32 max_text_glyphs = 1000000;
    u32 max_vertices = 10000000;
    u32 max_indices = 30000000;
    
    // Effects
    bool enable_blur = true;
    bool enable_glow = true;
    bool enable_shadows = true;
    bool enable_reflections = true;
    bool enable_particle_effects = true;
    bool enable_physics_simulation = true;
    
    // Text rendering
    u32 font_atlas_size = 4096;
    u32 max_fonts = 100;
    bool subpixel_rendering = true;
    bool font_hinting = true;
    
    // Animation
    u32 max_animations = 10000;
    f32 animation_framerate = 240.0f;
    bool smooth_animations = true;
    bool physics_based_animations = true;
    
    // Memory management
    size_t gpu_memory_pool_mb = 512;
    bool dynamic_memory_allocation = true;
    bool memory_compression = false;
};

struct UIVertex {
    Vec3 position;
    Vec2 tex_coord;
    Vec4 color;
    Vec3 normal;
    Vec2 uv2; // Secondary UV for effects
};

struct UIElementData {
    Mat4 transform;
    Vec4 color;
    Vec4 bounds; // x, y, width, height
    u32 type;
    u32 flags;
    f32 depth;
    f32 opacity;
    Vec2 texture_offset;
    Vec2 texture_scale;
    f32 corner_radius;
    f32 border_width;
    Vec4 border_color;
    f32 glow_intensity;
    f32 blur_radius;
};

struct GlyphData {
    Vec2 position;
    Vec2 size;
    Vec2 atlas_uv;
    Vec2 atlas_size;
    Vec4 color;
    f32 scale;
    f32 rotation;
    u32 font_id;
    u32 character;
};

struct AnimationData {
    Vec3 start_position;
    Vec3 end_position;
    Vec4 start_color;
    Vec4 end_color;
    f32 start_scale;
    f32 end_scale;
    f32 duration;
    f32 elapsed_time;
    u32 easing_type;
    u32 flags;
};

enum class UIElementType : u32 {
    Panel = 0,
    Button = 1,
    Text = 2,
    Image = 3,
    Window = 4,
    Icon = 5,
    Slider = 6,
    Checkbox = 7,
    ScrollBar = 8,
    Menu = 9,
    Tooltip = 10,
    Progress = 11,
    Custom = 12
};

enum class AnimationType : u32 {
    Linear = 0,
    EaseIn = 1,
    EaseOut = 2,
    EaseInOut = 3,
    Bounce = 4,
    Elastic = 5,
    Spring = 6,
    Physics = 7
};

struct UIElement {
    UIElementType type = UIElementType::Panel;
    Vec2 position = {0.0f, 0.0f};
    Vec2 size = {100.0f, 100.0f};
    Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    f32 rotation = 0.0f;
    f32 scale = 1.0f;
    f32 opacity = 1.0f;
    f32 depth = 0.0f;
    bool visible = true;
    bool interactive = true;
    u32 id = 0;
    u32 parent_id = 0;
    String text;
    String texture_path;
    u32 vertex_offset = 0;
    u32 vertex_count = 0;
    u32 index_offset = 0;
    u32 index_count = 0;
};

struct TextElement {
    String text;
    String font_name;
    f32 font_size = 16.0f;
    Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    Vec2 position = {0.0f, 0.0f};
    f32 line_height = 1.2f;
    u32 alignment = 0; // 0=left, 1=center, 2=right
    u32 glyph_count = 0;
    u32 glyph_offset = 0;
    u32 index_offset = 0;
    bool word_wrap = false;
    f32 max_width = 0.0f;
};

struct ButtonElement {
    UIElement base;
    String label;
    bool pressed = false;
    bool hovered = false;
    bool disabled = false;
    Vec4 normal_color = {0.3f, 0.3f, 0.3f, 1.0f};
    Vec4 hover_color = {0.4f, 0.4f, 0.4f, 1.0f};
    Vec4 pressed_color = {0.2f, 0.2f, 0.2f, 1.0f};
    f32 corner_radius = 4.0f;
    f32 border_width = 1.0f;
    Vec4 border_color = {0.5f, 0.5f, 0.5f, 1.0f};
};

struct PanelElement {
    UIElement base;
    bool has_background = true;
    bool has_border = true;
    f32 corner_radius = 0.0f;
    f32 border_width = 1.0f;
    Vec4 border_color = {0.5f, 0.5f, 0.5f, 1.0f};
    f32 blur_radius = 0.0f;
    f32 glow_intensity = 0.0f;
    Vec4 glow_color = {1.0f, 1.0f, 1.0f, 1.0f};
};

struct WindowElement {
    UIElement base;
    String title;
    bool has_title_bar = true;
    bool resizable = true;
    bool closable = true;
    bool minimizable = true;
    bool maximizable = true;
    Vec2 min_size = {200.0f, 100.0f};
    Vec2 max_size = {4000.0f, 4000.0f};
    f32 title_bar_height = 32.0f;
    Vec4 title_bar_color = {0.2f, 0.2f, 0.2f, 1.0f};
    f32 shadow_radius = 8.0f;
    Vec4 shadow_color = {0.0f, 0.0f, 0.0f, 0.5f};
};

struct IconElement {
    UIElement base;
    String icon_name;
    u32 icon_atlas_index = 0;
    Vec2 atlas_uv = {0.0f, 0.0f};
    Vec2 atlas_size = {1.0f, 1.0f};
    f32 icon_scale = 1.0f;
    Vec4 tint_color = {1.0f, 1.0f, 1.0f, 1.0f};
};

struct UIAnimation {
    u32 target_element_id = 0;
    AnimationType type = AnimationType::Linear;
    f32 duration = 1.0f;
    f32 elapsed_time = 0.0f;
    bool loop = false;
    bool reverse = false;
    bool active = true;
    
    // Animation targets
    Vec2 target_position;
    Vec2 target_size;
    Vec4 target_color;
    f32 target_rotation;
    f32 target_scale;
    f32 target_opacity;
    
    // Start values (set when animation begins)
    Vec2 start_position;
    Vec2 start_size;
    Vec4 start_color;
    f32 start_rotation;
    f32 start_scale;
    f32 start_opacity;
    
    bool is_complete() const { return elapsed_time >= duration && !loop; }
    void update(f64 delta_time) { elapsed_time += static_cast<f32>(delta_time); }
};

struct UIRenderParams {
    VkCommandBuffer command_buffer;
    Mat4 view_matrix;
    Mat4 projection_matrix;
    f64 delta_time;
    Vec2 screen_size;
    Vec2 mouse_position;
    bool mouse_pressed = false;
    f32 animation_time = 0.0f;
    u32 frame_index = 0;
};

struct UIAccelerationStats {
    std::atomic<u64> ui_elements_rendered{0};
    std::atomic<u64> text_glyphs_rendered{0};
    std::atomic<u64> draw_calls_issued{0};
    std::atomic<f64> gpu_ui_time_ms{0.0};
    std::atomic<f64> layout_time_ms{0.0};
    std::atomic<f64> text_rendering_time_ms{0.0};
    std::atomic<f64> effect_processing_time_ms{0.0};
    std::atomic<f64> animation_time_ms{0.0};
    std::atomic<u64> vertices_processed{0};
    std::atomic<u64> triangles_rendered{0};
    std::atomic<f64> gpu_memory_used_mb{0.0};
    std::atomic<f64> cpu_time_ms{0.0};
    
    // Feature usage
    bool gpu_culling_enabled = false;
    bool gpu_layout_enabled = false;
    bool gpu_text_rendering = false;
    bool gpu_effects_enabled = false;
    bool batch_rendering = false;
    bool instanced_rendering = false;
    bool async_compute = false;
    bool multi_threaded_updates = false;
    
    // Performance metrics
    f64 average_fps = 0.0;
    f64 frame_time_ms = 0.0;
    f64 ui_cpu_usage_percent = 0.0;
    f64 ui_gpu_usage_percent = 0.0;
};

class GPUAcceleratedUI {
public:
    GPUAcceleratedUI();
    ~GPUAcceleratedUI();
    
    // Core lifecycle
    bool initialize(VkDevice device, VkPhysicalDevice physical_device, const UIAccelerationConfig& config);
    void shutdown();
    
    // Main rendering
    void render_ui(const UIRenderParams& params);
    void present_ui(VkImage target_image);
    
    // UI element management
    u32 add_ui_element(const UIElement& element);
    void remove_ui_element(u32 element_id);
    void update_ui_element(u32 element_id, const UIElement& element);
    UIElement* get_ui_element(u32 element_id);
    
    // Specialized element creation
    u32 create_button(const String& text, Vec2 position, Vec2 size);
    u32 create_panel(Vec2 position, Vec2 size, Vec4 color);
    u32 create_window(const String& title, Vec2 position, Vec2 size);
    u32 create_text(const String& text, Vec2 position, f32 font_size);
    u32 create_icon(const String& icon_name, Vec2 position, f32 size);
    
    // Text rendering
    void add_text_element(const TextElement& text);
    void update_text(u32 element_id, const String& new_text);
    void set_font(const String& font_name, f32 size);
    Vec2 measure_text(const String& text, const String& font_name, f32 font_size);
    
    // Animation system
    u32 start_animation(const UIAnimation& animation);
    void stop_animation(u32 animation_id);
    void pause_animation(u32 animation_id);
    void resume_animation(u32 animation_id);
    void set_animation_speed(f32 speed_multiplier);
    
    // Layout system
    void set_layout_mode(u32 element_id, const String& layout_type);
    void update_layout();
    void invalidate_layout(u32 element_id);
    void auto_layout_children(u32 parent_id);
    
    // Input handling
    void handle_mouse_move(Vec2 position);
    void handle_mouse_click(Vec2 position, bool pressed);
    void handle_keyboard_input(u32 key_code, bool pressed);
    void handle_text_input(const String& text);
    
    // GPU acceleration controls
    void enable_gpu_culling(bool enabled);
    void enable_gpu_layout(bool enabled);
    void enable_gpu_text_rendering(bool enabled);
    void enable_gpu_effects(bool enabled);
    void enable_batch_rendering(bool enabled);
    void enable_instanced_rendering(bool enabled);
    void enable_async_compute(bool enabled);
    
    // Effects and styling
    void apply_blur_effect(u32 element_id, f32 radius);
    void apply_glow_effect(u32 element_id, f32 intensity, Vec4 color);
    void apply_shadow_effect(u32 element_id, Vec2 offset, f32 radius, Vec4 color);
    void set_corner_radius(u32 element_id, f32 radius);
    void set_border(u32 element_id, f32 width, Vec4 color);
    
    // Theme system
    void load_theme(const String& theme_name);
    void set_theme_color(const String& color_name, Vec4 color);
    void apply_theme_to_element(u32 element_id, const String& theme_name);
    
    // Performance monitoring
    UIAccelerationStats get_performance_stats() const;
    void reset_performance_counters();
    void enable_performance_monitoring(bool enabled);
    void set_target_framerate(f32 fps);
    
    // Resource management
    void preload_font(const String& font_path);
    void preload_texture(const String& texture_path);
    void preload_icon_atlas(const String& atlas_path);
    void garbage_collect_resources();
    size_t get_gpu_memory_usage() const;
    
    // Debugging and profiling
    void enable_debug_rendering(bool enabled);
    void enable_wireframe_mode(bool enabled);
    void capture_frame_data(const String& filename);
    void dump_ui_hierarchy(const String& filename);
    
    // Advanced features
    void enable_physics_simulation(bool enabled);
    void enable_particle_effects(bool enabled);
    void enable_volumetric_effects(bool enabled);
    void set_physics_gravity(Vec2 gravity);
    void add_physics_constraint(u32 element1_id, u32 element2_id);
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    
    // Initialization helpers
    bool create_command_pool();
    bool create_descriptor_pool();
    bool create_render_pass();
    bool create_gpu_buffers();
    bool create_render_targets();
    bool create_rendering_pipelines();
    bool initialize_font_system();
    bool initialize_subsystems();
    
    // Buffer management
    bool allocate_buffer_memory();
    bool allocate_render_target_memory();
    bool create_render_target_views();
    
    // Shader and pipeline creation
    bool load_ui_shaders();
    bool create_text_rendering_pipeline();
    bool create_button_rendering_pipeline();
    bool create_panel_rendering_pipeline();
    bool create_compute_pipelines();
    
    // Font system
    bool create_font_atlas();
    bool load_default_fonts();
    bool setup_glyph_cache();
    
    // Rendering methods
    void update_ui_elements_gpu(VkCommandBuffer cmd, const UIRenderParams& params);
    void perform_gpu_culling(VkCommandBuffer cmd, const UIRenderParams& params);
    void render_text(VkCommandBuffer cmd, const UIRenderParams& params);
    void render_buttons(VkCommandBuffer cmd, const UIRenderParams& params);
    void render_panels(VkCommandBuffer cmd, const UIRenderParams& params);
    void render_windows(VkCommandBuffer cmd, const UIRenderParams& params);
    void render_icons(VkCommandBuffer cmd, const UIRenderParams& params);
    void apply_gpu_effects(VkCommandBuffer cmd, const UIRenderParams& params);
    
    // Background processing
    void start_background_threads();
    void stop_background_threads();
    void ui_update_loop();
    void layout_update_loop();
    void animation_update_loop();
    
    // Update functions
    void update_ui_element_states();
    void update_animations();
    void update_physics();
    void update_particles();
    
    // Cleanup
    void cleanup_vulkan_objects();
    void cleanup_subsystems();
    void cleanup_pipelines();
    void cleanup_buffers();
    void cleanup_render_targets();
};

// Utility functions
Vec2 screen_to_ui_coordinates(Vec2 screen_pos, Vec2 screen_size);
Vec2 ui_to_screen_coordinates(Vec2 ui_pos, Vec2 screen_size);
f32 calculate_ui_scale_factor(Vec2 screen_size);
bool point_in_rect(Vec2 point, Vec2 rect_pos, Vec2 rect_size);
f32 ease_in_out(f32 t);
f32 ease_bounce(f32 t);
f32 ease_elastic(f32 t);
Vec4 interpolate_color(Vec4 start, Vec4 end, f32 t);
Mat4 create_ui_transform(Vec2 position, Vec2 size, f32 rotation, f32 scale);

// Theme utilities
Vector<String> get_available_themes();
Vector<String> get_theme_color_names(const String& theme_name);
Vec4 get_theme_color(const String& theme_name, const String& color_name);

// Font utilities
Vector<String> get_available_fonts();
bool is_font_loaded(const String& font_name);
f32 get_font_line_height(const String& font_name, f32 font_size);
f32 get_text_width(const String& text, const String& font_name, f32 font_size);

// Performance utilities
String format_performance_stats(const UIAccelerationStats& stats);
f64 calculate_ui_performance_score(const UIAccelerationStats& stats);
Vector<String> get_performance_recommendations(const UIAccelerationStats& stats);

} // namespace SU1
