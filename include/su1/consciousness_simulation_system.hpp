#pragma once

#include "su1/core.hpp"
#include <vector>
#include <memory>
#include <thread>
#include <atomic>
#include <string>
#include <chrono>

namespace SU1 {

class ConsciousnessSimulationSystem {
public:
    ConsciousnessSimulationSystem();
    ~ConsciousnessSimulationSystem();

    bool initialize();
    void shutdown();

    double get_consciousness_level() const;
    double get_attention_span() const;
    double get_emotional_stability() const;
    double get_cognitive_load() const;
    double get_adaptation_efficiency() const;
    double get_prediction_accuracy() const;
    double get_user_satisfaction() const;

    void set_learning_rate(double rate);
    void set_adaptation_rate(double rate);
    void set_prediction_horizon(double horizon);
    void set_consciousness_decay_rate(double rate);

    void enable_cognitive_modeling(bool enable);
    void enable_neural_networks(bool enable);
    void enable_adaptation_system(bool enable);
    void enable_prediction_system(bool enable);

    void process_user_action(const std::string& action_type, double intensity);
    void update_emotional_state(double valence, double arousal);
    void adapt_to_user_feedback(double feedback_score);
    void predict_user_needs();

private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    void cognition_update_loop();
    void neural_update_loop();
    void adaptation_update_loop();
    void prediction_update_loop();
};

} // namespace SU1

