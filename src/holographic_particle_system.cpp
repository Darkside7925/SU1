#include "su1/holographic_particle_system.hpp"
#include "su1/core.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <thread>
#include <atomic>
#include <cmath>
#include <complex>

namespace SU1 {

class HolographicParticleSystem::Impl {
public:
    struct HolographicParticle {
        Vec2 position;
        Vec2 velocity;
        Vec2 acceleration;
        float life_time;
        float max_life_time;
        float size;
        Vec4 color;
        float rotation;
        float rotation_speed;
        float holographic_intensity;
        float interference_pattern;
        std::vector<std::complex<float>> quantum_states;
        Vec2 force_field;
        float energy_level;
        float coherence_factor;
        int particle_type;
        bool is_active;
    };

    struct HolographicEmitter {
        Vec2 position;
        Vec2 direction;
        float emission_rate;
        float particle_speed;
        float particle_life_time;
        float spread_angle;
        Vec4 base_color;
        int particle_type;
        bool is_active;
        float holographic_density;
        float quantum_entanglement;
    };

    struct InterferenceField {
        Vec2 center;
        float radius;
        float strength;
        float frequency;
        float phase;
        Vec4 color_modulation;
        bool is_active;
        float holographic_projection;
        std::vector<std::complex<float>> field_states;
    };

    VkDevice device_;
    VkPhysicalDevice physical_device_;
    VkCommandPool command_pool_;
    VkDescriptorPool descriptor_pool_;

    VkPipeline particle_pipeline_;
    VkPipeline emitter_pipeline_;
    VkPipeline interference_pipeline_;
    VkPipeline holographic_pipeline_;

    VkBuffer particle_buffer_;
    VkBuffer emitter_buffer_;
    VkBuffer interference_buffer_;
    VkBuffer holographic_buffer_;

    VkDeviceMemory particle_memory_;
    VkDeviceMemory emitter_memory_;
    VkDeviceMemory interference_memory_;
    VkDeviceMemory holographic_memory_;

    std::vector<HolographicParticle> particles_;
    std::vector<HolographicEmitter> emitters_;
    std::vector<InterferenceField> interference_fields_;

    std::atomic<bool> active_{true};
    std::thread particle_thread_;
    std::thread emitter_thread_;
    std::thread interference_thread_;
    std::thread holographic_thread_;

    float global_gravity_;
    float air_resistance_;
    float time_scale_;
    float particle_density_;
    float holographic_intensity_;
    float quantum_coherence_;
    float interference_strength_;
    float energy_dissipation_rate_;

    std::vector<std::vector<float>> force_fields_;
    std::vector<std::vector<std::complex<float>>> quantum_fields_;
    std::vector<std::vector<float>> holographic_patterns_;

    std::mt19937 random_engine_;
    std::normal_distribution<float> noise_dist_;
    std::uniform_real_distribution<float> uniform_dist_;
    std::uniform_real_distribution<float> angle_dist_;

    int max_particles_;
    int max_emitters_;
    int max_interference_fields_;
    int particle_grid_size_;
    int force_field_resolution_;

    float particle_render_distance_;
    float emitter_update_rate_;
    float interference_update_rate_;
    float holographic_update_rate_;

