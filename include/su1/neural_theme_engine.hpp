#pragma once

#include "su1/core.hpp"
#include <vulkan/vulkan.h>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>
#include <random>

namespace SU1 {

struct ThemeConfig {
    bool enable_neural_adaptation = true;
    bool enable_emotion_tracking = true;
    bool enable_circadian_adaptation = true;
    bool enable_productivity_optimization = true;
    bool enable_pattern_generation = true;
    bool enable_color_blindness_compensation = false;
    f32 adaptation_speed = 0.1f;
    f32 learning_rate = 0.001f;
    u32 emotion_history_size = 1000;
    u32 theme_cache_size = 100;
    u32 neural_network_layers = 8;
    u32 neurons_per_layer = 512;
};

struct ColorPalette {
    String name;
    Vec3 primary = {0.5f, 0.5f, 0.5f};
    Vec3 secondary = {0.4f, 0.4f, 0.4f};
    Vec3 accent = {0.6f, 0.6f, 0.6f};
    Vec3 background = {0.95f, 0.95f, 0.95f};
    Vec3 surface = {0.98f, 0.98f, 0.98f};
    Vec3 text_primary = {0.1f, 0.1f, 0.1f};
    Vec3 text_secondary = {0.4f, 0.4f, 0.4f};
    Vec3 error = {0.8f, 0.2f, 0.2f};
    Vec3 warning = {0.9f, 0.7f, 0.1f};
    Vec3 success = {0.2f, 0.8f, 0.2f};
    Vec3 info = {0.2f, 0.6f, 0.9f};
    Vector<String> emotion_associations;
    Vector<Vec3> variations;
    f32 contrast_ratio = 4.5f;
    f32 accessibility_score = 1.0f;
};

struct PatternData {
    String name;
    String type;
    String complexity;
    f32 scale = 1.0f;
    f32 rotation = 0.0f;
    f32 opacity = 0.1f;
    f32 color_influence = 0.3f;
    Vector<f32> parameters;
    Vec2 offset = {0.0f, 0.0f};
    Vec2 frequency = {1.0f, 1.0f};
    bool animated = false;
    f32 animation_speed = 1.0f;
};

struct AnimationStyle {
    f32 transition_duration = 400.0f;
    String easing_type = "ease_in_out";
    f32 bounce_factor = 0.1f;
    f32 elasticity = 0.05f;
    f32 damping = 0.8f;
    f32 stiffness = 100.0f;
    bool enable_micro_interactions = true;
    f32 hover_scale = 1.05f;
    f32 press_scale = 0.95f;
    f32 focus_glow = 0.3f;
};

struct AdaptationParameters {
    f32 emotional_weight = 0.4f;
    f32 circadian_weight = 0.3f;
    f32 productivity_weight = 0.2f;
    f32 user_preference_weight = 0.1f;
    f32 saturation_boost = 1.0f;
    f32 contrast_boost = 1.0f;
    f32 brightness_adjustment = 0.0f;
    f32 warmth_factor = 1.0f;
    f32 blue_light_filter = 0.0f;
    f32 focus_enhancement = 1.0f;
    f32 distraction_reduction = 1.0f;
    f32 energy_enhancement = 1.0f;
    f32 calming_effect = 1.0f;
    f32 reward_indication = 1.0f;
    f32 satisfaction_boost = 1.0f;
};

struct ThemeData {
    String id;
    String name;
    String description;
    String emotion_context;
    ColorPalette color_palette;
    Vector<PatternData> pattern_set;
    AnimationStyle animation_style;
    AdaptationParameters adaptation_parameters;
    f32 user_rating = 0.0f;
    f32 usage_frequency = 0.0f;
    f32 effectiveness_score = 0.0f;
    u64 creation_timestamp = 0;
    u64 last_used_timestamp = 0;
    Vector<String> context_tags;
    bool is_favorite = false;
    bool is_auto_generated = true;
};

struct EmotionData {
    String emotion_label;
    f32 valence = 0.5f;
    f32 arousal = 0.5f;
    f32 dominance = 0.5f;
    f32 confidence = 0.0f;
    u64 timestamp = 0;
    Vector<f32> raw_features;
    String detection_method;
    f32 stability = 0.0f;
};

struct UserPreference {
    String preference_type;
    String preference_value;
    f32 strength = 1.0f;
    u64 last_updated = 0;
    u32 usage_count = 0;
    f32 satisfaction_rating = 0.0f;
    Vector<String> context_conditions;
    bool is_explicit = true;
    bool is_learned = false;
};

struct TrainingExample {
    Vector<f32> input_features;
    f32 emotion_state;
    f32 time_of_day;
    f32 productivity_level;
    f32 stress_level;
    f32 environmental_brightness;
    Vector<Vec4> target_colors;
    Vector<f32> target_patterns;
    f32 user_rating;
    String context_label;
    bool is_positive_example = true;
};

struct NeuralNetwork {
    u32 layer_count = 8;
    u32 neurons_per_layer = 512;
    f32 learning_rate = 0.001f;
    f32 momentum = 0.9f;
    f32 weight_decay = 0.0001f;
    f32 dropout_rate = 0.2f;
    String activation_function = "tanh";
    String optimizer = "adam";
    u32 training_epochs = 1000;
    u32 batch_size = 32;
    f32 validation_split = 0.2f;
    bool use_batch_normalization = true;
    bool use_layer_normalization = false;
};

struct EmotionClassifier {
    u32 feature_count = 64;
    u32 class_count = 8;
    f32 confidence_threshold = 0.7f;
    Vector<String> emotion_labels;
    Vector<f32> feature_weights;
    Vector<f32> class_biases;
    String classification_method = "neural_network";
    f32 temporal_smoothing = 0.3f;
    u32 history_window_size = 10;
};

struct ColorHarmonyAnalyzer {
    Vector<String> harmony_rules;
    Vector<String> color_spaces;
    f32 harmony_threshold = 0.8f;
    f32 contrast_weight = 0.4f;
    f32 saturation_weight = 0.3f;
    f32 brightness_weight = 0.3f;
    bool use_perceptual_uniformity = true;
    String primary_color_space = "LAB";
};

struct PatternGenerator {
    Vector<String> pattern_types;
    Vector<String> complexity_levels;
    f32 coherence_factor = 0.8f;
    f32 novelty_factor = 0.2f;
    u32 pattern_resolution = 512;
    u32 pattern_cache_size = 1000;
    bool use_procedural_generation = true;
    bool enable_fractal_patterns = true;
    bool enable_noise_patterns = true;
};

struct ThemeEvolutionEngine {
    f32 mutation_rate = 0.1f;
    f32 crossover_rate = 0.7f;
    u32 population_size = 50;
    u32 generation_count = 100;
    f32 fitness_threshold = 0.9f;
    Vector<String> evolution_strategies;
    bool enable_elitism = true;
    f32 diversity_pressure = 0.3f;
    String selection_method = "tournament";
};

enum class ColorBlindnessType {
    None = 0,
    Protanopia = 1,
    Deuteranopia = 2,
    Tritanopia = 3,
    Protanomaly = 4,
    Deuteranomaly = 5,
    Tritanomaly = 6,
    Monochromacy = 7
};

struct NeuralThemeStats {
    std::atomic<u64> themes_generated{0};
    std::atomic<u64> emotions_processed{0};
    std::atomic<u64> adaptations_made{0};
    std::atomic<f64> neural_processing_time_ms{0.0};
    std::atomic<f64> theme_generation_time_ms{0.0};
    std::atomic<f64> emotion_detection_time_ms{0.0};
    std::atomic<f64> color_analysis_time_ms{0.0};
    std::atomic<f64> pattern_generation_time_ms{0.0};
    String current_emotion = "neutral";
    f32 emotion_confidence = 0.0f;
    f32 adaptation_accuracy = 0.0f;
    f32 user_satisfaction = 0.0f;
    f32 learning_progress = 0.0f;
    f32 theme_diversity = 0.0f;
    u32 active_patterns = 0;
    u32 cached_themes = 0;
    f64 neural_network_accuracy = 0.0;
    f64 color_harmony_score = 0.0;
};

class NeuralThemeEngine {
public:
    NeuralThemeEngine();
    ~NeuralThemeEngine();
    
