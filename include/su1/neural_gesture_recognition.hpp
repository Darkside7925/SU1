#pragma once

#include "su1/core.hpp"
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <atomic>
#include <chrono>

namespace SU1 {

struct Vec2 {
    f32 x, y;
};

class NeuralGestureRecognition {
public:
    NeuralGestureRecognition();
    ~NeuralGestureRecognition();

    bool initialize();
    void shutdown();

    void process_mouse_input(const Vec2& position, float pressure);
    void process_keyboard_input(int key_code, bool is_press);

    std::string get_current_gesture() const;
    std::string get_predicted_intention() const;
    float get_gesture_recognition_accuracy() const;
    float get_intention_prediction_accuracy() const;

    void set_gesture_detection_threshold(float threshold);
    void set_intention_prediction_confidence(float confidence);
    void set_learning_rate(float rate);

    void enable_learning_mode(bool enable);
    void reset_gesture_templates();
    void add_custom_gesture(const std::string& name, const std::vector<float>& features);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    void gesture_recognition_loop();
    void intention_prediction_loop();
    void learning_loop();
    void adapt_neural_networks();
};

} // namespace SU1
