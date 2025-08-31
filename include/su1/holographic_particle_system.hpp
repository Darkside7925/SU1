#pragma once

#include "su1/core.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include <complex>
#include <memory>
#include <thread>
#include <atomic>

namespace SU1 {

struct Vec2 {
    f32 x, y;
};

struct Vec4 {
    f32 x, y, z, w;
};

class HolographicParticleSystem {
public:
    HolographicParticleSystem();
    ~HolographicParticleSystem();

    bool initialize(VkDevice device, VkPhysicalDevice physical_device);
    void shutdown();

    float get_total_energy() const;
    float get_average_coherence() const;
    float get_interference_density() const;
    float get_holographic_coverage() const;

    void set_global_gravity(float gravity);
    void set_air_resistance(float resistance);
    void set_time_scale(float scale);
    void set_particle_density(float density);

    void enable_holographic_effects(bool enable);
    void enable_quantum_coherence(bool enable);
    void enable_interference_fields(bool enable);
    void enable_particle_emitters(bool enable);

    void set_max_particles(int max_particles);
    void set_max_emitters(int max_emitters);
    void set_max_interference_fields(int max_fields);

    void add_particle_emitter(const Vec2& position, float emission_rate);
    void add_interference_field(const Vec2& center, float radius, float strength);
    void create_particle_explosion(const Vec2& position, int particle_count);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;

    void particle_simulation_loop();
    void emitter_update_loop();
    void interference_update_loop();
    void holographic_update_loop();

    bool create_vulkan_resources();
    bool create_particle_buffers();
    bool create_buffer(size_t size, VkBufferUsageFlags usage, VkBuffer* buffer, VkDeviceMemory* memory);
    void cleanup_vulkan_resources();
};

} // namespace SU1

