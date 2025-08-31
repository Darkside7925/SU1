#pragma once

#include "su1/core.hpp"
#include <vulkan/vulkan.h>
#include <memory>
#include <functional>
#include <atomic>
#include <thread>
#include <map>

namespace SU1 {

using AnimationId = u64;
const AnimationId INVALID_ANIMATION_ID = 0;

struct AnimationConfig {
    bool enable_fluid_dynamics = true;
    bool enable_particle_systems = true;
    bool enable_elastic_deformation = true;
    bool enable_morphing = true;
    bool enable_blur_effects = true;
    bool enable_distortion_effects = true;
    bool enable_parallax_scrolling = true;
    bool enable_depth_of_field = true;
    bool enable_gpu_acceleration = true;
    bool enable_temporal_reprojection = true;
    bool enable_motion_blur = true;
    bool enable_adaptive_quality = true;
    
    f32 global_animation_speed = 1.0f;
    f32 fluid_viscosity = 0.02f;
    f32 surface_tension = 0.8f;
    f32 gravity_strength = 9.81f;
    f32 elasticity = 0.7f;
    f32 damping_factor = 0.98f;
    
    u32 fluid_grid_resolution = 256;
    u32 max_particles = 100000;
    u32 physics_substeps = 4;
    f64 target_framerate = 120.0;
    f64 max_frame_time_ms = 16.67;
};

enum class InterpolationType : u32 {
    Linear = 0,
    Cubic = 1,
    Constant = 2,
    Bezier = 3,
    Hermite = 4,
    Catmull = 5
};

enum class WindowAnimationType : u32 {
    Open = 0,
    Close = 1,
    Minimize = 2,
    Maximize = 3,
    Restore = 4,
    Move = 5,
    Resize = 6,
    Focus = 7,
    Unfocus = 8,
    Hover = 9,
    Leave = 10,
    TaskbarHover = 11,
    TaskbarClick = 12,
    WorkspaceSwitch = 13
};

enum class TransitionType : u32 {
    Fade = 0,
    Slide = 1,
    Scale = 2,
    Rotate = 3,
    Morph = 4,
    FluidCube = 5,
    RadialExpansion = 6,
    LiquidWave = 7,
    ElasticBounce = 8,
    Ripple = 9,
    Shatter = 10,
    Vortex = 11
};

enum class ParticleType : u32 {
    Fluid = 0,
    Smoke = 1,
    Fire = 2,
    Spark = 3,
    Magic = 4,
    Effect = 5,
    System = 6,
    Trail = 7
};

enum class EmissionShape : u32 {
    Point = 0,
    Sphere = 1,
    Box = 2,
    Circle = 3,
    Cone = 4,
    Mesh = 5
};

enum class SimulationSpace : u32 {
    Local = 0,
    World = 1,
    Custom = 2
};

enum class CollisionShape : u32 {
    Sphere = 0,
    Box = 1,
    Capsule = 2,
    Mesh = 3,
    Plane = 4
};

struct AnimationKeyframe {
    f32 time = 0.0f;
    f32 value = 0.0f;
    f32 in_tangent = 0.0f;
    f32 out_tangent = 0.0f;
    InterpolationType interpolation_type = InterpolationType::Linear;
};

struct Vec3Keyframe {
    f32 time = 0.0f;
    Vec3 value = {0.0f, 0.0f, 0.0f};
    Vec3 in_tangent = {0.0f, 0.0f, 0.0f};
    Vec3 out_tangent = {0.0f, 0.0f, 0.0f};
    InterpolationType interpolation_type = InterpolationType::Linear;
};

struct FloatKeyframe {
    f32 time = 0.0f;
    f32 value = 0.0f;
    f32 in_tangent = 0.0f;
    f32 out_tangent = 0.0f;
    InterpolationType interpolation_type = InterpolationType::Linear;
};

struct AnimationTrack {
    String property_name;
    Vector<AnimationKeyframe> keyframes;
    void* target_object = nullptr;
    std::function<void(void*, f32)> property_setter;
    f32 weight = 1.0f;
    bool is_additive = false;
    bool is_enabled = true;
};

struct AnimationClip {
    String name;
    f32 duration = 1.0f;
    f32 current_time = 0.0f;
    f32 speed = 1.0f;
    String easing_function = "linear";
    Vector<AnimationTrack> tracks;
    bool is_playing = false;
    bool is_looping = false;
    bool is_reversible = false;
    u32 loop_count = 0;
    f32 blend_weight = 1.0f;
};

struct FluidParticle {
    Vec3 position = {0.0f, 0.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};
    Vec3 acceleration = {0.0f, 0.0f, 0.0f};
    Vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
    f32 mass = 1.0f;
    f32 radius = 1.0f;
    f32 density = 1.0f;
    f32 pressure = 0.0f;
    f32 temperature = 20.0f;
    f32 life_time = 1.0f;
    f32 age = 0.0f;
    bool is_active = false;
    ParticleType particle_type = ParticleType::Fluid;
    u32 system_id = 0;
    u32 collision_layers = 1;
};

struct SpringConstraint {
    u32 particle_a = 0;
    u32 particle_b = 0;
    f32 rest_length = 10.0f;
    f32 stiffness = 0.5f;
    f32 damping = 0.1f;
    f32 break_threshold = 1000.0f;
    bool is_active = true;
    bool can_break = false;
};

struct PhysicsBody {
    Vec3 position = {0.0f, 0.0f, 0.0f};
    Vec3 velocity = {0.0f, 0.0f, 0.0f};
    Vec3 acceleration = {0.0f, 0.0f, 0.0f};
    Vec3 angular_velocity = {0.0f, 0.0f, 0.0f};
    Vec3 size = {1.0f, 1.0f, 1.0f};
    f32 mass = 1.0f;
    f32 drag = 0.1f;
    f32 angular_drag = 0.1f;
    f32 restitution = 0.8f;
    f32 friction = 0.3f;
    bool is_kinematic = false;
    bool is_static = false;
    bool use_gravity = true;
    CollisionShape collision_shape = CollisionShape::Box;
    u32 collision_layers = 1;
    u32 collision_mask = 0xFFFFFFFF;
};

struct BezierCurve {
    Vector<Vec3> control_points;
    f32 duration = 1.0f;
    f32 current_time = 0.0f;
    Vec3 current_position = {0.0f, 0.0f, 0.0f};
    bool is_active = false;
    bool is_closed = false;
    f32 tension = 0.5f;
    f32 bias = 0.0f;
    f32 continuity = 0.0f;
};

struct ParticleSystem {
    String name;
    u32 max_particles = 1000;
    f32 spawn_rate = 100.0f;
    f32 emission_rate = 50.0f;
    f32 emission_timer = 0.0f;
    f32 particle_lifetime = 2.0f;
    Vec4 start_color = {1.0f, 1.0f, 1.0f, 1.0f};
    Vec4 end_color = {0.0f, 0.0f, 1.0f, 0.0f};
    f32 start_size = 1.0f;
    f32 end_size = 5.0f;
    f32 gravity_modifier = 1.0f;
    f32 air_resistance = 0.1f;
    SimulationSpace simulation_space = SimulationSpace::World;
    EmissionShape shape = EmissionShape::Sphere;
    Vec3 spawn_position = {0.0f, 0.0f, 0.0f};
    Vec3 spawn_direction = {0.0f, 1.0f, 0.0f};
    Vec3 spawn_size = {10.0f, 10.0f, 10.0f};
    f32 spawn_radius = 5.0f;
    f32 spawn_speed = 10.0f;
    f32 speed_variation = 2.0f;
    f32 velocity_randomness = 0.1f;
    u32 burst_count = 0;
    f32 burst_interval = 1.0f;
    bool is_looping = true;
    bool is_playing = false;
    bool inherit_velocity = false;
};

struct ElasticDeformation {
    Vec3 center = {0.0f, 0.0f, 0.0f};
    f32 radius = 100.0f;
    f32 strength = 0.3f;
    f32 frequency = 5.0f;
    f32 decay = 0.95f;
    f32 current_amplitude = 0.0f;
    bool is_active = false;
    Vector<Vec3> affected_vertices;
    Vector<f32> vertex_weights;
};

struct MorphTarget {
    u32 vertex_count = 0;
    Vector<Vec3> control_points;
    Vector<Vec3> target_positions;
    f32 deformation_strength = 0.5f;
    f32 smoothing_factor = 0.8f;
    bool is_active = false;
    f32 morph_weight = 0.0f;
    String morph_name;
};

struct Vec3Curve {
    Vector<Vec3Keyframe> keyframes;
    String curve_name;
    bool is_looping = false;
};

struct FloatCurve {
    Vector<FloatKeyframe> keyframes;
    String curve_name;
    bool is_looping = false;
};

struct WindowAnimation {
    String name;
    f32 duration = 0.5f;
    f32 current_time = 0.0f;
    String easing_function = "ease_out_quad";
    WindowAnimationType animation_type = WindowAnimationType::Open;
    void* target_window = nullptr;
    
