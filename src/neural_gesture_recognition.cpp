#include "su1/neural_gesture_recognition.hpp"
#include "su1/core.hpp"
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <atomic>
#include <cmath>
#include <unordered_map>

namespace SU1 {

class NeuralGestureRecognition::Impl {
public:
    struct GesturePattern {
        std::string gesture_name;
        std::vector<Vec2> mouse_trajectory;
        std::vector<float> pressure_sequence;
        std::vector<float> timing_sequence;
        std::vector<KeyEvent> key_sequence;
        std::vector<float> neural_features;
        float confidence_score;
        std::chrono::steady_clock::time_point detection_time;
        bool is_gesture_start;
        bool is_gesture_end;
    };

    struct UserIntention {
        std::string intention_name;
        std::vector<std::string> associated_gestures;
        std::vector<float> intention_features;
        float prediction_probability;
        std::chrono::steady_clock::time_point prediction_time;
        bool is_active;
    };

    struct NeuralNetwork {
        std::vector<std::vector<float>> layers;
        std::vector<std::vector<float>> weights;
        std::vector<std::vector<float>> biases;
        std::vector<ActivationFunction> activations;

        NeuralNetwork(const std::vector<int>& layer_sizes) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::normal_distribution<float> weight_dist(0.0f, 0.1f);
            std::normal_distribution<float> bias_dist(0.0f, 0.01f);

            layers.resize(layer_sizes.size());
            weights.resize(layer_sizes.size() - 1);
            biases.resize(layer_sizes.size() - 1);
            activations.resize(layer_sizes.size() - 1, ActivationFunction::TANH);

            for (size_t i = 0; i < layer_sizes.size(); ++i) {
                layers[i].resize(layer_sizes[i]);
            }

            for (size_t i = 0; i < weights.size(); ++i) {
                weights[i].resize(layer_sizes[i] * layer_sizes[i + 1]);
                biases[i].resize(layer_sizes[i + 1]);

                for (auto& w : weights[i]) w = weight_dist(gen);
                for (auto& b : biases[i]) b = bias_dist(gen);
            }
        }

        std::vector<float> forward(const std::vector<float>& input) {
            layers[0] = input;

            for (size_t i = 0; i < weights.size(); ++i) {
                for (size_t j = 0; j < layers[i + 1].size(); ++j) {
                    float sum = biases[i][j];
                    for (size_t k = 0; k < layers[i].size(); ++k) {
                        sum += layers[i][k] * weights[i][k * layers[i + 1].size() + j];
                    }
                    layers[i + 1][j] = activate(sum, activations[i]);
                }
            }

            return layers.back();
        }

        float activate(float x, ActivationFunction func) {
            switch (func) {
                case ActivationFunction::RELU: return std::max(0.0f, x);
                case ActivationFunction::TANH: return std::tanh(x);
                case ActivationFunction::SIGMOID: return 1.0f / (1.0f + std::exp(-x));
                case ActivationFunction::ELU: return x > 0.0f ? x : std::exp(x) - 1.0f;
                default: return x;
            }
        }
    };

    enum class ActivationFunction {
        RELU,
        TANH,
        SIGMOID,
        ELU
    };

    struct KeyEvent {
        int key_code;
        bool is_press;
        std::chrono::steady_clock::time_point timestamp;
    };

    NeuralNetwork gesture_network;
    NeuralNetwork intention_network;
    NeuralNetwork prediction_network;

    std::vector<GesturePattern> detected_gestures;
    std::vector<UserIntention> user_intentions;
    std::vector<Vec2> current_mouse_trajectory;
    std::vector<float> current_pressure_sequence;
    std::vector<float> current_timing_sequence;
    std::vector<KeyEvent> current_key_sequence;

    std::atomic<bool> active_{true};
    std::thread gesture_thread_;
    std::thread intention_thread_;
    std::thread learning_thread_;

    float gesture_detection_threshold_;
    float intention_prediction_confidence_;
    float learning_rate_;
    int trajectory_buffer_size_;
    int gesture_history_window_;

    std::unordered_map<std::string, std::vector<float>> gesture_templates;
    std::unordered_map<std::string, std::vector<float>> intention_patterns;

