#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <atomic>
#include <mutex>
#include <chrono>
#include <thread>
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <random>
#include <limits>
#include <type_traits>
#include <utility>
#include <tuple>
#include <variant>
#include <optional>
#include <any>
#include <bitset>
#include <array>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <unordered_set>
#include <list>
#include <forward_list>
#include <iterator>
#include <ranges>
#include <concepts>
#include <coroutine>
#include <future>
#include <exception>
#include <stdexcept>
#include <system_error>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <csignal>
#include <csetjmp>
#include <cfenv>
#include <cfloat>
#include <climits>
#include <cstdint>
#include <cinttypes>
#include <cstdbool>
#include <ccomplex>
#include <ctgmath>
#include <ciso646>
#include <cstdalign>
#include <cstdarg>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cwchar>
#include <cwctype>
#include <cmath>
#include <complex>
#include <numeric>
#include <random>
#include <ratio>
#include <regex>
#include <scoped_allocator>
#include <system_error>
#include <typeindex>
#include <typeinfo>
#include <valarray>
#include <variant>
#include <vector>
#include <bit>
#include <bitset>
#include <charconv>
#include <compare>
#include <coroutine>
#include <numbers>
#include <ranges>
#include <span>
#include <source_location>
#include <syncstream>
#include <version>

namespace su1 {

using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;
using f32 = float;
using f64 = double;
using f128 = long double;

using usize = std::size_t;
using isize = std::ptrdiff_t;

template<typename T>
using Ref = std::reference_wrapper<T>;

template<typename T>
using Ptr = std::unique_ptr<T>;

template<typename T>
using SharedPtr = std::shared_ptr<T>;

template<typename T>
using WeakPtr = std::weak_ptr<T>;

template<typename T>
using Atomic = std::atomic<T>;

template<typename... Args>
using Tuple = std::tuple<Args...>;

template<typename T>
using Optional = std::optional<T>;

template<typename T>
using Vector = std::vector<T>;

template<typename K, typename V>
using Map = std::unordered_map<K, V>;

template<typename T>
using Set = std::unordered_set<T>;

template<typename T>
using Queue = std::queue<T>;

template<typename T>
using Deque = std::deque<T>;

template<typename T>
using Stack = std::stack<T>;

template<typename T, usize N>
using Array = std::array<T, N>;

template<typename T>
using Function = std::function<T>;

template<typename... Types>
using Variant = std::variant<Types...>;

template<typename T>
using Any = std::any;

using String = std::string;
using StringView = std::string_view;
using Path = std::filesystem::path;

using Mutex = std::mutex;
using RecursiveMutex = std::recursive_mutex;
using SharedMutex = std::shared_mutex;
using TimedMutex = std::timed_mutex;
using RecursiveTimedMutex = std::recursive_timed_mutex;

template<typename T>
using LockGuard = std::lock_guard<T>;

template<typename T>
using UniqueLock = std::unique_lock<T>;

template<typename T>
using SharedLock = std::shared_lock<T>;

using ConditionVariable = std::condition_variable;
using ConditionVariableAny = std::condition_variable_any;

using Thread = std::thread;
using JThread = std::jthread;

template<typename T>
using Future = std::future<T>;

template<typename T>
using SharedFuture = std::shared_future<T>;

template<typename T>
using Promise = std::promise<T>;

template<typename T>
using PackagedTask = std::packaged_task<T>;

using Exception = std::exception;
using RuntimeError = std::runtime_error;
using LogicError = std::logic_error;
using InvalidArgument = std::invalid_argument;
using OutOfRange = std::out_of_range;
using LengthError = std::length_error;
using BadAlloc = std::bad_alloc;
using BadCast = std::bad_cast;
using BadTypeId = std::bad_typeid;
using BadWeakPtr = std::bad_weak_ptr;
using BadFunctionCall = std::bad_function_call;
using BadException = std::bad_exception;
using BadVariantAccess = std::bad_variant_access;
using BadAnyCast = std::bad_any_cast;
using BadOptionalAccess = std::bad_optional_access;

using SystemError = std::system_error;
using ErrorCode = std::error_code;
using ErrorCondition = std::error_condition;

using Duration = std::chrono::duration<f64>;
using TimePoint = std::chrono::time_point<std::chrono::high_resolution_clock>;
using Nanoseconds = std::chrono::nanoseconds;
using Microseconds = std::chrono::microseconds;
using Milliseconds = std::chrono::milliseconds;
using Seconds = std::chrono::seconds;
using Minutes = std::chrono::minutes;
using Hours = std::chrono::hours;
using Days = std::chrono::days;
using Weeks = std::chrono::weeks;
using Months = std::chrono::months;
using Years = std::chrono::years;

using HighResolutionClock = std::chrono::high_resolution_clock;
using SteadyClock = std::chrono::steady_clock;
using SystemClock = std::chrono::system_clock;

using RandomDevice = std::random_device;
using DefaultRandomEngine = std::default_random_engine;
using Mt19937 = std::mt19937;
using Mt19937_64 = std::mt19937_64;

template<typename T>
using UniformIntDistribution = std::uniform_int_distribution<T>;

template<typename T>
using UniformRealDistribution = std::uniform_real_distribution<T>;

template<typename T>
using NormalDistribution = std::normal_distribution<T>;

template<typename T>
using ExponentialDistribution = std::exponential_distribution<T>;

template<typename T>
using GammaDistribution = std::gamma_distribution<T>;

template<typename T>
using WeibullDistribution = std::weibaud_distribution<T>;

template<typename T>
using ExtremeValueDistribution = std::extreme_value_distribution<T>;

template<typename T>
using ChiSquaredDistribution = std::chi_squared_distribution<T>;

template<typename T>
using CauchyDistribution = std::cauchy_distribution<T>;

template<typename T>
using FisherFDistribution = std::fisher_f_distribution<T>;

template<typename T>
using StudentTDistribution = std::student_t_distribution<T>;

template<typename T>
using DiscreteDistribution = std::discrete_distribution<T>;

template<typename T>
using PiecewiseConstantDistribution = std::piecewise_constant_distribution<T>;

template<typename T>
using PiecewiseLinearDistribution = std::piecewise_linear_distribution<T>;

using Regex = std::regex;
using CRegex = std::cregex_iterator;
using SRegex = std::sregex_iterator;
using RegexMatch = std::smatch;
using RegexToken = std::sregex_token_iterator;

using CoroutineHandle = std::coroutine_handle<>;
using SuspendAlways = std::suspend_always;
using SuspendNever = std::suspend_never;

struct Point {
    f32 x, y;
    Point() : x(0), y(0) {}
    Point(f32 x, f32 y) : x(x), y(y) {}
    Point operator+(const Point& other) const { return {x + other.x, y + other.y}; }
    Point operator-(const Point& other) const { return {x - other.x, y - other.y}; }
    Point operator*(f32 scalar) const { return {x * scalar, y * scalar}; }
    Point operator/(f32 scalar) const { return {x / scalar, y / scalar}; }
    Point& operator+=(const Point& other) { x += other.x; y += other.y; return *this; }
    Point& operator-=(const Point& other) { x -= other.x; y -= other.y; return *this; }
    Point& operator*=(f32 scalar) { x *= scalar; y *= scalar; return *this; }
    Point& operator/=(f32 scalar) { x /= scalar; y /= scalar; return *this; }
    f32 distance(const Point& other) const { f32 dx = x - other.x; f32 dy = y - other.y; return std::sqrt(dx * dx + dy * dy); }
    f32 distance_squared(const Point& other) const { f32 dx = x - other.x; f32 dy = y - other.y; return dx * dx + dy * dy; }
    f32 length() const { return std::sqrt(x * x + y * y); }
    f32 length_squared() const { return x * x + y * y; }
    Point normalized() const { f32 len = length(); return len > 0 ? *this / len : Point(0, 0); }
    f32 dot(const Point& other) const { return x * other.x + y * other.y; }
    f32 cross(const Point& other) const { return x * other.y - y * other.x; }
    Point rotated(f32 angle) const { f32 cos_a = std::cos(angle); f32 sin_a = std::sin(angle); return {x * cos_a - y * sin_a, x * sin_a + y * cos_a}; }
    Point lerp(const Point& other, f32 t) const { return *this + (other - *this) * t; }
};

struct Size {
    f32 width, height;
    Size() : width(0), height(0) {}
    Size(f32 width, f32 height) : width(width), height(height) {}
    Size operator+(const Size& other) const { return {width + other.width, height + other.height}; }
    Size operator-(const Size& other) const { return {width - other.width, height - other.height}; }
    Size operator*(f32 scalar) const { return {width * scalar, height * scalar}; }
    Size operator/(f32 scalar) const { return {width / scalar, height / scalar}; }
    Size& operator+=(const Size& other) { width += other.width; height += other.height; return *this; }
    Size& operator-=(const Size& other) { width -= other.width; height -= other.height; return *this; }
    Size& operator*=(f32 scalar) { width *= scalar; height *= scalar; return *this; }
    Size& operator/=(f32 scalar) { width /= scalar; height /= scalar; return *this; }
    f32 area() const { return width * height; }
    f32 aspect_ratio() const { return height > 0 ? width / height : 0; }
    bool contains(const Point& point) const { return point.x >= 0 && point.x < width && point.y >= 0 && point.y < height; }
    Size expanded(f32 amount) const { return {width + amount * 2, height + amount * 2}; }
    Size shrunk(f32 amount) const { return {std::max(0.0f, width - amount * 2), std::max(0.0f, height - amount * 2)}; }
};

struct Rect {
    Point position;
    Size size;
    Rect() : position(), size() {}
    Rect(const Point& position, const Size& size) : position(position), size(size) {}
    Rect(f32 x, f32 y, f32 width, f32 height) : position(x, y), size(width, height) {}
    f32 left() const { return position.x; }
    f32 right() const { return position.x + size.width; }
    f32 top() const { return position.y; }
    f32 bottom() const { return position.y + size.height; }
    f32 center_x() const { return position.x + size.width * 0.5f; }
    f32 center_y() const { return position.y + size.height * 0.5f; }
    Point center() const { return {center_x(), center_y()}; }
    Point top_left() const { return position; }
    Point top_right() const { return {right(), top()}; }
    Point bottom_left() const { return {left(), bottom()}; }
    Point bottom_right() const { return {right(), bottom()}; }
    bool contains(const Point& point) const { return size.contains(point - position); }
    bool contains(const Rect& other) const { return contains(other.top_left()) && contains(other.bottom_right()); }
    bool intersects(const Rect& other) const { return !(right() < other.left() || left() > other.right() || bottom() < other.top() || top() > other.bottom()); }
    Rect intersection(const Rect& other) const { f32 x1 = std::max(left(), other.left()); f32 y1 = std::max(top(), other.top()); f32 x2 = std::min(right(), other.right()); f32 y2 = std::min(bottom(), other.bottom()); return {x1, y1, std::max(0.0f, x2 - x1), std::max(0.0f, y2 - y1)}; }
    Rect union_rect(const Rect& other) const { f32 x1 = std::min(left(), other.left()); f32 y1 = std::min(top(), other.top()); f32 x2 = std::max(right(), other.right()); f32 y2 = std::max(bottom(), other.bottom()); return {x1, y1, x2 - x1, y2 - y1}; }
    Rect expanded(f32 amount) const { return {position - Point(amount, amount), size.expanded(amount)}; }
    Rect shrunk(f32 amount) const { return {position + Point(amount, amount), size.shrunk(amount)}; }
    Rect moved(const Point& delta) const { return {position + delta, size}; }
    Rect resized(const Size& new_size) const { return {position, new_size}; }
    Rect resized(f32 width, f32 height) const { return {position, {width, height}}; }
    f32 area() const { return size.area(); }
    bool is_empty() const { return size.width <= 0 || size.height <= 0; }
    bool is_valid() const { return size.width >= 0 && size.height >= 0; }
};

struct Color {
    f32 r, g, b, a;
    Color() : r(0), g(0), b(0), a(1) {}
    Color(f32 r, f32 g, f32 b, f32 a = 1) : r(r), g(g), b(b), a(a) {}
    Color(u32 rgba) : r(((rgba >> 24) & 0xFF) / 255.0f), g(((rgba >> 16) & 0xFF) / 255.0f), b(((rgba >> 8) & 0xFF) / 255.0f), a((rgba & 0xFF) / 255.0f) {}
    Color(u32 rgb, f32 a) : r(((rgb >> 16) & 0xFF) / 255.0f), g(((rgb >> 8) & 0xFF) / 255.0f), b((rgb & 0xFF) / 255.0f), a(a) {}
    u32 to_rgba() const { return ((u32)(r * 255) << 24) | ((u32)(g * 255) << 16) | ((u32)(b * 255) << 8) | (u32)(a * 255); }
    u32 to_rgb() const { return ((u32)(r * 255) << 16) | ((u32)(g * 255) << 8) | (u32)(b * 255); }
    Color operator+(const Color& other) const { return {r + other.r, g + other.g, b + other.b, a + other.a}; }
    Color operator-(const Color& other) const { return {r - other.r, g - other.g, b - other.b, a - other.a}; }
    Color operator*(f32 scalar) const { return {r * scalar, g * scalar, b * scalar, a * scalar}; }
    Color operator*(const Color& other) const { return {r * other.r, g * other.g, b * other.b, a * other.a}; }
    Color operator/(f32 scalar) const { return {r / scalar, g / scalar, b / scalar, a / scalar}; }
    Color& operator+=(const Color& other) { r += other.r; g += other.g; b += other.b; a += other.a; return *this; }
    Color& operator-=(const Color& other) { r -= other.r; g -= other.g; b -= other.b; a -= other.a; return *this; }
    Color& operator*=(f32 scalar) { r *= scalar; g *= scalar; b *= scalar; a *= scalar; return *this; }
    Color& operator*=(const Color& other) { r *= other.r; g *= other.g; b *= other.b; a *= other.a; return *this; }
    Color& operator/=(f32 scalar) { r /= scalar; g /= scalar; b /= scalar; a /= scalar; return *this; }
    Color with_alpha(f32 new_alpha) const { return {r, g, b, new_alpha}; }
    Color lerp(const Color& other, f32 t) const { return *this + (other - *this) * t; }
    Color inverted() const { return {1 - r, 1 - g, 1 - b, a}; }
    Color grayscale() const { f32 gray = r * 0.299f + g * 0.587f + b * 0.114f; return {gray, gray, gray, a}; }
    Color sepia() const { f32 tr = r * 0.393f + g * 0.769f + b * 0.189f; f32 tg = r * 0.349f + g * 0.686f + b * 0.168f; f32 tb = r * 0.272f + g * 0.534f + b * 0.131f; return {std::min(tr, 1.0f), std::min(tg, 1.0f), std::min(tb, 1.0f), a}; }
    static Color transparent() { return {0, 0, 0, 0}; }
    static Color black() { return {0, 0, 0, 1}; }
    static Color white() { return {1, 1, 1, 1}; }
    static Color red() { return {1, 0, 0, 1}; }
    static Color green() { return {0, 1, 0, 1}; }
    static Color blue() { return {0, 0, 1, 1}; }
    static Color yellow() { return {1, 1, 0, 1}; }
    static Color magenta() { return {1, 0, 1, 1}; }
    static Color cyan() { return {0, 1, 1, 1}; }
    static Color gray(f32 brightness) { return {brightness, brightness, brightness, 1}; }
};

struct Matrix4x4 {
    f32 m[16];
    Matrix4x4() { std::memset(m, 0, sizeof(m)); m[0] = m[5] = m[10] = m[15] = 1; }
    Matrix4x4(f32 m00, f32 m01, f32 m02, f32 m03, f32 m10, f32 m11, f32 m12, f32 m13, f32 m20, f32 m21, f32 m22, f32 m23, f32 m30, f32 m31, f32 m32, f32 m33) {
        m[0] = m00; m[1] = m01; m[2] = m02; m[3] = m03;
        m[4] = m10; m[5] = m11; m[6] = m12; m[7] = m13;
        m[8] = m20; m[9] = m21; m[10] = m22; m[11] = m23;
        m[12] = m30; m[13] = m31; m[14] = m32; m[15] = m33;
    }
    static Matrix4x4 identity() { return {}; }
    static Matrix4x4 translation(f32 x, f32 y, f32 z) { Matrix4x4 mat; mat.m[12] = x; mat.m[13] = y; mat.m[14] = z; return mat; }
    static Matrix4x4 translation(const Point& p) { return translation(p.x, p.y, 0); }
    static Matrix4x4 scale(f32 x, f32 y, f32 z) { Matrix4x4 mat; mat.m[0] = x; mat.m[5] = y; mat.m[10] = z; return mat; }
    static Matrix4x4 scale(f32 s) { return scale(s, s, s); }
    static Matrix4x4 rotation_x(f32 angle) { f32 c = std::cos(angle); f32 s = std::sin(angle); Matrix4x4 mat; mat.m[5] = c; mat.m[6] = -s; mat.m[9] = s; mat.m[10] = c; return mat; }
    static Matrix4x4 rotation_y(f32 angle) { f32 c = std::cos(angle); f32 s = std::sin(angle); Matrix4x4 mat; mat.m[0] = c; mat.m[2] = s; mat.m[8] = -s; mat.m[10] = c; return mat; }
    static Matrix4x4 rotation_z(f32 angle) { f32 c = std::cos(angle); f32 s = std::sin(angle); Matrix4x4 mat; mat.m[0] = c; mat.m[1] = -s; mat.m[4] = s; mat.m[5] = c; return mat; }
    static Matrix4x4 rotation(f32 angle, f32 x, f32 y, f32 z) { f32 c = std::cos(angle); f32 s = std::sin(angle); f32 omc = 1 - c; Matrix4x4 mat; mat.m[0] = x*x*omc + c; mat.m[1] = y*x*omc + z*s; mat.m[2] = z*x*omc - y*s; mat.m[4] = x*y*omc - z*s; mat.m[5] = y*y*omc + c; mat.m[6] = z*y*omc + x*s; mat.m[8] = x*z*omc + y*s; mat.m[9] = y*z*omc - x*s; mat.m[10] = z*z*omc + c; return mat; }
    static Matrix4x4 ortho(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) { Matrix4x4 mat; mat.m[0] = 2/(right-left); mat.m[5] = 2/(top-bottom); mat.m[10] = -2/(far-near); mat.m[12] = -(right+left)/(right-left); mat.m[13] = -(top+bottom)/(top-bottom); mat.m[14] = -(far+near)/(far-near); return mat; }
    static Matrix4x4 perspective(f32 fov, f32 aspect, f32 near, f32 far) { f32 f = 1/std::tan(fov/2); f32 range = 1/(near-far); Matrix4x4 mat; mat.m[0] = f/aspect; mat.m[5] = f; mat.m[10] = (far+near)*range; mat.m[11] = -1; mat.m[14] = 2*far*near*range; mat.m[15] = 0; return mat; }
    Matrix4x4 operator*(const Matrix4x4& other) const { Matrix4x4 result; for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) for(int k = 0; k < 4; k++) result.m[i*4+j] += m[i*4+k] * other.m[k*4+j]; return result; }
    Point operator*(const Point& p) const { f32 x = m[0]*p.x + m[4]*p.y + m[12]; f32 y = m[1]*p.x + m[5]*p.y + m[13]; f32 w = m[3]*p.x + m[7]*p.y + m[15]; if(w != 0) { x /= w; y /= w; } return {x, y}; }
    Matrix4x4 transposed() const { Matrix4x4 result; for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++) result.m[i*4+j] = m[j*4+i]; return result; }
    f32 determinant() const { return m[0]*(m[5]*(m[10]*m[15]-m[11]*m[14])-m[6]*(m[9]*m[15]-m[11]*m[13])+m[7]*(m[9]*m[14]-m[10]*m[13])) - m[1]*(m[4]*(m[10]*m[15]-m[11]*m[14])-m[6]*(m[8]*m[15]-m[11]*m[12])+m[7]*(m[8]*m[14]-m[10]*m[12])) + m[2]*(m[4]*(m[9]*m[15]-m[11]*m[13])-m[5]*(m[8]*m[15]-m[11]*m[12])+m[7]*(m[8]*m[13]-m[9]*m[12])) - m[3]*(m[4]*(m[9]*m[14]-m[10]*m[13])-m[5]*(m[8]*m[14]-m[10]*m[12])+m[6]*(m[8]*m[13]-m[9]*m[12])); }
    Matrix4x4 inverted() const { f32 det = determinant(); if(det == 0) return {}; Matrix4x4 result; result.m[0] = (m[5]*(m[10]*m[15]-m[11]*m[14])-m[6]*(m[9]*m[15]-m[11]*m[13])+m[7]*(m[9]*m[14]-m[10]*m[13]))/det; result.m[1] = -(m[1]*(m[10]*m[15]-m[11]*m[14])-m[2]*(m[9]*m[15]-m[11]*m[13])+m[3]*(m[9]*m[14]-m[10]*m[13]))/det; result.m[2] = (m[1]*(m[6]*m[15]-m[7]*m[14])-m[2]*(m[5]*m[15]-m[7]*m[13])+m[3]*(m[5]*m[14]-m[6]*m[13]))/det; result.m[3] = -(m[1]*(m[6]*m[11]-m[7]*m[10])-m[2]*(m[5]*m[11]-m[7]*m[9])+m[3]*(m[5]*m[10]-m[6]*m[9]))/det; result.m[4] = -(m[4]*(m[10]*m[15]-m[11]*m[14])-m[6]*(m[8]*m[15]-m[11]*m[12])+m[7]*(m[8]*m[14]-m[10]*m[12]))/det; result.m[5] = (m[0]*(m[10]*m[15]-m[11]*m[14])-m[2]*(m[8]*m[15]-m[11]*m[12])+m[3]*(m[8]*m[14]-m[10]*m[12]))/det; result.m[6] = -(m[0]*(m[6]*m[15]-m[7]*m[14])-m[2]*(m[4]*m[15]-m[7]*m[12])+m[3]*(m[4]*m[14]-m[6]*m[12]))/det; result.m[7] = (m[0]*(m[6]*m[11]-m[7]*m[10])-m[2]*(m[4]*m[11]-m[7]*m[8])+m[3]*(m[4]*m[10]-m[6]*m[8]))/det; result.m[8] = (m[4]*(m[9]*m[15]-m[11]*m[13])-m[5]*(m[8]*m[15]-m[11]*m[12])+m[7]*(m[8]*m[13]-m[9]*m[12]))/det; result.m[9] = -(m[0]*(m[9]*m[15]-m[11]*m[13])-m[1]*(m[8]*m[15]-m[11]*m[12])+m[3]*(m[8]*m[13]-m[9]*m[12]))/det; result.m[10] = (m[0]*(m[5]*m[15]-m[7]*m[13])-m[1]*(m[4]*m[15]-m[7]*m[12])+m[3]*(m[4]*m[13]-m[5]*m[12]))/det; result.m[11] = -(m[0]*(m[5]*m[11]-m[7]*m[9])-m[1]*(m[4]*m[11]-m[7]*m[8])+m[3]*(m[4]*m[9]-m[5]*m[8]))/det; result.m[12] = -(m[4]*(m[9]*m[14]-m[10]*m[13])-m[5]*(m[8]*m[14]-m[10]*m[12])+m[6]*(m[8]*m[13]-m[9]*m[12]))/det; result.m[13] = (m[0]*(m[9]*m[14]-m[10]*m[13])-m[1]*(m[8]*m[14]-m[10]*m[12])+m[2]*(m[8]*m[13]-m[9]*m[12]))/det; result.m[14] = -(m[0]*(m[5]*m[14]-m[6]*m[13])-m[1]*(m[4]*m[14]-m[6]*m[12])+m[2]*(m[4]*m[13]-m[5]*m[12]))/det; result.m[15] = (m[0]*(m[5]*m[10]-m[6]*m[9])-m[1]*(m[4]*m[10]-m[6]*m[8])+m[2]*(m[4]*m[9]-m[5]*m[8]))/det; return result; }
};