    float total_energy_;
    float average_coherence_;
    float interference_density_;
    float holographic_coverage_;

public:
    Impl()
        : global_gravity_(0.0f)
        , air_resistance_(0.98f)
        , time_scale_(1.0f)
        , particle_density_(1.0f)
        , holographic_intensity_(0.8f)
        , quantum_coherence_(0.9f)
        , interference_strength_(0.7f)
        , energy_dissipation_rate_(0.95f)
        , random_engine_(std::random_device{}())
        , noise_dist_(0.0f, 0.1f)
        , uniform_dist_(0.0f, 1.0f)
        , angle_dist_(0.0f, 2.0f * 3.14159f)
        , max_particles_(10000)
        , max_emitters_(50)
        , max_interference_fields_(20)
        , particle_grid_size_(64)
        , force_field_resolution_(32)
        , particle_render_distance_(1000.0f)
        , emitter_update_rate_(60.0f)
        , interference_update_rate_(30.0f)
        , holographic_update_rate_(120.0f)
        , total_energy_(0.0f)
        , average_coherence_(0.0f)
        , interference_density_(0.0f)
        , holographic_coverage_(0.0f) {

        initialize_particles();
        initialize_emitters();
        initialize_interference_fields();

        force_fields_.resize(force_field_resolution_, std::vector<float>(force_field_resolution_, 0.0f));
        quantum_fields_.resize(particle_grid_size_, std::vector<std::complex<float>>(particle_grid_size_));
        holographic_patterns_.resize(particle_grid_size_, std::vector<float>(particle_grid_size_, 0.0f));
    }

    ~Impl() {
        active_ = false;
        if (particle_thread_.joinable()) particle_thread_.join();
        if (emitter_thread_.joinable()) emitter_thread_.join();
        if (interference_thread_.joinable()) interference_thread_.join();
        if (holographic_thread_.joinable()) holographic_thread_.join();
    }

    void initialize_particles() {
        particles_.resize(max_particles_);
        for (auto& particle : particles_) {
            reset_particle(particle);
        }
    }

    void initialize_emitters() {
        emitters_.resize(max_emitters_);
        for (auto& emitter : emitters_) {
            emitter.position = {uniform_dist_(random_engine_) * 1920.0f, uniform_dist_(random_engine_) * 1080.0f};
            emitter.direction = {uniform_dist_(random_engine_) * 2.0f - 1.0f, uniform_dist_(random_engine_) * 2.0f - 1.0f};
            emitter.emission_rate = uniform_dist_(random_engine_) * 100.0f + 10.0f;
            emitter.particle_speed = uniform_dist_(random_engine_) * 200.0f + 50.0f;
            emitter.particle_life_time = uniform_dist_(random_engine_) * 5.0f + 1.0f;
            emitter.spread_angle = uniform_dist_(random_engine_) * 3.14159f * 0.5f;
            emitter.base_color = {uniform_dist_(random_engine_), uniform_dist_(random_engine_), uniform_dist_(random_engine_), 1.0f};
            emitter.particle_type = random_engine_() % 5;
            emitter.is_active = uniform_dist_(random_engine_) > 0.5f;
            emitter.holographic_density = uniform_dist_(random_engine_);
            emitter.quantum_entanglement = uniform_dist_(random_engine_);
        }
    }

    void initialize_interference_fields() {
        interference_fields_.resize(max_interference_fields_);
        for (auto& field : interference_fields_) {
            field.center = {uniform_dist_(random_engine_) * 1920.0f, uniform_dist_(random_engine_) * 1080.0f};
            field.radius = uniform_dist_(random_engine_) * 300.0f + 50.0f;
            field.strength = uniform_dist_(random_engine_) * 2.0f;
            field.frequency = uniform_dist_(random_engine_) * 10.0f + 1.0f;
            field.phase = angle_dist_(random_engine_);
            field.color_modulation = {uniform_dist_(random_engine_), uniform_dist_(random_engine_), uniform_dist_(random_engine_), uniform_dist_(random_engine_)};
            field.is_active = uniform_dist_(random_engine_) > 0.3f;
            field.holographic_projection = uniform_dist_(random_engine_);
            field.field_states.resize(16);
            for (auto& state : field.field_states) {
                state = std::complex<float>(uniform_dist_(random_engine_), uniform_dist_(random_engine_));
            }
        }
    }

