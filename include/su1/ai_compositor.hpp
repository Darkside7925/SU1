#pragma once

#include "su1/core.hpp"
#include "su1/window.hpp"
#include "su1/theming.hpp"
#include <memory>
#include <atomic>
#include <future>
#include <thread>

namespace su1 {

// Forward declarations
class NeuralNetwork;
class QuantumProcessor;
class ConsciousnessAnalyzer;
class PredictionEngine;
class EmotionDetector;
class PerformanceOptimizer;
class FeatureExtractor;
class RewardCalculator;
class ThreadPool;

// AI Compositor configuration
struct AICompositorConfig {
    bool ai_enabled = true;
    bool quantum_enabled = false;
    bool consciousness_enabled = false;
    bool prediction_enabled = true;
    bool emotion_tracking_enabled = true;
    bool auto_optimization_enabled = true;
    bool learning_enabled = true;
    
    f32 adaptation_rate = 0.1f;
    f32 consciousness_sensitivity = 0.5f;
    f32 emotion_influence = 0.3f;
    f32 quantum_coherence_threshold = 0.7f;
    
    u32 neural_network_layers = 4;
    u32 prediction_horizon_frames = 60;
    u32 learning_batch_size = 32;
    u32 performance_history_size = 1000;
};

// Prediction structures
struct LayoutPrediction {
    u32 optimal_grid_size = 2;
    f32 preferred_aspect_ratio = 1.777f; // 16:9
    f32 window_spacing = 8.0f;
    f32 focus_enlargement = 1.2f;
    Vector<f32> window_priorities;
};

struct EffectsPrediction {
    f32 blur_intensity = 0.0f;
    f32 glow_intensity = 0.0f;
    f32 transparency_level = 1.0f;
    f32 animation_speed = 1.0f;
    f32 particle_density = 0.0f;
    f32 distortion_amount = 0.0f;
    
    // Consciousness effects
    f32 consciousness_level = 0.5f;
    f32 awareness_intensity = 0.0f;
    f32 enlightenment_glow = 0.0f;
    
    // Emotion effects
    f32 emotion_influence = 0.3f;
    f32 mood_color_shift = 0.0f;
    f32 stress_relief_amount = 0.0f;
    
    // Quantum effects
    f32 quantum_coherence = 0.0f;
    f32 superposition_blend = 0.0f;
    f32 entanglement_strength = 0.0f;
    f32 uncertainty_blur = 0.0f;
};

enum class PerformanceBottleneck {
    None = 0,
    CPU = 1,
    GPU = 2,
    Memory = 3,
    Bandwidth = 4
};

struct PerformancePrediction {
    f32 expected_frame_time = 16.67f; // 60 FPS
    f32 gpu_utilization = 0.5f;
    f32 memory_usage = 0.4f;
    f32 bandwidth_usage = 0.3f;
    
    bool should_use_lod = false;
    bool should_cull_offscreen = true;
    bool should_use_temporal_upsampling = false;
    bool should_reduce_effects = false;
    
    f32 optimal_resolution_scale = 1.0f;
    u32 optimal_refresh_rate = 60;
    u32 optimal_thread_count = 4;
    
    PerformanceBottleneck predicted_bottleneck = PerformanceBottleneck::None;
};

struct CompositionPrediction {
    LayoutPrediction optimal_layout;
    EffectsPrediction suggested_effects;
    PerformancePrediction performance_hints;
    f32 confidence = 0.5f;
};

// Consciousness state
struct ConsciousnessState {
    f32 awareness_level = 0.5f;
    f32 focus_intensity = 0.5f;
    f32 meditation_depth = 0.0f;
    f32 enlightenment_level = 0.0f;
    f32 transcendence_factor = 0.0f;
    f32 presence_quality = 0.5f;
    f32 mindfulness_score = 0.5f;
    Vector<f32> brainwave_patterns;
};

// Emotion state
struct EmotionState {
    f32 joy_level = 0.5f;
    f32 calm_level = 0.5f;
    f32 energy_level = 0.5f;
    f32 stress_level = 0.3f;
    f32 focus_level = 0.5f;
    f32 creativity_level = 0.5f;
    f32 satisfaction_level = 0.5f;
    Color dominant_emotion_color = Color::white();
    Vector<f32> emotion_history;
};

// Performance metrics
struct PerformanceMetrics {
    f32 average_frame_time = 16.67f;
    f32 average_compose_time = 5.0f;
    f32 gpu_utilization = 0.5f;
    f32 cpu_utilization = 0.4f;
    f32 memory_usage = 0.3f;
    f32 bandwidth_usage = 0.2f;
    u32 dropped_frames = 0;
    u32 total_frames = 0;
    f32 user_satisfaction_score = 0.7f;
};

// Learning statistics
struct LearningStats {
    u64 total_training_samples = 0;
    f32 average_reward = 0.0f;
    f32 learning_rate = 0.001f;
    f32 prediction_accuracy = 0.5f;
    f32 adaptation_speed = 0.1f;
    Vector<f32> reward_history;
};

// Render parameters
struct RenderParams {
    f32 quality_level = 1.0f;
    f32 lod_bias = 0.0f;
    bool use_temporal_upsampling = false;
    bool use_neural_enhancement = false;
    u32 sample_count = 1;
    f32 sharpening_amount = 0.0f;
};

// Render target
struct RenderTarget {
    u32 width = 1920;
    u32 height = 1080;
    u32 format = 0;
    void* handle = nullptr;
};

// Effect base class
class Effect {
public:
    virtual ~Effect() = default;
    virtual void apply(WindowPtr window, f32 intensity) = 0;
    virtual void update(f64 delta_time) = 0;
};

// Consciousness effects
class AwarenessGlowEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

class MeditationCalmEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

class EnlightenmentRadianceEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

class TranscendentBlurEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

// Emotion effects
class JoySparkleEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

class CalmWaveEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

class EnergyPulseEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

class StressReliefEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

// Quantum effects
class SuperpositionEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

class EntanglementEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

class UncertaintyEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

class TunnelingEffect : public Effect {
public:
    void apply(WindowPtr window, f32 intensity) override;
    void update(f64 delta_time) override;
};

// Main AI Compositor class
class AICompositor {
public:
    AICompositor();
    ~AICompositor();
    