enum class EventType : u32 {
    None = 0,
    KeyPress = 1,
    KeyRelease = 2,
    KeyRepeat = 3,
    ButtonPress = 4,
    ButtonRelease = 5,
    ButtonRepeat = 6,
    Motion = 7,
    MotionAbsolute = 8,
    Scroll = 9,
    ScrollAbsolute = 10,
    Touch = 11,
    TouchBegin = 12,
    TouchUpdate = 13,
    TouchEnd = 14,
    TouchCancel = 15,
    Gesture = 16,
    GestureBegin = 17,
    GestureUpdate = 18,
    GestureEnd = 19,
    GestureCancel = 20,
    Proximity = 21,
    ProximityIn = 22,
    ProximityOut = 23,
    Switch = 24,
    SwitchToggle = 25,
    Tablet = 26,
    TabletProximity = 27,
    TabletPressure = 28,
    TabletDistance = 29,
    TabletTilt = 30,
    TabletRotation = 31,
    TabletSlider = 32,
    TabletWheel = 33,
    TabletTool = 34,
    TabletButton = 35,
    WindowCreate = 36,
    WindowDestroy = 37,
    WindowShow = 38,
    WindowHide = 39,
    WindowFocus = 40,
    WindowUnfocus = 41,
    WindowMap = 42,
    WindowUnmap = 43,
    WindowConfigure = 44,
    WindowProperty = 45,
    WindowClientMessage = 46,
    WindowColormap = 47,
    WindowGravity = 48,
    DisplayConnect = 49,
    DisplayDisconnect = 50,
    DisplayMode = 51,
    DisplayGamma = 52,
    DisplayBrightness = 53,
    DisplayContrast = 54,
    DisplaySaturation = 55,
    DisplayHue = 56,
    DisplayTemperature = 57,
    DisplayOrientation = 58,
    DisplayReflection = 59,
    DisplayMirror = 60,
    Timer = 61,
    Idle = 62,
    Wakeup = 63,
    Sleep = 64,
    Power = 65,
    Battery = 66,
    Thermal = 67,
    Network = 68,
    Bluetooth = 69,
    USB = 70,
    Audio = 71,
    Video = 72,
    Camera = 73,
    Sensor = 74,
    GPS = 75,
    NFC = 76,
    RFID = 77,
    Barcode = 78,
    Fingerprint = 79,
    Face = 80,
    Iris = 81,
    Voice = 82,
    Gesture3D = 83,
    Haptic = 84,
    Force = 85,
    Pressure = 86,
    Temperature = 87,
    Humidity = 88,
    Light = 89,
    Color = 90,
    Proximity2 = 91,
    Acceleration = 92,
    Gyroscope = 93,
    Magnetometer = 94,
    Barometer = 95,
    Altimeter = 96,
    Compass = 97,
    Pedometer = 98,
    Step = 99,
    Activity = 100,
    HeartRate = 101,
    BloodPressure = 102,
    Glucose = 103,
    Oxygen = 104,
    ECG = 105,
    EMG = 106,
    EEG = 107,
    EOG = 108,
    GSR = 109,
    Respiration = 110,
    BodyTemperature = 111,
    SkinTemperature = 112,
    AmbientTemperature = 113,
    UV = 114,
    Radiation = 115,
    Chemical = 116,
    Biological = 117,
    Genetic = 118,
    Neural = 119,
    Quantum = 120,
    Relativistic = 121,
    Subatomic = 122,
    Nanoscopic = 123,
    Microscopic = 124,
    Macroscopic = 125,
    Cosmic = 126,
    Universal = 127,
    Multiversal = 128,
    Omniversal = 129,
    Custom = 130
};