    void reset_particle(HolographicParticle& particle) {
        particle.position = {uniform_dist_(random_engine_) * 1920.0f, uniform_dist_(random_engine_) * 1080.0f};
        particle.velocity = {uniform_dist_(random_engine_) * 200.0f - 100.0f, uniform_dist_(random_engine_) * 200.0f - 100.0f};
        particle.acceleration = {0.0f, 0.0f};
        particle.life_time = 0.0f;
        particle.max_life_time = uniform_dist_(random_engine_) * 5.0f + 1.0f;
        particle.size = uniform_dist_(random_engine_) * 10.0f + 2.0f;
        particle.color = {uniform_dist_(random_engine_), uniform_dist_(random_engine_), uniform_dist_(random_engine_), 1.0f};
        particle.rotation = angle_dist_(random_engine_);
        particle.rotation_speed = uniform_dist_(random_engine_) * 10.0f - 5.0f;
        particle.holographic_intensity = uniform_dist_(random_engine_);
        particle.interference_pattern = uniform_dist_(random_engine_);
        particle.quantum_states.resize(8);
        for (auto& state : particle.quantum_states) {
            state = std::complex<float>(uniform_dist_(random_engine_), uniform_dist_(random_engine_));
        }
        particle.force_field = {uniform_dist_(random_engine_) * 2.0f - 1.0f, uniform_dist_(random_engine_) * 2.0f - 1.0f};
        particle.energy_level = uniform_dist_(random_engine_);
        particle.coherence_factor = uniform_dist_(random_engine_);
        particle.particle_type = random_engine_() % 5;
        particle.is_active = false;
    }

    void update_particles(float delta_time) {
        for (auto& particle : particles_) {
            if (!particle.is_active) continue;

            // Update lifetime
            particle.life_time += delta_time;
            if (particle.life_time >= particle.max_life_time) {
                particle.is_active = false;
                continue;
            }

            // Update quantum states
            update_quantum_states(particle, delta_time);

            // Apply forces
            apply_forces(particle, delta_time);

            // Apply interference fields
            apply_interference_fields(particle, delta_time);

            // Update holographic properties
            update_holographic_properties(particle, delta_time);

            // Update position and velocity
            particle.velocity.x += particle.acceleration.x * delta_time;
            particle.velocity.y += particle.acceleration.y * delta_time;

            particle.position.x += particle.velocity.x * delta_time * time_scale_;
            particle.position.y += particle.velocity.y * delta_time * time_scale_;

            // Apply air resistance
            particle.velocity.x *= air_resistance_;
            particle.velocity.y *= air_resistance_;

            // Update rotation
            particle.rotation += particle.rotation_speed * delta_time;

            // Update color based on lifetime
            float life_ratio = particle.life_time / particle.max_life_time;
            particle.color.w = 1.0f - life_ratio * life_ratio;

            // Update energy
            particle.energy_level *= energy_dissipation_rate_;
        }
    }

    void update_quantum_states(HolographicParticle& particle, float delta_time) {
        for (size_t i = 0; i < particle.quantum_states.size(); ++i) {
            // Apply quantum evolution
            float phase = 2.0f * 3.14159f * particle.energy_level * delta_time * (i + 1);
            std::complex<float> evolution_factor = std::complex<float>(std::cos(phase), std::sin(phase));

            particle.quantum_states[i] *= evolution_factor * particle.coherence_factor;

            // Apply decoherence
            particle.quantum_states[i] *= (1.0f - delta_time * 0.1f);
        }

        // Normalize quantum states
        float total_probability = 0.0f;
        for (const auto& state : particle.quantum_states) {
            total_probability += std::norm(state);
        }

        if (total_probability > 0.0f) {
            float normalization_factor = 1.0f / std::sqrt(total_probability);
            for (auto& state : particle.quantum_states) {
                state *= normalization_factor;
            }
        }
    }

    void apply_forces(HolographicParticle& particle, float delta_time) {
        // Apply global gravity
        particle.acceleration.y += global_gravity_;

        // Apply force field
        particle.acceleration.x += particle.force_field.x * 10.0f;
        particle.acceleration.y += particle.force_field.y * 10.0f;

        // Apply particle-to-particle forces
        for (const auto& other : particles_) {
            if (&other == &particle || !other.is_active) continue;

            Vec2 direction = {
                other.position.x - particle.position.x,
                other.position.y - particle.position.y
            };

            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
            if (distance > 0.0f && distance < 50.0f) {
                float force = (50.0f - distance) / 50.0f * particle_density_;
                direction.x /= distance;
                direction.y /= distance;

                particle.acceleration.x -= direction.x * force * delta_time;
                particle.acceleration.y -= direction.y * force * delta_time;
            }
        }
    }