    // Core functionality
    bool initialize();
    void shutdown();
    void update(f64 delta_time);
    void compose_frame(const Vector<WindowPtr>& windows, const RenderTarget& target);
    
    // Configuration
    void set_config(const AICompositorConfig& config) { config_ = config; }
    const AICompositorConfig& get_config() const { return config_; }
    
    // AI features
    void enable_ai(bool enabled) { ai_enabled_ = enabled; }
    void enable_quantum_effects(bool enabled) { quantum_enabled_ = enabled; }
    void enable_consciousness_tracking(bool enabled) { consciousness_enabled_ = enabled; }
    void enable_prediction(bool enabled) { prediction_enabled_ = enabled; }
    void enable_emotion_tracking(bool enabled) { emotion_tracking_enabled_ = enabled; }
    void enable_auto_optimization(bool enabled) { auto_optimization_enabled_ = enabled; }
    void enable_learning(bool enabled) { learning_enabled_ = enabled; }
    
    // State queries
    bool is_ai_enabled() const { return ai_enabled_; }
    bool is_quantum_enabled() const { return quantum_enabled_; }
    bool is_consciousness_enabled() const { return consciousness_enabled_; }
    bool is_prediction_enabled() const { return prediction_enabled_; }
    bool is_emotion_tracking_enabled() const { return emotion_tracking_enabled_; }
    bool is_auto_optimization_enabled() const { return auto_optimization_enabled_; }
    bool is_learning_enabled() const { return learning_enabled_; }
    
    // Parameters
    void set_adaptation_rate(f32 rate) { adaptation_rate_ = rate; }
    void set_consciousness_level(f32 level) { consciousness_level_ = level; }
    void set_emotion_influence(f32 influence) { emotion_influence_ = influence; }
    void set_quantum_coherence(f32 coherence) { quantum_coherence_ = coherence; }
    
    f32 get_adaptation_rate() const { return adaptation_rate_; }
    f32 get_consciousness_level() const { return consciousness_level_; }
    f32 get_emotion_influence() const { return emotion_influence_; }
    f32 get_quantum_coherence() const { return quantum_coherence_; }
    
    // Metrics
    const PerformanceMetrics& get_performance_metrics() const { return performance_metrics_; }
    const LearningStats& get_learning_stats() const { return learning_stats_; }
    
    // Manual overrides
    void override_consciousness_state(const ConsciousnessState& state);
    void override_emotion_state(const EmotionState& state);
    void force_performance_mode(PerformanceBottleneck bottleneck);
    
    // Advanced features
    void train_with_user_feedback(f32 satisfaction_score);
    void save_learned_model(const String& path);
    bool load_learned_model(const String& path);
    void reset_learning();
    
    // Debugging and profiling
    void enable_debug_visualization(bool enabled) { debug_visualization_ = enabled; }
    void enable_performance_profiling(bool enabled) { performance_profiling_ = enabled; }
    String get_debug_info() const;
    String get_profiling_info() const;

private:
    // Core components
    std::unique_ptr<NeuralNetwork> neural_network_;
    std::unique_ptr<QuantumProcessor> quantum_processor_;
    std::unique_ptr<ConsciousnessAnalyzer> consciousness_analyzer_;
    std::unique_ptr<PredictionEngine> prediction_engine_;
    std::unique_ptr<EmotionDetector> emotion_detector_;
    std::unique_ptr<PerformanceOptimizer> performance_optimizer_;
    std::unique_ptr<FeatureExtractor> feature_extractor_;
    std::unique_ptr<RewardCalculator> reward_calculator_;
    std::unique_ptr<ThreadPool> ai_thread_pool_;
    
    // Configuration
    AICompositorConfig config_;
    
    // State
    std::atomic<bool> initialized_;
    std::atomic<bool> ai_enabled_;
    std::atomic<bool> quantum_enabled_;
    std::atomic<bool> consciousness_enabled_;
    std::atomic<bool> prediction_enabled_;
    std::atomic<bool> emotion_tracking_enabled_;
    std::atomic<bool> auto_optimization_enabled_;
    std::atomic<bool> learning_enabled_;
    