    std::mt19937 random_engine_;
    std::normal_distribution<float> noise_dist_;
    std::uniform_real_distribution<float> uniform_dist_;

    float gesture_recognition_accuracy_;
    float intention_prediction_accuracy_;
    float false_positive_rate_;
    float response_time_ms_;

    Vec2 last_mouse_position_;
    Vec2 current_mouse_velocity_;
    float current_mouse_pressure_;
    std::chrono::steady_clock::time_point last_mouse_time_;
    std::chrono::steady_clock::time_point gesture_start_time_;

    bool is_gesture_active_;
    bool is_learning_mode_;
    std::string current_gesture_name_;
    std::string predicted_intention_;

public:
    Impl()
        : gesture_network({100, 200, 150, 100, 50})
        , intention_network({75, 150, 200, 150, 75})
        , prediction_network({50, 100, 150, 100, 50})
        , gesture_detection_threshold_(0.7f)
        , intention_prediction_confidence_(0.8f)
        , learning_rate_(0.001f)
        , trajectory_buffer_size_(200)
        , gesture_history_window_(1000)
        , random_engine_(std::random_device{}())
        , noise_dist_(0.0f, 0.01f)
        , uniform_dist_(0.0f, 1.0f)
        , gesture_recognition_accuracy_(0.0f)
        , intention_prediction_accuracy_(0.0f)
        , false_positive_rate_(0.0f)
        , response_time_ms_(0.0f)
        , last_mouse_position_({0.0f, 0.0f})
        , current_mouse_velocity_({0.0f, 0.0f})
        , current_mouse_pressure_(0.0f)
        , is_gesture_active_(false)
        , is_learning_mode_(false) {

        initialize_gesture_templates();
        initialize_intention_patterns();

        last_mouse_time_ = std::chrono::steady_clock::now();
        gesture_start_time_ = std::chrono::steady_clock::now();
    }

    ~Impl() {
        active_ = false;
        if (gesture_thread_.joinable()) gesture_thread_.join();
        if (intention_thread_.joinable()) intention_thread_.join();
        if (learning_thread_.joinable()) learning_thread_.join();
    }

    void initialize_gesture_templates() {
        // Mouse gesture templates
        gesture_templates["swipe_right"] = generate_gesture_features("horizontal_line", 200.0f, 0.0f);
        gesture_templates["swipe_left"] = generate_gesture_features("horizontal_line", -200.0f, 0.0f);
        gesture_templates["swipe_up"] = generate_gesture_features("vertical_line", 0.0f, -200.0f);
        gesture_templates["swipe_down"] = generate_gesture_features("vertical_line", 0.0f, 200.0f);
        gesture_templates["circle_clockwise"] = generate_gesture_features("circle", 100.0f, 1.0f);
        gesture_templates["circle_counterclockwise"] = generate_gesture_features("circle", 100.0f, -1.0f);
        gesture_templates["zigzag"] = generate_gesture_features("zigzag", 150.0f, 0.0f);
        gesture_templates["checkmark"] = generate_gesture_features("checkmark", 100.0f, 0.0f);

        // Keyboard gesture templates
        gesture_templates["ctrl_c"] = generate_keyboard_features({29, 46}, {true, true});
        gesture_templates["ctrl_v"] = generate_keyboard_features({29, 47}, {true, true});
        gesture_templates["ctrl_z"] = generate_keyboard_features({29, 44}, {true, true});
        gesture_templates["alt_tab"] = generate_keyboard_features({56, 15}, {true, true});
        gesture_templates["win_d"] = generate_keyboard_features({125, 32}, {true, true});

        // Multi-modal gesture templates
        gesture_templates["select_all"] = generate_multimodal_features("rectangle_selection", {29, 65}, {true, true});
        gesture_templates["copy_paste"] = generate_multimodal_features("drag_drop", {29, 46, 47}, {true, true, true});
    }