struct Event {
    EventType type;
    u64 timestamp;
    u32 device_id;
    u32 window_id;
    u32 key_code;
    u32 button_code;
    f32 x;
    f32 y;
    f32 z;
    f32 pressure;
    f32 distance;
    f32 tilt_x;
    f32 tilt_y;
    f32 rotation_x;
    f32 rotation_y;
    f32 rotation_z;
    f32 acceleration_x;
    f32 acceleration_y;
    f32 acceleration_z;
    f32 velocity_x;
    f32 velocity_y;
    f32 velocity_z;
    f32 size_x;
    f32 size_y;
    f32 shape_x;
    f32 shape_y;
    f32 color_r;
    f32 color_g;
    f32 color_b;
    f32 color_a;
    f32 temperature;
    f32 humidity;
    f32 light;
    f32 sound;
    f32 vibration;
    f32 magnetic_x;
    f32 magnetic_y;
    f32 magnetic_z;
    f32 electric_x;
    f32 electric_y;
    f32 electric_z;
    f32 gravitational_x;
    f32 gravitational_y;
    f32 gravitational_z;
    f32 nuclear_energy;
    f32 chemical_concentration;
    f32 biological_activity;
    f32 genetic_expression;
    f32 neural_firing;
    f32 quantum_state;
    f32 relativistic_speed;
    f32 subatomic_particles;
    f32 nanoscopic_scale;
    f32 microscopic_resolution;
    f32 macroscopic_view;
    f32 cosmic_radiation;
    f32 universal_constant;
    f32 multiversal_branch;
    f32 omniversal_potential;
    u32 data[16];
    void* user_data;
    Event() : type(EventType::None), timestamp(0), device_id(0), window_id(0), key_code(0), button_code(0), x(0), y(0), z(0), pressure(0), distance(0), tilt_x(0), tilt_y(0), rotation_x(0), rotation_y(0), rotation_z(0), acceleration_x(0), acceleration_y(0), acceleration_z(0), velocity_x(0), velocity_y(0), velocity_z(0), size_x(0), size_y(0), shape_x(0), shape_y(0), color_r(0), color_g(0), color_b(0), color_a(0), temperature(0), humidity(0), light(0), sound(0), vibration(0), magnetic_x(0), magnetic_y(0), magnetic_z(0), electric_x(0), electric_y(0), electric_z(0), gravitational_x(0), gravitational_y(0), gravitational_z(0), nuclear_energy(0), chemical_concentration(0), biological_activity(0), genetic_expression(0), neural_firing(0), quantum_state(0), relativistic_speed(0), subatomic_particles(0), nanoscopic_scale(0), microscopic_resolution(0), macroscopic_view(0), cosmic_radiation(0), universal_constant(0), multiversal_branch(0), omniversal_potential(0), user_data(nullptr) { std::memset(data, 0, sizeof(data)); }
};

