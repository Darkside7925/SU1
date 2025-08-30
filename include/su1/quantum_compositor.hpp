#pragma once

#include "su1/core.hpp"
#include <vulkan/vulkan.h>
#include <vector>
#include <memory>
#include <thread>
#include <atomic>

namespace SU1 {

class QuantumCompositor {
public:
    QuantumCompositor();
    ~QuantumCompositor();
    
    bool initialize(VkDevice device, VkPhysicalDevice physical_device);
    void shutdown();
    
    float get_quantum_entanglement_strength() const;
    float get_holographic_interference_intensity() const;
    float get_fractal_self_similarity() const;
    float get_consciousness_awareness_factor() const;
    
    void set_quantum_entanglement_strength(float strength);
    void set_holographic_interference_intensity(float intensity);
    void set_fractal_self_similarity(float similarity);
    void set_consciousness_awareness_factor(float factor);
    
    void enable_quantum_composition(bool enable);
    void enable_holographic_rendering(bool enable);
    void enable_fractal_composition(bool enable);
    void enable_consciousness_driven_composition(bool enable);
    
    void set_rendering_quality(float quality);
    void set_performance_target(float target);
    void set_energy_efficiency(float efficiency);
    void set_visual_fidelity(float fidelity);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
    
    void composition_loop();
    void rendering_loop();
    void optimization_loop();
    
    void quantum_composition_pass();
    void holographic_rendering_pass();
    void fractal_composition_pass();
    void consciousness_driven_pass();
    
    void update_composition_matrix();
    void apply_rendering_weights();
    void optimize_composition_parameters();
    
    void adaptive_quality_adjustment();
    void performance_optimization();
    void energy_efficiency_tuning();
    
    bool create_vulkan_resources();
    bool create_composition_buffers();
    bool create_buffer(size_t size, VkBufferUsageFlags usage, VkBuffer* buffer, VkDeviceMemory* memory);
    void cleanup_vulkan_resources();
};

} // namespace SU1