    bool initialize(VkDevice device, VkPhysicalDevice physical_device, const ThemeConfig& config);
    void shutdown();
    
    ThemeData get_current_theme() const;
    Vector<ThemeData> get_available_themes() const;
    Vector<ThemeData> get_recent_themes(u32 count) const;
    Vector<ThemeData> get_themes_by_emotion(const String& emotion) const;
    Vector<ThemeData> get_themes_by_context(const Vector<String>& context_tags) const;
    
    void set_emotion_state(const String& emotion, f32 intensity);
    void set_user_context(f32 stress_level, f32 productivity, f32 environmental_brightness);
    void add_user_preference(const UserPreference& preference);
    void update_user_rating(const String& theme_id, f32 rating);
    
    void enable_neural_adaptation(bool enabled);
    void enable_emotion_tracking(bool enabled);
    void enable_circadian_adaptation(bool enabled);
    void enable_productivity_optimization(bool enabled);
    void enable_pattern_generation(bool enabled);
    void enable_color_blindness_compensation(ColorBlindnessType type);
    
    void set_adaptation_speed(f32 speed);
    void set_learning_rate(f32 rate);
    void set_neural_network_parameters(const NeuralNetwork& params);
    void set_emotion_classifier_parameters(const EmotionClassifier& params);
    