    void apply_interference_fields(HolographicParticle& particle, float delta_time) {
        for (const auto& field : interference_fields_) {
            if (!field.is_active) continue;

            Vec2 direction = {
                particle.position.x - field.center.x,
                particle.position.y - field.center.y
            };

            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance < field.radius) {
                float interference_factor = (field.radius - distance) / field.radius;
                float wave_phase = field.frequency * particle.life_time + field.phase;

                float interference_x = std::cos(wave_phase) * field.strength * interference_factor;
                float interference_y = std::sin(wave_phase) * field.strength * interference_factor;

                particle.velocity.x += interference_x * delta_time;
                particle.velocity.y += interference_y * delta_time;

                // Apply color modulation
                particle.color.x = std::min(1.0f, particle.color.x + field.color_modulation.x * interference_factor);
                particle.color.y = std::min(1.0f, particle.color.y + field.color_modulation.y * interference_factor);
                particle.color.z = std::min(1.0f, particle.color.z + field.color_modulation.z * interference_factor);

                // Update interference pattern
                particle.interference_pattern += field.holographic_projection * interference_factor * delta_time;
            }
        }
    }

    void update_holographic_properties(HolographicParticle& particle, float delta_time) {
        // Update holographic intensity based on quantum coherence
        float coherence_sum = 0.0f;
        for (const auto& state : particle.quantum_states) {
            coherence_sum += std::norm(state);
        }
        particle.holographic_intensity = coherence_sum / particle.quantum_states.size();

        // Update particle size based on energy
        particle.size *= (1.0f + particle.energy_level * delta_time * 0.1f);
        particle.size = std::max(1.0f, std::min(20.0f, particle.size));

        // Apply holographic projection effects
        particle.position.x += std::sin(particle.interference_pattern) * 2.0f * delta_time;
        particle.position.y += std::cos(particle.interference_pattern) * 2.0f * delta_time;
    }

    void update_emitters(float delta_time) {
        for (auto& emitter : emitters_) {
            if (!emitter.is_active) continue;

            // Emit particles
            float particles_to_emit = emitter.emission_rate * delta_time;
            int particles_emitted = 0;

            for (auto& particle : particles_) {
                if (!particle.is_active && particles_emitted < particles_to_emit) {
                    emit_particle(emitter, particle);
                    particles_emitted++;
                }
            }

            // Update emitter properties
            emitter.position.x += emitter.direction.x * 10.0f * delta_time;
            emitter.position.y += emitter.direction.y * 10.0f * delta_time;

            // Bounce off screen edges
            if (emitter.position.x < 0 || emitter.position.x > 1920) emitter.direction.x *= -1;
            if (emitter.position.y < 0 || emitter.position.y > 1080) emitter.direction.y *= -1;
        }
    }

    void emit_particle(const HolographicEmitter& emitter, HolographicParticle& particle) {
        reset_particle(particle);
        particle.is_active = true;

        // Set initial position near emitter
        particle.position = emitter.position;
        particle.position.x += uniform_dist_(random_engine_) * 20.0f - 10.0f;
        particle.position.y += uniform_dist_(random_engine_) * 20.0f - 10.0f;

        // Set initial velocity based on emitter direction
        float angle = std::atan2(emitter.direction.y, emitter.direction.x);
        angle += (uniform_dist_(random_engine_) - 0.5f) * emitter.spread_angle;

        particle.velocity.x = std::cos(angle) * emitter.particle_speed;
        particle.velocity.y = std::sin(angle) * emitter.particle_speed;

        // Set particle properties based on emitter
        particle.color = emitter.base_color;
        particle.color.w = 0.8f;
        particle.max_life_time = emitter.particle_life_time;
        particle.particle_type = emitter.particle_type;
        particle.holographic_intensity = emitter.holographic_density;
        particle.energy_level = 1.0f;

        // Initialize quantum states based on emitter
        for (auto& state : particle.quantum_states) {
            state = std::complex<float>(
                uniform_dist_(random_engine_) * emitter.quantum_entanglement,
                uniform_dist_(random_engine_) * emitter.quantum_entanglement
            );
        }
    }