    void initialize_intention_patterns() {
        intention_patterns["open_application"] = {0.9f, 0.1f, 0.2f, 0.8f, 0.3f, 0.1f, 0.7f, 0.4f};
        intention_patterns["close_window"] = {0.2f, 0.8f, 0.9f, 0.1f, 0.6f, 0.3f, 0.2f, 0.7f};
        intention_patterns["switch_workspace"] = {0.4f, 0.6f, 0.3f, 0.7f, 0.9f, 0.2f, 0.5f, 0.8f};
        intention_patterns["maximize_window"] = {0.7f, 0.3f, 0.8f, 0.2f, 0.4f, 0.9f, 0.1f, 0.6f};
        intention_patterns["minimize_window"] = {0.3f, 0.7f, 0.4f, 0.6f, 0.2f, 0.8f, 0.9f, 0.1f};
        intention_patterns["resize_window"] = {0.6f, 0.4f, 0.7f, 0.3f, 0.8f, 0.2f, 0.5f, 0.9f};
        intention_patterns["move_window"] = {0.5f, 0.5f, 0.6f, 0.4f, 0.7f, 0.3f, 0.8f, 0.2f};
        intention_patterns["scroll_content"] = {0.8f, 0.2f, 0.5f, 0.7f, 0.3f, 0.9f, 0.4f, 0.6f};
        intention_patterns["zoom_content"] = {0.4f, 0.8f, 0.6f, 0.3f, 0.9f, 0.1f, 0.7f, 0.5f};
        intention_patterns["search_content"] = {0.7f, 0.4f, 0.8f, 0.3f, 0.5f, 0.9f, 0.2f, 0.6f};
    }

    std::vector<float> generate_gesture_features(const std::string& gesture_type, float amplitude, float direction) {
        std::vector<float> features(50, 0.0f);

        if (gesture_type == "horizontal_line") {
            for (size_t i = 0; i < 25; ++i) {
                features[i] = amplitude * (i / 24.0f);
                features[i + 25] = 0.0f;
            }
        } else if (gesture_type == "vertical_line") {
            for (size_t i = 0; i < 25; ++i) {
                features[i] = 0.0f;
                features[i + 25] = amplitude * (i / 24.0f);
            }
        } else if (gesture_type == "circle") {
            for (size_t i = 0; i < 25; ++i) {
                float angle = 2.0f * 3.14159f * i / 24.0f * direction;
                features[i] = amplitude * std::cos(angle);
                features[i + 25] = amplitude * std::sin(angle);
            }
        } else if (gesture_type == "zigzag") {
            for (size_t i = 0; i < 25; ++i) {
                float zig = (i % 2 == 0) ? 1.0f : -1.0f;
                features[i] = amplitude * (i / 24.0f) * zig;
                features[i + 25] = amplitude * 0.5f * std::sin(i * 0.5f);
            }
        } else if (gesture_type == "checkmark") {
            for (size_t i = 0; i < 12; ++i) {
                features[i] = amplitude * (i / 11.0f);
                features[i + 25] = amplitude * (i / 11.0f);
            }
            for (size_t i = 12; i < 25; ++i) {
                features[i] = amplitude * 0.5f + amplitude * 0.5f * ((i - 12) / 12.0f);
                features[i + 25] = amplitude * ((i - 12) / 12.0f);
            }
        }

        return features;
    }

    std::vector<float> generate_keyboard_features(const std::vector<int>& key_codes, const std::vector<bool>& is_presses) {
        std::vector<float> features(50, 0.0f);

        for (size_t i = 0; i < key_codes.size() && i < 25; ++i) {
            features[i] = key_codes[i] / 255.0f;
            features[i + 25] = is_presses[i] ? 1.0f : 0.0f;
        }

        return features;
    }

    std::vector<float> generate_multimodal_features(const std::string& mouse_gesture, const std::vector<int>& key_codes, const std::vector<bool>& is_presses) {
        auto mouse_features = generate_gesture_features(mouse_gesture, 100.0f, 0.0f);
        auto keyboard_features = generate_keyboard_features(key_codes, is_presses);

        std::vector<float> multimodal_features;
        multimodal_features.insert(multimodal_features.end(), mouse_features.begin(), mouse_features.end());
        multimodal_features.insert(multimodal_features.end(), keyboard_features.begin(), keyboard_features.end());

        return multimodal_features;
    }