    Vector<ColorPalette> generate_color_palettes(u32 count);
    Vector<PatternData> generate_patterns(u32 count);
    ColorPalette analyze_color_harmony(const ColorPalette& palette);
    f32 calculate_accessibility_score(const ColorPalette& palette);
    
    void train_neural_network(const Vector<TrainingExample>& examples);
    void save_learning_state(const String& filename);
    void load_learning_state(const String& filename);
    void export_theme_data(const String& filename);
    void import_theme_data(const String& filename);
    
    Vector<String> get_available_emotions() const;
    Vector<String> get_available_patterns() const;
    Vector<String> get_available_color_spaces() const;
    Vector<String> get_supported_color_blindness_types() const;
    
    EmotionData detect_current_emotion();
    String classify_user_mood();
    f32 estimate_productivity_level();
    f32 estimate_stress_level();
    f32 calculate_circadian_factor();
    
    void calibrate_emotion_detection();
    void calibrate_color_perception();
    void reset_learning_state();
    void clear_theme_cache();
    void optimize_neural_network();
    
    NeuralThemeStats get_performance_stats() const;
    void reset_performance_counters();
    void enable_debug_mode(bool enabled);
    void set_debug_output_level(u32 level);
    
private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    
    bool create_vulkan_resources();
    bool create_compute_buffers();
    bool create_buffer(size_t size, VkBufferUsageFlags usage, VkBuffer* buffer, VkDeviceMemory* memory);
    
    bool initialize_neural_network();
    bool initialize_emotion_classifier();
    bool initialize_color_systems();
    bool initialize_pattern_systems();
    bool load_training_data();
    
    bool upload_neural_data_to_gpu();
    bool upload_training_data(const Vector<TrainingExample>& examples);
    
    void start_processing_threads();
    void stop_processing_threads();
    void neural_processing_loop();
    void emotion_analysis_loop();
    void theme_generation_loop();
    
    void process_neural_network();
    Vector<f32> gather_input_features();
    Vector<f32> forward_pass(const Vector<f32>& input);
    f32 activation_function(f32 x);
    void apply_theme_adjustments(const Vector<f32>& neural_output);
    
    void update_network_weights();
    f32 calculate_prediction_error();
    Vector<f32> calculate_gradients(f32 error);
    void apply_weight_updates(const Vector<f32>& gradients);
    
    void analyze_current_emotion();
    Vector<f32> extract_emotion_features();
    String classify_emotion(const Vector<f32>& features);
    void update_emotion_state(const String& emotion, const Vector<f32>& features);
    void update_emotion_history();
    
    void generate_adaptive_theme();
    ColorPalette generate_emotion_based_palette();
    Vector<PatternData> generate_context_based_patterns();
    AnimationStyle select_appropriate_animations();
    
    void apply_circadian_adjustments(ThemeData& theme);
    void adjust_for_night_mode(ThemeData& theme);
    void adjust_for_morning_mode(ThemeData& theme);
    void adjust_for_evening_mode(ThemeData& theme);
    
    void apply_productivity_optimizations(ThemeData& theme);
    
    void generate_base_color_palettes();
    void generate_palette_variations(ColorPalette& palette);
    Vec3 hsv_to_rgb(const Vec3& hsv);
    Vec3 rgb_to_hsv(const Vec3& rgb);
    
    void generate_base_patterns();
    void generate_pattern_parameters(PatternData& pattern);
    
    void normalize_colors(ColorPalette& palette);
    void compensate_for_color_blindness(ColorPalette& palette);
    void compensate_protanopia(ColorPalette& palette);
    void compensate_deuteranopia(ColorPalette& palette);
    void compensate_tritanopia(ColorPalette& palette);
    
    void cleanup_vulkan_resources();
};

Vector<String> get_emotion_categories();
Vector<String> get_color_harmony_types();
Vector<String> get_pattern_types();
f32 calculate_color_distance(const Vec3& color1, const Vec3& color2);
f32 calculate_contrast_ratio(const Vec3& foreground, const Vec3& background);
bool is_accessible_color_combination(const Vec3& foreground, const Vec3& background);
Vec3 adjust_color_for_accessibility(const Vec3& color, const Vec3& background, f32 target_contrast);
String format_theme_statistics(const NeuralThemeStats& stats);

} // namespace SU1