    void update_interference_fields(float delta_time) {
        for (auto& field : interference_fields_) {
            if (!field.is_active) continue;

            // Update field phase
            field.phase += field.frequency * delta_time;

            // Update field states
            for (size_t i = 0; i < field.field_states.size(); ++i) {
                float phase = 2.0f * 3.14159f * field.frequency * delta_time * (i + 1) / field.field_states.size();
                std::complex<float> evolution_factor = std::complex<float>(std::cos(phase), std::sin(phase));
                field.field_states[i] *= evolution_factor;
            }

            // Update field strength based on energy
            field.strength *= (1.0f - delta_time * 0.1f);
            field.strength += noise_dist_(random_engine_) * delta_time;
            field.strength = std::max(0.0f, std::min(3.0f, field.strength));
        }
    }

    void update_force_fields(float delta_time) {
        // Update force fields based on particle distribution
        std::fill(force_fields_.begin(), force_fields_.end(), std::vector<float>(force_field_resolution_, 0.0f));

        for (const auto& particle : particles_) {
            if (!particle.is_active) continue;

            int grid_x = static_cast<int>(particle.position.x / (1920.0f / force_field_resolution_));
            int grid_y = static_cast<int>(particle.position.y / (1080.0f / force_field_resolution_));

            if (grid_x >= 0 && grid_x < force_field_resolution_ &&
                grid_y >= 0 && grid_y < force_field_resolution_) {
                force_fields_[grid_x][grid_y] += particle.energy_level;
            }
        }

        // Smooth force fields
        for (int x = 1; x < force_field_resolution_ - 1; ++x) {
            for (int y = 1; y < force_field_resolution_ - 1; ++y) {
                float average = (force_fields_[x-1][y-1] + force_fields_[x-1][y] + force_fields_[x-1][y+1] +
                               force_fields_[x][y-1] + force_fields_[x][y] + force_fields_[x][y+1] +
                               force_fields_[x+1][y-1] + force_fields_[x+1][y] + force_fields_[x+1][y+1]) / 9.0f;
                force_fields_[x][y] = force_fields_[x][y] * 0.8f + average * 0.2f;
            }
        }
    }

    void update_holographic_patterns(float delta_time) {
        for (int x = 0; x < particle_grid_size_; ++x) {
            for (int y = 0; y < particle_grid_size_; ++y) {
                float interference_sum = 0.0f;

                // Calculate interference from nearby particles
                for (const auto& particle : particles_) {
                    if (!particle.is_active) continue;

                    int particle_x = static_cast<int>(particle.position.x / (1920.0f / particle_grid_size_));
                    int particle_y = static_cast<int>(particle.position.y / (1080.0f / particle_grid_size_));

                    if (std::abs(particle_x - x) < 3 && std::abs(particle_y - y) < 3) {
                        float distance = std::sqrt((particle_x - x) * (particle_x - x) + (particle_y - y) * (particle_y - y));
                        if (distance > 0.0f) {
                            interference_sum += particle.holographic_intensity / distance;
                        }
                    }
                }

                holographic_patterns_[x][y] = holographic_patterns_[x][y] * 0.95f + interference_sum * 0.05f;
            }
        }
    }