    Vec3Curve scale_curve;
    Vec3Curve position_curve;
    Vec3Curve rotation_curve;
    FloatCurve opacity_curve;
    FloatCurve blur_curve;
    FloatCurve glow_curve;
    FloatCurve elevation_curve;
    FloatCurve border_glow_curve;
    FloatCurve border_thickness_curve;
    FloatCurve corner_radius_curve;
    FloatCurve bounce_curve;
    
    Vec3 current_scale = {1.0f, 1.0f, 1.0f};
    Vec3 current_position = {0.0f, 0.0f, 0.0f};
    Vec3 current_rotation = {0.0f, 0.0f, 0.0f};
    f32 current_opacity = 1.0f;
    f32 current_blur = 0.0f;
    f32 current_glow = 0.0f;
    f32 current_elevation = 0.0f;
    f32 current_border_glow = 0.0f;
    f32 current_border_thickness = 1.0f;
    f32 current_corner_radius = 8.0f;
    f32 current_bounce = 0.0f;
    
    Vec3 target_position = {0.0f, 0.0f, 0.0f};
    Vec3 target_scale = {1.0f, 1.0f, 1.0f};
    
    bool is_active = false;
    bool enable_particles = false;
    bool enable_fluid_distortion = false;
    bool enable_elastic_deformation = false;
    bool enable_depth_of_field = false;
    bool enable_glow_effects = false;
    bool enable_shadow_enhancement = false;
    bool enable_subtle_particles = false;
    bool enable_border_animation = false;
    bool enable_ripple_effect = false;
    bool enable_trajectory_particles = false;
    bool enable_morphing_transition = false;
    bool enable_edge_highlights = false;
    bool enable_expansion_waves = false;
    bool enable_icon_glow = false;
    bool enable_bounce_animation = false;
    bool enable_click_ripple = false;
    bool enable_press_feedback = false;
    