    // Parameters
    f32 adaptation_rate_;
    f32 consciousness_level_;
    f32 emotion_influence_;
    f32 quantum_coherence_;
    
    // Metrics and statistics
    PerformanceMetrics performance_metrics_;
    LearningStats learning_stats_;
    
    // Feature vectors
    Vector<f32> temporal_features_;
    Vector<f32> user_behavior_features_;
    Vector<f32> environmental_features_;
    
    // Effects
    Map<String, std::unique_ptr<Effect>> consciousness_effects_;
    Map<String, std::unique_ptr<Effect>> emotion_effects_;
    Map<String, std::unique_ptr<Effect>> quantum_effects_;
    
    // Threading
    std::atomic<bool> should_stop_threads_;
    std::future<void> learning_future_;
    std::future<void> consciousness_future_;
    std::future<void> emotion_future_;
    std::future<void> optimization_future_;
    
    // Debug and profiling
    bool debug_visualization_ = false;
    bool performance_profiling_ = false;
    
    // Core AI functions
    CompositionPrediction predict_optimal_composition(const Vector<WindowPtr>& windows);
    void apply_consciousness_effects(const Vector<WindowPtr>& windows, 
                                   const CompositionPrediction& prediction);
    void apply_emotion_theming(const Vector<WindowPtr>& windows);
    void apply_quantum_composition(const Vector<WindowPtr>& windows,
                                 const CompositionPrediction& prediction);
    void render_with_ai_optimizations(const Vector<WindowPtr>& windows,
                                    const RenderTarget& target,
                                    const CompositionPrediction& prediction);
    void learn_from_composition(const Vector<WindowPtr>& windows,
                              const CompositionPrediction& prediction);
    
    // State update functions
    void update_consciousness_level(f64 delta_time);
    void update_emotion_state(f64 delta_time);
    void update_quantum_coherence(f64 delta_time);
    void run_predictions(f64 delta_time);
    void optimize_performance(f64 delta_time);
    void learn_from_current_state(f64 delta_time);
    
    // Helper functions
    Vector<f32> extract_window_features(const Vector<WindowPtr>& windows);
    LayoutPrediction interpret_layout_prediction(const Vector<f32>& output);
    EffectsPrediction interpret_effects_prediction(const Vector<f32>& output);
    PerformancePrediction interpret_performance_prediction(const Vector<f32>& output);
    f32 calculate_prediction_confidence(const Vector<f32>& output);
    
    // Consciousness functions
    f32 calculate_awareness_alpha(WindowPtr window, const ConsciousnessState& state);
    f32 calculate_focus_blur(WindowPtr window, const ConsciousnessState& state);
    void update_consciousness_effects(const ConsciousnessState& state);
    
    // Emotion functions
    Color calculate_emotion_color(const EmotionState& emotion);
    f32 calculate_emotion_animation_speed(const EmotionState& emotion);
    void apply_stress_relief_effects(WindowPtr window, f32 stress_level);
    void apply_joy_enhancement_effects(WindowPtr window, f32 joy_level);
    void update_emotion_effects(const EmotionState& emotion);
    
    // Quantum functions
    bool windows_overlap(WindowPtr window1, WindowPtr window2);
    void apply_quantum_superposition(WindowPtr window1, WindowPtr window2);
    void apply_quantum_entanglement(const Vector<WindowPtr>& windows);
    void apply_quantum_uncertainty_blur(const Vector<WindowPtr>& windows);
    void apply_quantum_tunneling_transitions(const Vector<WindowPtr>& windows);
    
    // Rendering optimizations
    void apply_attention_based_lod(const Vector<WindowPtr>& windows,
                                  const CompositionPrediction& prediction);
    void apply_predictive_texture_streaming(const Vector<WindowPtr>& windows,
                                           const CompositionPrediction& prediction);
    void apply_ai_culling(const Vector<WindowPtr>& windows,
                         const CompositionPrediction& prediction);
    void apply_neural_super_sampling(const RenderTarget& target);
    bool should_render_window(WindowPtr window, const CompositionPrediction& prediction);
    RenderParams calculate_optimal_render_params(WindowPtr window,
                                                const CompositionPrediction& prediction);
    void render_window_optimized(WindowPtr window, const RenderTarget& target,
                                const RenderParams& params);
    void apply_ai_post_processing(const RenderTarget& target,
                                const CompositionPrediction& prediction);
    
    // Learning functions
    void setup_learning_pipeline();
    void initialize_advanced_effects();
    PerformanceMetrics collect_performance_metrics();
    f32 calculate_learning_reward(const PerformanceMetrics& metrics,
                                const CompositionPrediction& prediction);
    void update_prediction_models();
    void analyze_user_patterns();
    
    // Threading functions
    void start_ai_threads();
    void stop_ai_threads();
    void background_learning_loop();
    void consciousness_monitoring_loop();
    void emotion_tracking_loop();
    void performance_optimization_loop();
};

using AICompositorPtr = SharedPtr<AICompositor>;

} // namespace su1