    void update_system_metrics() {
        total_energy_ = 0.0f;
        average_coherence_ = 0.0f;
        interference_density_ = 0.0f;
        holographic_coverage_ = 0.0f;

        int active_particles = 0;
        for (const auto& particle : particles_) {
            if (particle.is_active) {
                total_energy_ += particle.energy_level;
                average_coherence_ += particle.coherence_factor;
                interference_density_ += particle.interference_pattern;
                holographic_coverage_ += particle.holographic_intensity;
                active_particles++;
            }
        }

        if (active_particles > 0) {
            average_coherence_ /= active_particles;
            interference_density_ /= active_particles;
            holographic_coverage_ /= active_particles;
        }
    }
};

HolographicParticleSystem::HolographicParticleSystem() : impl_(std::make_unique<Impl>()) {}

HolographicParticleSystem::~HolographicParticleSystem() = default;

bool HolographicParticleSystem::initialize(VkDevice device, VkPhysicalDevice physical_device) {
    impl_->device_ = device;
    impl_->physical_device_ = physical_device;

    if (!create_vulkan_resources()) {
        return false;
    }

    impl_->particle_thread_ = std::thread([this]() {
        particle_simulation_loop();
    });

    impl_->emitter_thread_ = std::thread([this]() {
        emitter_update_loop();
    });

    impl_->interference_thread_ = std::thread([this]() {
        interference_update_loop();
    });

    impl_->holographic_thread_ = std::thread([this]() {
        holographic_update_loop();
    });

    return true;
}

void HolographicParticleSystem::shutdown() {
    impl_->active_ = false;
    if (impl_->particle_thread_.joinable()) impl_->particle_thread_.join();
    if (impl_->emitter_thread_.joinable()) impl_->emitter_thread_.join();
    if (impl_->interference_thread_.joinable()) impl_->interference_thread_.join();
    if (impl_->holographic_thread_.joinable()) impl_->holographic_thread_.join();

    cleanup_vulkan_resources();
}

bool HolographicParticleSystem::create_vulkan_resources() {
    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = 0;

    if (vkCreateCommandPool(impl_->device_, &pool_info, nullptr, &impl_->command_pool_) != VK_SUCCESS) {
        return false;
    }

    VkDescriptorPoolCreateInfo descriptor_pool_info = {};
    descriptor_pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    descriptor_pool_info.maxSets = 100;
    descriptor_pool_info.poolSizeCount = 0;

    if (vkCreateDescriptorPool(impl_->device_, &descriptor_pool_info, nullptr, &impl_->descriptor_pool_) != VK_SUCCESS) {
        return false;
    }

    return create_particle_buffers();
}

bool HolographicParticleSystem::create_particle_buffers() {
    size_t particle_buffer_size = impl_->max_particles_ * sizeof(HolographicParticle);
    size_t emitter_buffer_size = impl_->max_emitters_ * sizeof(HolographicEmitter);
    size_t interference_buffer_size = impl_->max_interference_fields_ * sizeof(InterferenceField);
    size_t holographic_buffer_size = impl_->particle_grid_size_ * impl_->particle_grid_size_ * 4;

    if (!create_buffer(particle_buffer_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                      &impl_->particle_buffer_, &impl_->particle_memory_)) {
        return false;
    }

    if (!create_buffer(emitter_buffer_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                      &impl_->emitter_buffer_, &impl_->emitter_memory_)) {
        return false;
    }

    if (!create_buffer(interference_buffer_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                      &impl_->interference_buffer_, &impl_->interference_memory_)) {
        return false;
    }

    if (!create_buffer(holographic_buffer_size, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                      &impl_->holographic_buffer_, &impl_->holographic_memory_)) {
        return false;
    }

    return true;
}

