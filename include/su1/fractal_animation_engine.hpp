#pragma once

#include "su1/core.hpp"
#include <vector>
#include <complex>
#include <memory>
#include <thread>
#include <atomic>
#include <string>

namespace SU1 {

class FractalAnimationEngine {
public:
    FractalAnimationEngine();
    ~FractalAnimationEngine();

    bool initialize();
    void shutdown();

    double get_fractal_complexity() const;
    double get_animation_smoothness() const;
    double get_particle_density() const;
    double get_field_strength() const;
    double get_rendering_quality() const;

    void set_fractal_dimension(double dimension);
    void set_self_similarity_factor(double factor);
    void set_chaos_parameter(double parameter);
    void set_time_step(double step);

    void enable_fractal_animations(bool enable);
    void enable_fractal_particles(bool enable);
    void enable_fractal_fields(bool enable);
    void enable_fractal_rendering(bool enable);

    void create_mandelbrot_animation(const std::string& id, double center_x, double center_y, double zoom);
    void create_julia_animation(const std::string& id, double c_real, double c_imag, double zoom);
    void add_fractal_particle(double x, double y, double mass, double charge);
    void create_fractal_field(int resolution, double strength, double frequency);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    void animation_update_loop();
    void particle_update_loop();
    void field_update_loop();
    void rendering_loop();
    void perform_fractal_rendering();
};

} // namespace SU1

