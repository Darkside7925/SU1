#pragma once

#include "su1/core.hpp"

namespace su1 {

// Forward declarations for all manager classes
class Manager;
class InputManager;
class Compositor;
class Theme;
class Config;
class NotificationManager;
class PowerManager;
class SecurityManager;
class AccessibilityManager;
class HotkeyManager;
class AnimationEngine;
class EffectEngine;
class SessionManager;
class NetworkManager;
class AudioManager;
class VideoManager;
class CameraManager;
class SensorManager;
class BluetoothManager;
class WifiManager;
class CellularManager;
class GPSManager;
class NFCManager;
class RFIDManager;
class BarcodeManager;
class FingerprintManager;
class FaceManager;
class IrisManager;
class VoiceManager;
class GestureManager;
class HapticManager;
class ForceManager;
class PressureManager;
class TemperatureManager;
class HumidityManager;
class LightManager;
class ColorManager;
class ProximityManager;
class AccelerationManager;
class GyroscopeManager;
class MagnetometerManager;
class BarometerManager;
class AltimeterManager;
class CompassManager;
class PedometerManager;
class StepManager;
class ActivityManager;
class HeartRateManager;
class BloodPressureManager;
class GlucoseManager;
class OxygenManager;
class ECGManager;
class EMGManager;
class EEGManager;
class EOGManager;
class GSRManager;
class RespirationManager;
class BodyTemperatureManager;
class SkinTemperatureManager;
class AmbientTemperatureManager;
class UVManager;
class RadiationManager;
class ChemicalManager;
class BiologicalManager;
class GeneticManager;
class NeuralManager;
class QuantumManager;
class RelativisticManager;
class SubatomicManager;
class NanoscopicManager;
class MicroscopicManager;
class MacroscopicManager;
class CosmicManager;
class UniversalManager;
class MultiversalManager;
class OmniversalManager;

// Forward declarations for other core classes
class DisplayServer;
class DisplayManager;
class WindowManager;
class Renderer;
class PluginManager;
class PerformanceMonitor;

// Forward declarations for UI classes
class Component;
class Window;
class Layout;
class Workspace;
class ThemeManager;
class UIManager;

// Forward declarations for advanced classes
class QuantumStateEngine;
class ConsciousnessAnalyzer;
class LayoutPredictor;
class HarmonyCalculator;
class NeuralNetwork;
class QuantumProcessor;
class PredictionEngine;
class EmotionDetector;
class PerformanceOptimizer;
class FeatureExtractor;
class RewardCalculator;
class ThreadPool;
class GPULayoutEngine;
class GPUTextRenderer;
class GPUEffectProcessor;
class NeuralEffectProcessor;
class ConsciousnessFieldRenderer;
class QuantumEffectSimulator;

// Common structures
struct Point {
    f32 x = 0.0f;
    f32 y = 0.0f;

    Point() = default;
    Point(f32 x, f32 y) : x(x), y(y) {}
};

struct Size {
    f32 width = 0.0f;
    f32 height = 0.0f;

    Size() = default;
    Size(f32 w, f32 h) : width(w), height(h) {}
};

struct Rect {
    Point position;
    Size size;

    Rect() = default;
    Rect(f32 x, f32 y, f32 w, f32 h) : position(x, y), size(w, h) {}
    Rect(const Point& p, const Size& s) : position(p), size(s) {}
};

struct Color {
    f32 r = 0.0f;
    f32 g = 0.0f;
    f32 b = 0.0f;
    f32 a = 1.0f;

    Color() = default;
    Color(f32 r, f32 g, f32 b, f32 a = 1.0f) : r(r), g(g), b(b), a(a) {}
};

enum class Alignment {
    TopLeft = 0,
    TopCenter = 1,
    TopRight = 2,
    MiddleLeft = 3,
    MiddleCenter = 4,
    MiddleRight = 5,
    BottomLeft = 6,
    BottomCenter = 7,
    BottomRight = 8
};

enum class ComponentType {
    None = 0,
    Panel = 1,
    Button = 2,
    Label = 3,
    TextBox = 4,
    CheckBox = 5,
    RadioButton = 6,
    ComboBox = 7,
    ListBox = 8,
    ScrollBar = 9,
    ProgressBar = 10,
    Slider = 11,
    TabControl = 12,
    TreeView = 13,
    Menu = 14,
    MenuBar = 15,
    ToolBar = 16,
    StatusBar = 17,
    Dock = 18,
    Splitter = 19,
    GroupBox = 20,
    Frame = 21,
    Dialog = 22,
    Window = 23,
    Custom = 999
};

enum class LayoutType {
    None = 0,
    Vertical = 1,
    Horizontal = 2,
    Grid = 3,
    Flow = 4,
    Absolute = 5,
    Docking = 6,
    Tiling = 7,
    Floating = 8,
    Stacking = 9,
    Tabbed = 10,
    Accordion = 11,
    Masonry = 12,
    Flex = 13,
    Quantum = 14,
    Consciousness = 15,
    Neural = 16,
    Harmonic = 17
};

// Timer class declaration
class Timer {
public:
    Timer();
    ~Timer();