bool HolographicParticleSystem::create_buffer(size_t size, VkBufferUsageFlags usage, VkBuffer* buffer, VkDeviceMemory* memory) {
    VkBufferCreateInfo buffer_info = {};
    buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buffer_info.size = size;
    buffer_info.usage = usage;
    buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(impl_->device_, &buffer_info, nullptr, buffer) != VK_SUCCESS) {
        return false;
    }

    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(impl_->device_, *buffer, &mem_requirements);

    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(impl_->physical_device_, &mem_properties);

    u32 memory_type = UINT32_MAX;
    for (u32 i = 0; i < mem_properties.memoryTypeCount; i++) {
        if ((mem_requirements.memoryTypeBits & (1 << i)) &&
            (mem_properties.memoryTypes[i].propertyFlags &
             (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))) {
            memory_type = i;
            break;
        }
    }

    if (memory_type == UINT32_MAX) {
        return false;
    }

    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = mem_requirements.size;
    alloc_info.memoryTypeIndex = memory_type;

    if (vkAllocateMemory(impl_->device_, &alloc_info, nullptr, memory) != VK_SUCCESS) {
        return false;
    }

    return vkBindBufferMemory(impl_->device_, *buffer, *memory, 0) == VK_SUCCESS;
}

void HolographicParticleSystem::particle_simulation_loop() {
    auto last_time = std::chrono::high_resolution_clock::now();

    while (impl_->active_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - last_time).count();
        last_time = current_time;

        impl_->update_particles(delta_time);
        impl_->update_force_fields(delta_time);
        impl_->update_system_metrics();

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void HolographicParticleSystem::emitter_update_loop() {
    auto last_time = std::chrono::high_resolution_clock::now();

    while (impl_->active_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - last_time).count();
        last_time = current_time;

        impl_->update_emitters(delta_time);

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0f / impl_->emitter_update_rate_)));
    }
}

void HolographicParticleSystem::interference_update_loop() {
    auto last_time = std::chrono::high_resolution_clock::now();

    while (impl_->active_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - last_time).count();
        last_time = current_time;

        impl_->update_interference_fields(delta_time);

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0f / impl_->interference_update_rate_)));
    }
}

void HolographicParticleSystem::holographic_update_loop() {
    auto last_time = std::chrono::high_resolution_clock::now();

    while (impl_->active_) {
        auto current_time = std::chrono::high_resolution_clock::now();
        float delta_time = std::chrono::duration<float>(current_time - last_time).count();
        last_time = current_time;

        impl_->update_holographic_patterns(delta_time);

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(1000.0f / impl_->holographic_update_rate_)));
    }
}

void HolographicParticleSystem::cleanup_vulkan_resources() {
    if (impl_->device_ != VK_NULL_HANDLE) {
        vkDeviceWaitIdle(impl_->device_);

        if (impl_->particle_buffer_ != VK_NULL_HANDLE) {
            vkDestroyBuffer(impl_->device_, impl_->particle_buffer_, nullptr);
        }
        if (impl_->emitter_buffer_ != VK_NULL_HANDLE) {
            vkDestroyBuffer(impl_->device_, impl_->emitter_buffer_, nullptr);
        }
        if (impl_->interference_buffer_ != VK_NULL_HANDLE) {
            vkDestroyBuffer(impl_->device_, impl_->interference_buffer_, nullptr);
        }
        if (impl_->holographic_buffer_ != VK_NULL_HANDLE) {
            vkDestroyBuffer(impl_->device_, impl_->holographic_buffer_, nullptr);
        }

        if (impl_->particle_memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(impl_->device_, impl_->particle_memory_, nullptr);
        }
        if (impl_->emitter_memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(impl_->device_, impl_->emitter_memory_, nullptr);
        }
        if (impl_->interference_memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(impl_->device_, impl_->interference_memory_, nullptr);
        }
        if (impl_->holographic_memory_ != VK_NULL_HANDLE) {
            vkFreeMemory(impl_->device_, impl_->holographic_memory_, nullptr);
        }

        if (impl_->descriptor_pool_ != VK_NULL_HANDLE) {
            vkDestroyDescriptorPool(impl_->device_, impl_->descriptor_pool_, nullptr);
        }

        if (impl_->command_pool_ != VK_NULL_HANDLE) {
            vkDestroyCommandPool(impl_->device_, impl_->command_pool_, nullptr);
        }
    }
}

} // namespace SU1