    void process_mouse_input(const Vec2& mouse_position, float pressure) {
        auto current_time = std::chrono::steady_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - last_mouse_time_).count();

        if (delta_time > 0.0f) {
            current_mouse_velocity_.x = (mouse_position.x - last_mouse_position_.x) / delta_time;
            current_mouse_velocity_.y = (mouse_position.y - last_mouse_position_.y) / delta_time;
        }

        last_mouse_position_ = mouse_position;
        last_mouse_time_ = current_time;
        current_mouse_pressure_ = pressure;

        // Add to current trajectory
        current_mouse_trajectory.push_back(mouse_position);
        current_pressure_sequence.push_back(pressure);
        current_timing_sequence.push_back(delta_time);

        // Maintain buffer size
        if (current_mouse_trajectory.size() > trajectory_buffer_size_) {
            current_mouse_trajectory.erase(current_mouse_trajectory.begin());
            current_pressure_sequence.erase(current_pressure_sequence.begin());
            current_timing_sequence.erase(current_timing_sequence.begin());
        }

        // Check for gesture start
        if (!is_gesture_active_ && should_start_gesture()) {
            start_gesture();
        }

        // Check for gesture end
        if (is_gesture_active_ && should_end_gesture()) {
            end_gesture();
        }
    }

    void process_keyboard_input(int key_code, bool is_press) {
        KeyEvent key_event;
        key_event.key_code = key_code;
        key_event.is_press = is_press;
        key_event.timestamp = std::chrono::steady_clock::now();

        current_key_sequence.push_back(key_event);

        // Maintain buffer size
        if (current_key_sequence.size() > 50) {
            current_key_sequence.erase(current_key_sequence.begin());
        }

        // Check for keyboard gestures
        detect_keyboard_gesture();
    }

    bool should_start_gesture() {
        if (current_mouse_trajectory.size() < 5) return false;

        // Check for significant movement
        float total_distance = 0.0f;
        for (size_t i = 1; i < current_mouse_trajectory.size(); ++i) {
            Vec2 diff = {
                current_mouse_trajectory[i].x - current_mouse_trajectory[i-1].x,
                current_mouse_trajectory[i].y - current_mouse_trajectory[i-1].y
            };
            total_distance += std::sqrt(diff.x * diff.x + diff.y * diff.y);
        }

        return total_distance > 50.0f; // Minimum distance threshold
    }

    bool should_end_gesture() {
        if (current_mouse_trajectory.size() < 3) return false;

        // Check for movement slowdown
        Vec2 recent_velocity = {0.0f, 0.0f};
        if (current_mouse_trajectory.size() >= 3) {
            Vec2 diff = {
                current_mouse_trajectory.back().x - current_mouse_trajectory[current_mouse_trajectory.size() - 3].x,
                current_mouse_trajectory.back().y - current_mouse_trajectory[current_mouse_trajectory.size() - 3].y
            };
            recent_velocity.x = diff.x / 0.1f; // Approximate velocity
            recent_velocity.y = diff.y / 0.1f;
        }

        float speed = std::sqrt(recent_velocity.x * recent_velocity.x + recent_velocity.y * recent_velocity.y);
        return speed < 10.0f; // Speed threshold for gesture end
    }

    void start_gesture() {
        is_gesture_active_ = true;
        gesture_start_time_ = std::chrono::steady_clock::now();

        // Clear buffers to start fresh
        current_mouse_trajectory.clear();
        current_pressure_sequence.clear();
        current_timing_sequence.clear();
        current_key_sequence.clear();
    }

    void end_gesture() {
        is_gesture_active_ = false;

        // Extract gesture features
        std::vector<float> gesture_features = extract_gesture_features();

        // Recognize gesture
        std::string recognized_gesture = recognize_gesture(gesture_features);

        if (!recognized_gesture.empty() && !gesture_features.empty()) {
            GesturePattern pattern;
            pattern.gesture_name = recognized_gesture;
            pattern.mouse_trajectory = current_mouse_trajectory;
            pattern.pressure_sequence = current_pressure_sequence;
            pattern.timing_sequence = current_timing_sequence;
            pattern.key_sequence = current_key_sequence;
            pattern.neural_features = gesture_features;
            pattern.confidence_score = calculate_gesture_confidence(gesture_features, recognized_gesture);
            pattern.detection_time = std::chrono::steady_clock::now();
            pattern.is_gesture_start = true;
            pattern.is_gesture_end = true;

            detected_gestures.push_back(pattern);

            // Maintain history size
            if (detected_gestures.size() > gesture_history_window_) {
                detected_gestures.erase(detected_gestures.begin());
            }

            // Predict user intention
            predict_user_intention(recognized_gesture);
        }
    }

    std::vector<float> extract_gesture_features() {
        std::vector<float> features;

        if (current_mouse_trajectory.empty()) return features;

        // Trajectory features
        Vec2 start_pos = current_mouse_trajectory.front();
        Vec2 end_pos = current_mouse_trajectory.back();

        features.push_back(end_pos.x - start_pos.x); // Delta X
        features.push_back(end_pos.y - start_pos.y); // Delta Y

        float total_distance = 0.0f;
        for (size_t i = 1; i < current_mouse_trajectory.size(); ++i) {
            Vec2 diff = {
                current_mouse_trajectory[i].x - current_mouse_trajectory[i-1].x,
                current_mouse_trajectory[i].y - current_mouse_trajectory[i-1].y
            };
            total_distance += std::sqrt(diff.x * diff.x + diff.y * diff.y);
        }
        features.push_back(total_distance); // Total distance

        // Direction features
        float avg_direction_x = 0.0f;
        float avg_direction_y = 0.0f;
        for (size_t i = 1; i < current_mouse_trajectory.size(); ++i) {
            Vec2 diff = {
                current_mouse_trajectory[i].x - current_mouse_trajectory[i-1].x,
                current_mouse_trajectory[i].y - current_mouse_trajectory[i-1].y
            };
            float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
            if (length > 0.0f) {
                avg_direction_x += diff.x / length;
                avg_direction_y += diff.y / length;
            }
        }
        if (current_mouse_trajectory.size() > 1) {
            avg_direction_x /= (current_mouse_trajectory.size() - 1);
            avg_direction_y /= (current_mouse_trajectory.size() - 1);
        }
        features.push_back(avg_direction_x);
        features.push_back(avg_direction_y);

        // Curvature features
        float total_curvature = 0.0f;
        for (size_t i = 1; i < current_mouse_trajectory.size() - 1; ++i) {
            Vec2 v1 = {
                current_mouse_trajectory[i].x - current_mouse_trajectory[i-1].x,
                current_mouse_trajectory[i].y - current_mouse_trajectory[i-1].y
            };
            Vec2 v2 = {
                current_mouse_trajectory[i+1].x - current_mouse_trajectory[i].x,
                current_mouse_trajectory[i+1].y - current_mouse_trajectory[i].y
            };

            float dot_product = v1.x * v2.x + v1.y * v2.y;
            float mag1 = std::sqrt(v1.x * v1.x + v1.y * v1.y);
            float mag2 = std::sqrt(v2.x * v2.x + v2.y * v2.y);

            if (mag1 > 0.0f && mag2 > 0.0f) {
                float cos_angle = dot_product / (mag1 * mag2);
                cos_angle = std::max(-1.0f, std::min(1.0f, cos_angle));
                total_curvature += std::acos(cos_angle);
            }
        }
        features.push_back(total_curvature);

        // Speed features
        float avg_speed = 0.0f;
        float max_speed = 0.0f;
        for (size_t i = 0; i < current_timing_sequence.size(); ++i) {
            if (current_timing_sequence[i] > 0.0f) {
                Vec2 velocity = current_mouse_velocity_;
                float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
                avg_speed += speed;
                max_speed = std::max(max_speed, speed);
            }
        }
        if (!current_timing_sequence.empty()) {
            avg_speed /= current_timing_sequence.size();
        }
        features.push_back(avg_speed);
        features.push_back(max_speed);

        // Pressure features
        float avg_pressure = 0.0f;
        float pressure_variance = 0.0f;
        for (float pressure : current_pressure_sequence) {
            avg_pressure += pressure;
        }
        if (!current_pressure_sequence.empty()) {
            avg_pressure /= current_pressure_sequence.size();

            for (float pressure : current_pressure_sequence) {
                pressure_variance += (pressure - avg_pressure) * (pressure - avg_pressure);
            }
            pressure_variance /= current_pressure_sequence.size();
        }
        features.push_back(avg_pressure);
        features.push_back(pressure_variance);

        // Pad to fixed size
        while (features.size() < 50) {
            features.push_back(0.0f);
        }

        return features;
    }

    std::string recognize_gesture(const std::vector<float>& features) {
        std::string best_match;
        float best_score = 0.0f;

        for (const auto& [gesture_name, template_features] : gesture_templates) {
            float similarity = calculate_feature_similarity(features, template_features);
            if (similarity > best_score && similarity > gesture_detection_threshold_) {
                best_score = similarity;
                best_match = gesture_name;
            }
        }

        return best_match;
    }

    float calculate_gesture_confidence(const std::vector<float>& features, const std::string& gesture_name) {
        auto it = gesture_templates.find(gesture_name);
        if (it == gesture_templates.end()) return 0.0f;

        return calculate_feature_similarity(features, it->second);
    }

    float calculate_feature_similarity(const std::vector<float>& features1, const std::vector<float>& features2) {
        if (features1.size() != features2.size()) return 0.0f;

        float dot_product = 0.0f;
        float norm1 = 0.0f;
        float norm2 = 0.0f;

        for (size_t i = 0; i < features1.size(); ++i) {
            dot_product += features1[i] * features2[i];
            norm1 += features1[i] * features1[i];
            norm2 += features2[i] * features2[i];
        }

        if (norm1 == 0.0f || norm2 == 0.0f) return 0.0f;

        return dot_product / (std::sqrt(norm1) * std::sqrt(norm2));
    }

    void detect_keyboard_gesture() {
        if (current_key_sequence.size() < 2) return;

        // Check for common keyboard shortcuts
        if (current_key_sequence.size() >= 2) {
            KeyEvent last_key = current_key_sequence.back();
            KeyEvent prev_key = current_key_sequence[current_key_sequence.size() - 2];

            // Check for Ctrl+C
            if (prev_key.key_code == 29 && last_key.key_code == 46 && prev_key.is_press && last_key.is_press) {
                create_keyboard_gesture("ctrl_c");
            }
            // Check for Ctrl+V
            else if (prev_key.key_code == 29 && last_key.key_code == 47 && prev_key.is_press && last_key.is_press) {
                create_keyboard_gesture("ctrl_v");
            }
        }
    }

    void create_keyboard_gesture(const std::string& gesture_name) {
        GesturePattern pattern;
        pattern.gesture_name = gesture_name;
        pattern.key_sequence = current_key_sequence;
        pattern.neural_features = generate_keyboard_features({29, 46}, {true, true}); // Example features
        pattern.confidence_score = 0.9f;
        pattern.detection_time = std::chrono::steady_clock::now();
        pattern.is_gesture_start = true;
        pattern.is_gesture_end = true;

        detected_gestures.push_back(pattern);

        // Clear key sequence
        current_key_sequence.clear();

        // Predict intention
        predict_user_intention(gesture_name);
    }

    void predict_user_intention(const std::string& gesture_name) {
        std::vector<float> intention_input;

        // Add gesture features
        auto gesture_it = gesture_templates.find(gesture_name);
        if (gesture_it != gesture_templates.end()) {
            intention_input.insert(intention_input.end(), gesture_it->second.begin(), gesture_it->second.end());
        }

        // Add context features
        intention_input.push_back(current_mouse_trajectory.size() / 100.0f);
        intention_input.push_back(current_mouse_velocity_.x / 1000.0f);
        intention_input.push_back(current_mouse_velocity_.y / 1000.0f);
        intention_input.push_back(current_mouse_pressure_);

        // Pad to network input size
        while (intention_input.size() < 75) {
            intention_input.push_back(0.0f);
        }

        // Get intention prediction
        auto intention_output = intention_network.forward(intention_input);

        // Find best intention match
        std::string best_intention;
        float best_probability = 0.0f;

        for (const auto& [intention_name, intention_features] : intention_patterns) {
            float similarity = calculate_feature_similarity(intention_output, intention_features);
            if (similarity > best_probability && similarity > intention_prediction_confidence_) {
                best_probability = similarity;
                best_intention = intention_name;
            }
        }

        if (!best_intention.empty()) {
            predicted_intention_ = best_intention;

            // Create user intention
            UserIntention intention;
            intention.intention_name = best_intention;
            intention.associated_gestures = {gesture_name};
            intention.intention_features = intention_output;
            intention.prediction_probability = best_probability;
            intention.prediction_time = std::chrono::steady_clock::now();
            intention.is_active = true;

            user_intentions.push_back(intention);

            // Maintain history
            if (user_intentions.size() > 50) {
                user_intentions.erase(user_intentions.begin());
            }
        }
    }

    void update_recognition_metrics() {
        // Update accuracy metrics based on recent gestures
        if (!detected_gestures.empty()) {
            float total_confidence = 0.0f;
            for (const auto& gesture : detected_gestures) {
                total_confidence += gesture.confidence_score;
            }
            gesture_recognition_accuracy_ = total_confidence / detected_gestures.size();
        }

        if (!user_intentions.empty()) {
            float total_probability = 0.0f;
            for (const auto& intention : user_intentions) {
                total_probability += intention.prediction_probability;
            }
            intention_prediction_accuracy_ = total_probability / user_intentions.size();
        }

        // Calculate response time
        if (!detected_gestures.empty()) {
            auto last_gesture = detected_gestures.back();
            auto response_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                last_gesture.detection_time - gesture_start_time_).count();
            response_time_ms_ = response_time;
        }
    }
};