    Vector<ElasticDeformation> elastic_deformations;
};

struct TransitionEffect {
    String name;
    f32 duration = 1.0f;
    f32 current_time = 0.0f;
    String easing_function = "ease_in_out_quad";
    TransitionType transition_type = TransitionType::Fade;
    
    bool is_active = false;
    bool morphing_enabled = false;
    bool fluid_simulation_enabled = false;
    bool particle_effects_enabled = false;
    bool depth_peeling_enabled = false;
    bool motion_blur_enabled = false;
    bool cube_rotation_enabled = false;
    bool perspective_correction_enabled = false;
    bool ambient_occlusion_enabled = false;
    bool radial_expansion_enabled = false;
    bool icon_morphing_enabled = false;
    bool background_blur_enabled = false;
    
    f32 morph_intensity = 0.7f;
    f32 fluid_viscosity = 0.03f;
    u32 particle_count = 5000;
    u32 blur_samples = 24;
    u32 depth_layers = 8;
    f32 cube_size = 2000.0f;
    f32 rotation_speed = 90.0f;
    f32 fluid_density = 0.5f;
    f32 ambient_strength = 0.3f;
    f32 expansion_radius = 500.0f;
    f32 icon_scale_factor = 1.2f;
    f32 background_blur_radius = 15.0f;
    f32 current_radius = 0.0f;
    f32 icon_scale = 1.0f;
    f32 background_blur = 0.0f;
    Vec3 cube_rotation = {0.0f, 0.0f, 0.0f};
    f32 cube_scale = 1.0f;
    