    void start();
    void stop();
    void reset();
    f64 elapsed_seconds() const;
    f64 elapsed_milliseconds() const;
    f64 elapsed_microseconds() const;
    f64 elapsed_nanoseconds() const;

private:
    // Implementation details would go here
};

// Atomic class declaration
template<typename T>
class Atomic {
public:
    Atomic() = default;
    Atomic(T value) : value_(value) {}

    T load() const { return value_.load(); }
    void store(T value) { value_.store(value); }
    T exchange(T value) { return value_.exchange(value); }
    bool compare_exchange_weak(T& expected, T desired) {
        return value_.compare_exchange_weak(expected, desired);
    }
    bool compare_exchange_strong(T& expected, T desired) {
        return value_.compare_exchange_strong(expected, desired);
    }

    Atomic& operator=(T value) {
        store(value);
        return *this;
    }

    operator T() const { return load(); }

private:
    std::atomic<T> value_;
};

// Mutex class declaration
class Mutex {
public:
    Mutex();
    ~Mutex();

    void lock();
    bool try_lock();
    void unlock();

private:
    std::mutex mutex_;
};

// SharedMutex class declaration
class SharedMutex {
public:
    SharedMutex();
    ~SharedMutex();

    void lock();
    void unlock();
    bool try_lock();

    void lock_shared();
    void unlock_shared();
    bool try_lock_shared();

private:
    std::shared_mutex mutex_;
};

// Thread class declaration
class Thread {
public:
    Thread();
    Thread(std::function<void()> func);
    ~Thread();

    void join();
    bool joinable() const;
    void detach();
    void swap(Thread& other);

    static unsigned int hardware_concurrency();

private:
    std::thread thread_;
};

// ConditionVariable class declaration
class ConditionVariable {
public:
    ConditionVariable();
    ~ConditionVariable();

    void notify_one();
    void notify_all();
    void wait(std::unique_lock<Mutex>& lock);
    template<class Predicate>
    void wait(std::unique_lock<Mutex>& lock, Predicate pred);

private:
    std::condition_variable cv_;
};

// Common typedefs
using TimerPtr = std::shared_ptr<Timer>;
using MutexPtr = std::shared_ptr<Mutex>;
using ThreadPtr = std::shared_ptr<Thread>;
using ConditionVariablePtr = std::shared_ptr<ConditionVariable>;

// Additional commonly needed structures
struct Vector2 {
    f32 x = 0.0f;
    f32 y = 0.0f;

    Vector2() = default;
    Vector2(f32 x, f32 y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(f32 scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    f32 length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalized() const {
        f32 len = length();
        if (len > 0.0f) {
            return Vector2(x / len, y / len);
        }
        return Vector2(0.0f, 0.0f);
    }
};

struct Vector3 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;

    Vector3() = default;
    Vector3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
};

struct Vector4 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 w = 0.0f;

    Vector4() = default;
    Vector4(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
    Vector4(const Vector3& v, f32 w) : x(v.x), y(v.y), z(v.z), w(w) {}
    Vector4(const Color& c) : x(c.r), y(c.g), z(c.b), w(c.a) {}
};

struct Matrix4x4 {
    f32 m[16] = {0};

    Matrix4x4() {
        // Identity matrix
        m[0] = m[5] = m[10] = m[15] = 1.0f;
    }

    static Matrix4x4 translation(const Vector3& v) {
        Matrix4x4 result;
        result.m[12] = v.x;
        result.m[13] = v.y;
        result.m[14] = v.z;
        return result;
    }

    static Matrix4x4 rotation(f32 angle, const Vector3& axis) {
        // Simplified rotation matrix implementation
        Matrix4x4 result;
        f32 c = std::cos(angle);
        f32 s = std::sin(angle);
        f32 t = 1.0f - c;

        result.m[0] = t * axis.x * axis.x + c;
        result.m[1] = t * axis.x * axis.y - s * axis.z;
        result.m[2] = t * axis.x * axis.z + s * axis.y;
        result.m[4] = t * axis.x * axis.y + s * axis.z;
        result.m[5] = t * axis.y * axis.y + c;
        result.m[6] = t * axis.y * axis.z - s * axis.x;
        result.m[8] = t * axis.x * axis.z - s * axis.y;
        result.m[9] = t * axis.y * axis.z + s * axis.x;
        result.m[10] = t * axis.z * axis.z + c;

        return result;
    }