template<typename T>
class Singleton {
public:
    static T& instance() {
        static T instance;
        return instance;
    }
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
protected:
    Singleton() = default;
    ~Singleton() = default;
};

class Timer {
public:
    Timer() : start_time_(HighResolutionClock::now()) {}
    void reset() { start_time_ = HighResolutionClock::now(); }
    f64 elapsed() const { return std::chrono::duration_cast<std::chrono::duration<f64>>(HighResolutionClock::now() - start_time_).count(); }
    f64 elapsed_ms() const { return elapsed() * 1000.0; }
    f64 elapsed_us() const { return elapsed() * 1000000.0; }
    f64 elapsed_ns() const { return elapsed() * 1000000000.0; }
private:
    TimePoint start_time_;
};

class Profiler {
public:
    Profiler(const String& name) : name_(name), timer_() {}
    ~Profiler() { std::cout << name_ << ": " << timer_.elapsed_ms() << "ms\n"; }
private:
    String name_;
    Timer timer_;
};

class Random {
public:
    Random() : engine_(std::random_device{}()) {}
    Random(u32 seed) : engine_(seed) {}
    i32 next_int(i32 min = 0, i32 max = std::numeric_limits<i32>::max()) { UniformIntDistribution<i32> dist(min, max); return dist(engine_); }
    f32 next_float(f32 min = 0.0f, f32 max = 1.0f) { UniformRealDistribution<f32> dist(min, max); return dist(engine_); }
    f64 next_double(f64 min = 0.0, f64 max = 1.0) { UniformRealDistribution<f64> dist(min, max); return dist(engine_); }
    f32 next_gaussian(f32 mean = 0.0f, f32 stddev = 1.0f) { NormalDistribution<f32> dist(mean, stddev); return dist(engine_); }
    bool next_bool(f32 probability = 0.5f) { return next_float() < probability; }
    Point next_point(const Rect& bounds) { return {next_float(bounds.left(), bounds.right()), next_float(bounds.top(), bounds.bottom())}; }
    Color next_color() { return {next_float(), next_float(), next_float(), 1.0f}; }
    Size next_size(f32 min_width = 0, f32 max_width = 100, f32 min_height = 0, f32 max_height = 100) { return {next_float(min_width, max_width), next_float(min_height, max_height)}; }
    void seed(u32 seed) { engine_.seed(seed); }
private:
    DefaultRandomEngine engine_;
};