    Vector<MorphTarget> morph_targets;
};

struct EasingLibrary {
    std::map<String, std::function<f32(f32)>> functions;
};

struct LightSource {
    Vec3 position = {0.0f, 0.0f, 0.0f};
    Vec3 direction = {0.0f, -1.0f, 0.0f};
    Vec3 color = {1.0f, 1.0f, 1.0f};
    f32 intensity = 1.0f;
    f32 range = 100.0f;
    f32 spot_angle = 45.0f;
    bool cast_shadows = true;
    bool is_enabled = true;
};

struct ShadowCaster {
    Vec3 position = {0.0f, 0.0f, 0.0f};
    Vec3 size = {1.0f, 1.0f, 1.0f};
    f32 shadow_intensity = 0.8f;
    f32 shadow_softness = 2.0f;
    bool is_enabled = true;
};

struct ReflectionProbe {
    Vec3 position = {0.0f, 0.0f, 0.0f};
    Vec3 size = {10.0f, 10.0f, 10.0f};
    f32 intensity = 1.0f;
    f32 blend_distance = 1.0f;
    bool box_projection = false;
    bool is_enabled = true;
};

struct PerformanceMetric {
    String metric_name;
    f64 current_value = 0.0;
    f64 average_value = 0.0;
    f64 min_value = 0.0;
    f64 max_value = 0.0;
    u64 sample_count = 0;
    Vector<f64> history;
};

struct FluidAnimationStats {
    std::atomic<u64> animations_played{0};
    std::atomic<u64> particles_simulated{0};
    std::atomic<u64> physics_steps{0};
    std::atomic<f64> animation_frame_time_ms{0.0};
    std::atomic<f64> physics_time_ms{0.0};
    std::atomic<f64> particle_time_ms{0.0};
    std::atomic<u32> active_animations{0};
    std::atomic<u32> active_particles{0};
    std::atomic<f64> fluid_simulation_fps{0.0};
    std::atomic<f64> animation_fps{0.0};
    std::atomic<f64> interpolation_time_ms{0.0};
    std::atomic<u64> bezier_evaluations{0};
    std::atomic<u64> spring_calculations{0};
    std::atomic<f64> gpu_memory_usage_mb{0.0};
    std::atomic<f64> cpu_usage_percent{0.0};
};

class FluidAnimationEngine {
public:
    FluidAnimationEngine();
    ~FluidAnimationEngine();
    
    bool initialize(VkDevice device, VkPhysicalDevice physical_device, const AnimationConfig& config);
    void shutdown();
    
    AnimationId play_window_animation(const String& animation_name, void* target_window);
    AnimationId play_transition_effect(const String& effect_name);
    AnimationId create_custom_animation(const AnimationClip& clip);
    AnimationId create_particle_effect(const ParticleSystem& system);
    
    void stop_animation(AnimationId animation_id);
    void pause_animation(AnimationId animation_id);
    void resume_animation(AnimationId animation_id);
    void set_animation_time(AnimationId animation_id, f32 time);
    void set_animation_speed(AnimationId animation_id, f32 speed);
    
    void pause_all_animations();
    void resume_all_animations();
    void stop_all_animations();
    void set_animation_speed(f32 speed);
    void set_time_scale(f64 scale);
    
    void enable_fluid_dynamics(bool enabled);
    void enable_particle_systems(bool enabled);
    void enable_elastic_deformation(bool enabled);
    void enable_morphing(bool enabled);
    void enable_blur_effects(bool enabled);
    void enable_distortion_effects(bool enabled);
    void enable_depth_of_field(bool enabled);
    void enable_motion_blur(bool enabled);
    
    void set_fluid_viscosity(f32 viscosity);
    void set_surface_tension(f32 tension);
    void set_gravity_strength(f32 gravity);
    void set_elasticity(f32 elasticity);
    void set_damping_factor(f32 damping);
    void set_wind_force(f32 force);
    
    void add_spring_constraint(u32 particle_a, u32 particle_b, f32 rest_length, f32 stiffness);
    void remove_spring_constraint(u32 particle_a, u32 particle_b);
    void add_physics_body(const PhysicsBody& body);
    void remove_physics_body(u32 body_id);
    
    void create_particle_system(const ParticleSystem& system);
    void destroy_particle_system(const String& system_name);
    void play_particle_system(const String& system_name);
    void stop_particle_system(const String& system_name);
    
    void add_elastic_deformation(const ElasticDeformation& deformation);
    void remove_elastic_deformation(u32 deformation_id);
    void add_morph_target(const MorphTarget& target);
    void set_morph_weight(const String& target_name, f32 weight);
    
    void register_easing_function(const String& name, std::function<f32(f32)> function);
    Vector<String> get_available_easing_functions() const;
    Vector<String> get_available_window_animations() const;
    Vector<String> get_available_transition_effects() const;
    
    void add_light_source(const LightSource& light);
    void remove_light_source(u32 light_id);
    void add_shadow_caster(const ShadowCaster& caster);
    void add_reflection_probe(const ReflectionProbe& probe);
    
    bool is_animation_playing(AnimationId animation_id) const;
    f32 get_animation_progress(AnimationId animation_id) const;
    f32 get_animation_time(AnimationId animation_id) const;
    f32 get_animation_duration(AnimationId animation_id) const;
    
    void set_quality_level(u32 level);
    void enable_adaptive_quality(bool enabled);
    void set_target_framerate(f64 fps);
    void set_max_particles(u32 max_particles);
    void set_physics_substeps(u32 substeps);
    