    static Matrix4x4 scale(const Vector3& v) {
        Matrix4x4 result;
        result.m[0] = v.x;
        result.m[5] = v.y;
        result.m[10] = v.z;
        return result;
    }
};

struct Quaternion {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 w = 1.0f;

    Quaternion() = default;
    Quaternion(f32 x, f32 y, f32 z, f32 w) : x(x), y(y), z(z), w(w) {}
};

struct Transform {
    Vector3 position = Vector3(0, 0, 0);
    Quaternion rotation = Quaternion(0, 0, 0, 1);
    Vector3 scale = Vector3(1, 1, 1);

    Transform() = default;
    Transform(const Vector3& pos, const Quaternion& rot, const Vector3& scl)
        : position(pos), rotation(rot), scale(scl) {}

    Matrix4x4 to_matrix() const {
        Matrix4x4 translation_matrix = Matrix4x4::translation(position);
        Matrix4x4 rotation_matrix = Matrix4x4::rotation(0.0f, Vector3(0, 1, 0)); // Placeholder
        Matrix4x4 scale_matrix = Matrix4x4::scale(scale);

        // Combine matrices (simplified - in reality you'd multiply them properly)
        return scale_matrix;
    }
};

// Input event structures
enum class KeyCode {
    Unknown = 0,
    Space = 32,
    Apostrophe = 39,
    Comma = 44,
    Minus = 45,
    Period = 46,
    Slash = 47,
    Num0 = 48,
    Num1 = 49,
    Num2 = 50,
    Num3 = 51,
    Num4 = 52,
    Num5 = 53,
    Num6 = 54,
    Num7 = 55,
    Num8 = 56,
    Num9 = 57,
    Semicolon = 59,
    Equal = 61,
    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,
    LeftBracket = 91,
    Backslash = 92,
    RightBracket = 93,
    GraveAccent = 96,
    World1 = 161,
    World2 = 162,
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,
    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348
};

enum class MouseButton {
    Left = 0,
    Right = 1,
    Middle = 2,
    Button4 = 3,
    Button5 = 4,
    Button6 = 5,
    Button7 = 6,
    Button8 = 7
};

struct InputEvent {
    enum class Type {
        KeyDown,
        KeyUp,
        MouseDown,
        MouseUp,
        MouseMove,
        MouseWheel,
        TextInput,
        TouchBegin,
        TouchMove,
        TouchEnd,
        Gesture
    };

    Type type;
    KeyCode key = KeyCode::Unknown;
    MouseButton button = MouseButton::Left;
    Point position;
    Point delta;
    f32 wheel_delta = 0.0f;
    String text;
    u32 modifiers = 0;
    u64 timestamp = 0;

    InputEvent(Type t = Type::KeyDown) : type(t) {}
};

// Utility functions
template<typename T>
String to_string(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

template<typename T>
T from_string(const String& str) {
    std::stringstream ss(str);
    T value;
    ss >> value;
    return value;
}

inline String format_string(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[1024];
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return String(buffer);
}

template<typename... Args>
String format_string(const char* format, Args... args) {
    char buffer[1024];
    snprintf(buffer, sizeof(buffer), format, args...);
    return String(buffer);
}

// Math constants and functions
namespace math {
    constexpr f64 PI = 3.14159265358979323846;
    constexpr f64 E = 2.71828182845904523536;
    constexpr f64 SQRT2 = 1.41421356237309504880;
    constexpr f64 SQRT3 = 1.73205080756887729352;

    template<typename T>
    T abs(T value) { return std::abs(value); }

    template<typename T>
    T min(T a, T b) { return std::min(a, b); }

    template<typename T>
    T max(T a, T b) { return std::max(a, b); }

    template<typename T>
    T clamp(T value, T min_val, T max_val) {
        return std::clamp(value, min_val, max_val);
    }

    f32 sin(f32 x) { return std::sin(x); }
    f32 cos(f32 x) { return std::cos(x); }
    f32 tan(f32 x) { return std::tan(x); }
    f32 asin(f32 x) { return std::asin(x); }
    f32 acos(f32 x) { return std::acos(x); }
    f32 atan(f32 x) { return std::atan(x); }
    f32 atan2(f32 y, f32 x) { return std::atan2(y, x); }

    f32 sqrt(f32 x) { return std::sqrt(x); }
    f32 pow(f32 base, f32 exp) { return std::pow(base, exp); }
    f32 exp(f32 x) { return std::exp(x); }
    f32 log(f32 x) { return std::log(x); }
    f32 log10(f32 x) { return std::log10(x); }
    f32 log2(f32 x) { return std::log2(x); }

    f32 ceil(f32 x) { return std::ceil(x); }
    f32 floor(f32 x) { return std::floor(x); }
    f32 round(f32 x) { return std::round(x); }
    f32 trunc(f32 x) { return std::trunc(x); }
}

} // namespace su1