class Logger {
public:
    enum class Level { Debug, Info, Warning, Error, Fatal };
    static void log(Level level, const String& message) {
        static const char* level_names[] = {"DEBUG", "INFO", "WARN", "ERROR", "FATAL"};
        std::cout << "[" << level_names[static_cast<int>(level)] << "] " << message << std::endl;
    }
    static void debug(const String& message) { log(Level::Debug, message); }
    static void info(const String& message) { log(Level::Info, message); }
    static void warning(const String& message) { log(Level::Warning, message); }
    static void error(const String& message) { log(Level::Error, message); }
    static void fatal(const String& message) { log(Level::Fatal, message); }
};

template<typename T>
class ObjectPool {
public:
    ObjectPool(usize size = 1024) { objects_.reserve(size); }
    ~ObjectPool() { clear(); }
    T* acquire() {
        if (objects_.empty()) {
            return new T();
        } else {
            T* obj = objects_.back();
            objects_.pop_back();
            return obj;
        }
    }
    void release(T* obj) {
        if (obj) {
            obj->~T();
            new (obj) T();
            objects_.push_back(obj);
        }
    }
    void clear() {
        for (T* obj : objects_) {
            delete obj;
        }
        objects_.clear();
    }
    usize size() const { return objects_.size(); }
private:
    Vector<T*> objects_;
};