    void create_bezier_curve(const Vector<Vec3>& control_points, f32 duration);
    void create_animation_path(const Vector<Vec3>& waypoints, f32 duration);
    void create_keyframe_sequence(const Vector<AnimationKeyframe>& keyframes);
    
    void enable_debug_visualization(bool enabled);
    void enable_performance_profiling(bool enabled);
    void dump_animation_data(const String& filename) const;
    void save_animation_preset(const String& name, const AnimationClip& clip);
    void load_animation_preset(const String& name);
    
    FluidAnimationStats get_performance_stats() const;
    Vector<PerformanceMetric> get_detailed_metrics() const;
    void reset_performance_counters();
    f64 get_current_fps() const;
    f64 get_frame_time_ms() const;
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    
    bool create_vulkan_resources();
    bool create_compute_buffers();
    bool create_buffer(size_t size, VkBufferUsageFlags usage, VkBuffer* buffer, VkDeviceMemory* memory);
    
    bool initialize_fluid_dynamics();
    bool initialize_particle_systems();
    bool initialize_physics_engine();
    bool initialize_easing_library();
    bool setup_default_animations();
    
    void create_window_open_animation();
    void create_window_close_animation();
    void create_app_switch_animation();
    void create_hover_animation();
    void create_focus_animation();
    void create_minimization_animation();
    void create_maximization_animation();
    void create_taskbar_animations();
    void create_fluid_transition_animations();
    
    void start_animation_threads();
    void stop_animation_threads();
    void animation_update_loop();
    void physics_simulation_loop();
    void particle_simulation_loop();
    void interpolation_processing_loop();
    
    void update_active_animations();
    void update_transition_effects();
    void update_window_animations();
    
    f32 apply_easing_function(f32 t, const String& function_name);
    void evaluate_animation_clip(AnimationClip& clip, f32 time);
    f32 interpolate_keyframes(const Vector<AnimationKeyframe>& keyframes, f32 time);
    f32 lerp(f32 a, f32 b, f32 t);
    f32 cubic_interpolate(f32 a, f32 b, f32 tan_a, f32 tan_b, f32 t);
    f32 bezier_interpolate(f32 a, f32 b, f32 cp1, f32 cp2, f32 t);
    void apply_animation_value(AnimationClip& clip, AnimationTrack& track, f32 value);
    
    void process_transition_effect(TransitionEffect& effect, f32 progress);
    void process_morphing_transition(TransitionEffect& effect, f32 progress);
    void process_cube_transition(TransitionEffect& effect, f32 progress);
    void process_radial_expansion(TransitionEffect& effect, f32 progress);
    void process_liquid_wave_transition(TransitionEffect& effect, f32 progress);
    
    void process_window_animation(WindowAnimation& anim, f32 progress);
    Vec3 interpolate_vec3_curve(const Vec3Curve& curve, f32 time);
    f32 interpolate_float_curve(const FloatCurve& curve, f32 time);
    Vec3 lerp_vec3(const Vec3& a, const Vec3& b, f32 t);
    Vec4 lerp_vec4(const Vec4& a, const Vec4& b, f32 t);
    
    void spawn_animation_particles(WindowAnimation& anim, f32 progress);
    void apply_elastic_deformation(WindowAnimation& anim, f32 progress);
    
    void simulate_fluid_dynamics();
    void update_velocity_field();
    void apply_viscosity();
    void apply_pressure_forces();
    void advect_density();
    void apply_boundary_conditions();
    
    void simulate_spring_constraints();
    void simulate_rigid_body_physics();
    
    void update_particle_systems();
    void spawn_particles(ParticleSystem& system);
    Vec3 generate_spawn_position(const ParticleSystem& system);
    Vec3 generate_spawn_velocity(const ParticleSystem& system);
    void update_system_particles(ParticleSystem& system, f32 dt);
    void simulate_particle_physics();
    void update_particle_rendering();
    
    void process_bezier_curves();
    void update_bezier_curve(BezierCurve& curve);
    Vec3 evaluate_bezier(const Vector<Vec3>& control_points, f32 t);
    void process_spline_interpolation();
    void process_keyframe_blending();
    
    u32 count_active_animations() const;
    void cleanup_vulkan_resources();
    
    Vector<ParticleSystem> impl_->particle_systems_;
};

Vector<String> get_builtin_easing_functions();
Vector<String> get_builtin_window_animations();
Vector<String> get_builtin_transition_effects();
String format_animation_statistics(const FluidAnimationStats& stats);
f32 evaluate_easing_function(const String& function_name, f32 t);

} // namespace SU1