NeuralGestureRecognition::NeuralGestureRecognition() : impl_(std::make_unique<Impl>()) {}

NeuralGestureRecognition::~NeuralGestureRecognition() = default;

bool NeuralGestureRecognition::initialize() {
    impl_->gesture_thread_ = std::thread([this]() {
        gesture_recognition_loop();
    });

    impl_->intention_thread_ = std::thread([this]() {
        intention_prediction_loop();
    });

    impl_->learning_thread_ = std::thread([this]() {
        learning_loop();
    });

    return true;
}

void NeuralGestureRecognition::shutdown() {
    impl_->active_ = false;
    if (impl_->gesture_thread_.joinable()) impl_->gesture_thread_.join();
    if (impl_->intention_thread_.joinable()) impl_->intention_thread_.join();
    if (impl_->learning_thread_.joinable()) impl_->learning_thread_.join();
}

void NeuralGestureRecognition::process_mouse_input(const Vec2& position, float pressure) {
    impl_->process_mouse_input(position, pressure);
}

void NeuralGestureRecognition::process_keyboard_input(int key_code, bool is_press) {
    impl_->process_keyboard_input(key_code, is_press);
}

std::string NeuralGestureRecognition::get_current_gesture() const {
    return impl_->current_gesture_name_;
}

std::string NeuralGestureRecognition::get_predicted_intention() const {
    return impl_->predicted_intention_;
}

float NeuralGestureRecognition::get_gesture_recognition_accuracy() const {
    return impl_->gesture_recognition_accuracy_;
}

float NeuralGestureRecognition::get_intention_prediction_accuracy() const {
    return impl_->intention_prediction_accuracy_;
}

void NeuralGestureRecognition::gesture_recognition_loop() {
    while (impl_->active_) {
        impl_->update_recognition_metrics();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void NeuralGestureRecognition::intention_prediction_loop() {
    while (impl_->active_) {
        // Continuous intention monitoring
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void NeuralGestureRecognition::learning_loop() {
    while (impl_->active_) {
        // Continuous learning and adaptation
        if (impl_->is_learning_mode_ && !impl_->detected_gestures.empty()) {
            // Adapt neural networks based on user feedback
            adapt_neural_networks();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    }
}

void NeuralGestureRecognition::adapt_neural_networks() {
    // Implement neural network adaptation based on user feedback
    // This would involve updating weights based on correct/incorrect recognitions
}

} // namespace SU1