template<typename T>
class RingBuffer {
public:
    RingBuffer(usize capacity) : buffer_(capacity), head_(0), tail_(0), size_(0) {}
    bool push(const T& value) {
        if (size_ == buffer_.size()) return false;
        buffer_[tail_] = value;
        tail_ = (tail_ + 1) % buffer_.size();
        ++size_;
        return true;
    }
    bool pop(T& value) {
        if (size_ == 0) return false;
        value = buffer_[head_];
        head_ = (head_ + 1) % buffer_.size();
        --size_;
        return true;
    }
    bool peek(T& value) const {
        if (size_ == 0) return false;
        value = buffer_[head_];
        return true;
    }
    void clear() { head_ = tail_ = size_ = 0; }
    usize size() const { return size_; }
    usize capacity() const { return buffer_.size(); }
    bool empty() const { return size_ == 0; }
    bool full() const { return size_ == buffer_.size(); }
private:
    Vector<T> buffer_;
    usize head_;
    usize tail_;
    usize size_;
};

template<typename T>
class LRUCache {
public:
    LRUCache(usize capacity) : capacity_(capacity) {}
    Optional<T> get(const String& key) {
        auto it = cache_.find(key);
        if (it == cache_.end()) return {};
        order_.splice(order_.begin(), order_, it->second.second);
        return it->second.first;
    }
    void put(const String& key, const T& value) {
        auto it = cache_.find(key);
        if (it != cache_.end()) {
            it->second.first = value;
            order_.splice(order_.begin(), order_, it->second.second);
        } else {
            if (cache_.size() == capacity_) {
                auto last = order_.back();
                cache_.erase(last);
                order_.pop_back();
            }
            order_.push_front(key);
            cache_[key] = {value, order_.begin()};
        }
    }
    void clear() { cache_.clear(); order_.clear(); }
    usize size() const { return cache_.size(); }
    usize capacity() const { return capacity_; }
    bool empty() const { return cache_.empty(); }
private:
    usize capacity_;
    Map<String, Pair<T, typename List<String>::iterator>> cache_;
    List<String> order_;
};

u64 get_timestamp();
void set_thread_priority(int priority);
void set_process_priority(int priority);
void set_cpu_affinity(int cpu_id);
void lock_memory();
void unlock_memory();
void prefault_memory(usize size);
void set_realtime_scheduling();
void disable_cpu_scaling();
void enable_cpu_scaling();
void set_cpu_frequency(int cpu_id, int frequency_khz);
void set_gpu_frequency(int gpu_id, int frequency_khz);
void set_memory_frequency(int memory_id, int frequency_khz);
void set_network_affinity(int interface_id, int cpu_id);
void set_disk_affinity(int disk_id, int cpu_id);
void set_gpu_affinity(int gpu_id, int cpu_id);
void set_memory_affinity(int memory_id, int cpu_id);
void set_cache_affinity(int cache_id, int cpu_id);
void set_scheduler_affinity(int scheduler_id, int cpu_id);
void set_timer_affinity(int timer_id, int cpu_id);
void set_clock_affinity(int clock_id, int cpu_id);
void set_power_affinity(int power_id, int cpu_id);
void set_thermal_affinity(int thermal_id, int cpu_id);
void set_frequency_affinity(int frequency_id, int cpu_id);
void set_voltage_affinity(int voltage_id, int cpu_id);
void set_current_affinity(int current_id, int cpu_id);

String to_string(const Point& p);
String to_string(const Size& s);
String to_string(const Rect& r);
String to_string(const Color& c);
String to_string(EventType type);
String to_string(const Event& e);

Point from_string_point(const String& str);
Size from_string_size(const String& str);
Rect from_string_rect(const String& str);
Color from_string_color(const String& str);

template<typename T>
T clamp(T value, T min, T max) {
    return value < min ? min : (value > max ? max : value);
}

template<typename T>
T lerp(T a, T b, f32 t) {
    return a + (b - a) * t;
}

template<typename T>
T map(T value, T from_min, T from_max, T to_min, T to_max) {
    return to_min + (value - from_min) * (to_max - to_min) / (from_max - from_min);
}

template<typename T>
T smoothstep(T edge0, T edge1, T x) {
    T t = clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
    return t * t * (T(3) - T(2) * t);
}

template<typename T>
T smootherstep(T edge0, T edge1, T x) {
    T t = clamp((x - edge0) / (edge1 - edge0), T(0), T(1));
    return t * t * t * (t * (t * T(6) - T(15)) + T(10));
}

f32 ease_in_quad(f32 t);
f32 ease_out_quad(f32 t);
f32 ease_in_out_quad(f32 t);
f32 ease_in_cubic(f32 t);
f32 ease_out_cubic(f32 t);
f32 ease_in_out_cubic(f32 t);
f32 ease_in_quart(f32 t);
f32 ease_out_quart(f32 t);
f32 ease_in_out_quart(f32 t);
f32 ease_in_quint(f32 t);
f32 ease_out_quint(f32 t);
f32 ease_in_out_quint(f32 t);
f32 ease_in_sine(f32 t);
f32 ease_out_sine(f32 t);
f32 ease_in_out_sine(f32 t);
f32 ease_in_expo(f32 t);
f32 ease_out_expo(f32 t);
f32 ease_in_out_expo(f32 t);
f32 ease_in_circ(f32 t);
f32 ease_out_circ(f32 t);
f32 ease_in_out_circ(f32 t);
f32 ease_in_elastic(f32 t);
f32 ease_out_elastic(f32 t);
f32 ease_in_out_elastic(f32 t);
f32 ease_in_back(f32 t);
f32 ease_out_back(f32 t);
f32 ease_in_out_back(f32 t);
f32 ease_in_bounce(f32 t);
f32 ease_out_bounce(f32 t);
f32 ease_in_out_bounce(f32 t);

f32 noise1d(f32 x);
f32 noise2d(f32 x, f32 y);
f32 noise3d(f32 x, f32 y, f32 z);
f32 noise1d_octave(f32 x, i32 octaves, f32 persistence);
f32 noise2d_octave(f32 x, f32 y, i32 octaves, f32 persistence);
f32 noise3d_octave(f32 x, f32 y, f32 z, i32 octaves, f32 persistence);

u32 hash32(u32 x);
u64 hash64(u64 x);
u32 hash_string(const String& str);
u64 hash_string64(const String& str);

template<typename T>
usize hash_combine(usize seed, const T& value);

template<typename... Args>
usize hash_combine_all(const Args&... args);

String format_string(const char* format, ...);
String vformat_string(const char* format, va_list args);

String to_lower(const String& str);
String to_upper(const String& str);
String trim_left(const String& str);
String trim_right(const String& str);
String trim(const String& str);
Vector<String> split(const String& str, char delimiter);
Vector<String> split(const String& str, const String& delimiter);
String join(const Vector<String>& strings, const String& delimiter);
bool starts_with(const String& str, const String& prefix);
bool ends_with(const String& str, const String& suffix);
bool contains(const String& str, const String& substr);
String replace_all(const String& str, const String& from, const String& to);

Path get_home_directory();
Path get_config_directory();
Path get_cache_directory();
Path get_temp_directory();
Path get_data_directory();
bool create_directory(const Path& path);
bool remove_directory(const Path& path);
bool directory_exists(const Path& path);
bool file_exists(const Path& path);
u64 get_file_size(const Path& path);
u64 get_file_modified_time(const Path& path);
Vector<Path> list_directory(const Path& path);
bool read_file(const Path& path, String& content);
bool write_file(const Path& path, const String& content);
bool append_file(const Path& path, const String& content);
bool copy_file(const Path& from, const Path& to);
bool move_file(const Path& from, const Path& to);
bool remove_file(const Path& path);

class FileWatcher {
public:
    FileWatcher(const Path& path, Function<void(const Path&)> callback);
    ~FileWatcher();
    void start();
    void stop();
    bool is_running() const;
private:
    Path path_;
    Function<void(const Path&)> callback_;
    Atomic<bool> running_;
    Thread thread_;
    void watch_loop();
};

class ConfigFile {
public:
    ConfigFile(const Path& path);
    ~ConfigFile();
    bool load();
    bool save();
    bool has_key(const String& key) const;
    String get_string(const String& key, const String& default_value = "") const;
    i32 get_int(const String& key, i32 default_value = 0) const;
    f32 get_float(const String& key, f32 default_value = 0.0f) const;
    bool get_bool(const String& key, bool default_value = false) const;
    void set_string(const String& key, const String& value);
    void set_int(const String& key, i32 value);
    void set_float(const String& key, f32 value);
    void set_bool(const String& key, bool value);
    void remove_key(const String& key);
    void clear();
    Vector<String> get_keys() const;
private:
    Path path_;
    Map<String, String> values_;
    bool parse_line(const String& line, String& key, String& value);
    String serialize_value(const String& key, const String& value);
};

class JSONParser {
public:
    static Optional<Any> parse(const String& json);
    static String stringify(const Any& value, bool pretty = false);
private:
    static Optional<Any> parse_value(const String& json, usize& index);
    static String stringify_value(const Any& value, usize indent = 0);
};

class PluginLoader {
public:
    PluginLoader(const Path& plugin_directory);
    ~PluginLoader();
    bool load_plugin(const String& name);
    bool unload_plugin(const String& name);
    void* get_symbol(const String& plugin_name, const String& symbol_name);
    Vector<String> get_loaded_plugins() const;
    Vector<String> get_available_plugins() const;
private:
    Path plugin_directory_;
    Map<String, void*> loaded_plugins_;
    Map<String, Vector<String>> plugin_symbols_;
};

class Signal {
public:
    using ConnectionId = u64;
    Signal() : next_id_(0) {}
    template<typename... Args>
    ConnectionId connect(Function<void(Args...)> callback) {
        ConnectionId id = next_id_++;
        connections_[id] = [callback](const Any& args) {
            std::apply(callback, std::any_cast<Tuple<Args...>>(args));
        };
        return id;
    }
    void disconnect(ConnectionId id) {
        connections_.erase(id);
    }
    template<typename... Args>
    void emit(Args... args) {
        Tuple<Args...> tuple_args = {args...};
        for (auto& connection : connections_) {
            connection.second(tuple_args);
        }
    }
    void clear() { connections_.clear(); }
    usize connection_count() const { return connections_.size(); }
private:
    Map<ConnectionId, Function<void(const Any&)>> connections_;
    ConnectionId next_id_;
};

template<typename T>
class Property {
public:
    Property() = default;
    Property(const T& value) : value_(value) {}
    Property(T&& value) : value_(std::move(value)) {}
    Property& operator=(const T& value) {
        if (value_ != value) {
            value_ = value;
            changed_signal_.emit(value_);
        }
        return *this;
    }
    Property& operator=(T&& value) {
        if (value_ != std::move(value)) {
            value_ = std::move(value);
            changed_signal_.emit(value_);
        }
        return *this;
    }
    operator const T&() const { return value_; }
    const T& get() const { return value_; }
    void set(const T& value) {
        if (value_ != value) {
            value_ = value;
            changed_signal_.emit(value_);
        }
    }
    void set(T&& value) {
        if (value_ != std::move(value)) {
            value_ = std::move(value);
            changed_signal_.emit(value_);
        }
    }
    Signal<void(const T&)>& changed() { return changed_signal_; }
private:
    T value_;
    Signal<void(const T&)> changed_signal_;
};

template<typename T>
class ObservableVector : public Vector<T> {
public:
    using Vector<T>::Vector;
    void push_back(const T& value) {
        Vector<T>::push_back(value);
        added_signal_.emit(value, this->size() - 1);
    }
    void push_back(T&& value) {
        Vector<T>::push_back(std::move(value));
        added_signal_.emit(this->back(), this->size() - 1);
    }
    void pop_back() {
        if (!this->empty()) {
            T value = this->back();
            Vector<T>::pop_back();
            removed_signal_.emit(value, this->size());
        }
    }
    void insert(usize index, const T& value) {
        Vector<T>::insert(this->begin() + index, value);
        added_signal_.emit(value, index);
    }
    void insert(usize index, T&& value) {
        Vector<T>::insert(this->begin() + index, std::move(value));
        added_signal_.emit(this->at(index), index);
    }
    void erase(usize index) {
        if (index < this->size()) {
            T value = this->at(index);
            Vector<T>::erase(this->begin() + index);
            removed_signal_.emit(value, index);
        }
    }
    void clear() {
        Vector<T>::clear();
        cleared_signal_.emit();
    }
    Signal<void(const T&, usize)>& added() { return added_signal_; }
    Signal<void(const T&, usize)>& removed() { return removed_signal_; }
    Signal<void()>& cleared() { return cleared_signal_; }
private:
    Signal<void(const T&, usize)> added_signal_;
    Signal<void(const T&, usize)> removed_signal_;
    Signal<void()> cleared_signal_;
};

class TaskScheduler {
public:
    TaskScheduler(u32 thread_count = std::thread::hardware_concurrency());
    ~TaskScheduler();
    template<typename F, typename... Args>
    Future<std::invoke_result_t<F, Args...>> schedule(F&& f, Args&&... args) {
        using ReturnType = std::invoke_result_t<F, Args...>;
        auto task = std::make_shared<PackagedTask<ReturnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        Future<ReturnType> future = task->get_future();
        {
            LockGuard<Mutex> lock(queue_mutex_);
            tasks_.push([task]() { (*task)(); });
        }
        condition_.notify_one();
        return future;
    }
    void wait_for_all();
    void stop();
    bool is_stopped() const;
private:
    Vector<Thread> threads_;
    Queue<Function<void()>> tasks_;
    Mutex queue_mutex_;
    ConditionVariable condition_;
    Atomic<bool> stop_;
    void worker_thread();
};

class MemoryPool {
public:
    MemoryPool(usize block_size, usize block_count);
    ~MemoryPool();
    void* allocate();
    void deallocate(void* ptr);
    usize block_size() const;
    usize block_count() const;
    usize used_blocks() const;
    usize free_blocks() const;
    f32 utilization() const;
private:
    usize block_size_;
    usize block_count_;
    usize used_blocks_;
    Vector<u8*> free_blocks_;
    Mutex mutex_;
};

class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();
    template<typename T, typename... Args>
    SharedPtr<T> load(const String& name, Args&&... args) {
        String key = typeid(T).name() + name;
        auto it = resources_.find(key);
        if (it != resources_.end()) {
            return std::static_pointer_cast<T>(it->second);
        }
        auto resource = std::make_shared<T>(std::forward<Args>(args)...);
        resources_[key] = resource;
        return resource;
    }
    template<typename T>
    void unload(const String& name) {
        String key = typeid(T).name() + name;
        resources_.erase(key);
    }
    void clear();
    usize resource_count() const;
private:
    Map<String, Any> resources_;
};

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void run();
    void quit();
    void post_event(const Event& event);
    void process_events();
    bool has_pending_events() const;
    usize pending_event_count() const;
private:
    Queue<Event> event_queue_;
    Mutex queue_mutex_;
    ConditionVariable condition_;
    Atomic<bool> running_;
    Thread event_thread_;
    void event_loop();
};

class Application {
public:
    Application(int argc, char** argv);
    ~Application();
    int exec();
    void quit(int exit_code = 0);
    String get_application_name() const;
    String get_application_version() const;
    String get_organization_name() const;
    Path get_application_directory() const;
    Path get_config_directory() const;
    Path get_cache_directory() const;
    Path get_temp_directory() const;
    Path get_data_directory() const;
    void set_application_name(const String& name);
    void set_application_version(const String& version);
    void set_organization_name(const String& name);
    static Application* instance();
    EventLoop& event_loop();
private:
    static Application* instance_;
    String application_name_;
    String application_version_;
    String organization_name_;
    Vector<String> arguments_;
    EventLoop event_loop_;
    int exit_code_;
    Atomic<bool> quit_requested_;
};

} // namespace su1
