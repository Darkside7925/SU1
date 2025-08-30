//
// SU1 GPU Accelerated UI System
// Comprehensive GPU acceleration for all UI components
//

#include "su1/gpu_accelerated_ui.hpp"
#include "su1/core.hpp"
#include <vulkan/vulkan.h>
#include <cmath>
#include <algorithm>
#include <thread>
#include <chrono>

namespace SU1 {

class GPUAcceleratedUI::Impl {
public:
    // Vulkan state
    VkDevice device_ = VK_NULL_HANDLE;
    VkPhysicalDevice physical_device_ = VK_NULL_HANDLE;
    VkQueue graphics_queue_ = VK_NULL_HANDLE;
    VkQueue compute_queue_ = VK_NULL_HANDLE;
    VkCommandPool command_pool_ = VK_NULL_HANDLE;
    VkDescriptorPool descriptor_pool_ = VK_NULL_HANDLE;
    
    // UI rendering pipelines
    VkPipeline text_rendering_pipeline_ = VK_NULL_HANDLE;
    VkPipeline button_rendering_pipeline_ = VK_NULL_HANDLE;
    VkPipeline panel_rendering_pipeline_ = VK_NULL_HANDLE;
    VkPipeline window_rendering_pipeline_ = VK_NULL_HANDLE;
    VkPipeline icon_rendering_pipeline_ = VK_NULL_HANDLE;
    VkPipeline blur_pipeline_ = VK_NULL_HANDLE;
    VkPipeline shadow_pipeline_ = VK_NULL_HANDLE;
    VkPipeline glow_pipeline_ = VK_NULL_HANDLE;
    
    // Compute pipelines for effects
    VkPipeline ui_physics_pipeline_ = VK_NULL_HANDLE;
    VkPipeline particle_update_pipeline_ = VK_NULL_HANDLE;
    VkPipeline layout_compute_pipeline_ = VK_NULL_HANDLE;
    VkPipeline text_layout_pipeline_ = VK_NULL_HANDLE;
    VkPipeline animation_pipeline_ = VK_NULL_HANDLE;
    
    // GPU buffers
    VkBuffer vertex_buffer_ = VK_NULL_HANDLE;
    VkBuffer index_buffer_ = VK_NULL_HANDLE;
    VkBuffer uniform_buffer_ = VK_NULL_HANDLE;
    VkBuffer text_buffer_ = VK_NULL_HANDLE;
    VkBuffer glyph_buffer_ = VK_NULL_HANDLE;
    VkBuffer ui_element_buffer_ = VK_NULL_HANDLE;
    VkBuffer animation_buffer_ = VK_NULL_HANDLE;
    
    // Memory for buffers
    VkDeviceMemory vertex_memory_ = VK_NULL_HANDLE;
    VkDeviceMemory index_memory_ = VK_NULL_HANDLE;
    VkDeviceMemory uniform_memory_ = VK_NULL_HANDLE;
    VkDeviceMemory text_memory_ = VK_NULL_HANDLE;
    VkDeviceMemory glyph_memory_ = VK_NULL_HANDLE;
    VkDeviceMemory ui_element_memory_ = VK_NULL_HANDLE;
    VkDeviceMemory animation_memory_ = VK_NULL_HANDLE;
    
    // Textures and images
    VkImage font_atlas_ = VK_NULL_HANDLE;
    VkImageView font_atlas_view_ = VK_NULL_HANDLE;
    VkSampler font_sampler_ = VK_NULL_HANDLE;
    VkImage icon_atlas_ = VK_NULL_HANDLE;
    VkImageView icon_atlas_view_ = VK_NULL_HANDLE;
    VkSampler icon_sampler_ = VK_NULL_HANDLE;
    
    // Render targets
    VkImage ui_color_target_ = VK_NULL_HANDLE;
    VkImageView ui_color_view_ = VK_NULL_HANDLE;
    VkImage ui_depth_target_ = VK_NULL_HANDLE;
    VkImageView ui_depth_view_ = VK_NULL_HANDLE;
    VkFramebuffer ui_framebuffer_ = VK_NULL_HANDLE;
    VkRenderPass ui_render_pass_ = VK_NULL_HANDLE;
    
    // Configuration
    UIAccelerationConfig config_;
    
    // UI element management
    Vector<UIElement> ui_elements_;
    Vector<TextElement> text_elements_;
    Vector<ButtonElement> button_elements_;
    Vector<PanelElement> panel_elements_;
    Vector<WindowElement> window_elements_;
    Vector<IconElement> icon_elements_;
    
    // Animation system
    Vector<UIAnimation> active_animations_;
    std::atomic<f64> animation_time_{0.0};
    
    // Layout system
    std::unique_ptr<GPULayoutEngine> layout_engine_;
    std::unique_ptr<GPUTextRenderer> text_renderer_;
    std::unique_ptr<GPUEffectProcessor> effect_processor_;
    
    // Performance monitoring
    std::atomic<u64> ui_elements_rendered_{0};
    std::atomic<u64> text_glyphs_rendered_{0};
    std::atomic<u64> draw_calls_issued_{0};
    std::atomic<f64> gpu_ui_time_ms_{0.0};
    std::atomic<f64> layout_time_ms_{0.0};
    std::atomic<f64> text_rendering_time_ms_{0.0};
    
    // Optimization flags
    bool gpu_culling_enabled_ = true;
    bool gpu_layout_enabled_ = true;
    bool gpu_text_rendering_ = true;
    bool gpu_effects_enabled_ = true;
    bool batch_rendering_ = true;
    bool instanced_rendering_ = true;
    bool async_compute_ = true;
    
    // Resource management
    u32 max_ui_elements_ = 100000;
    u32 max_text_glyphs_ = 1000000;
    u32 max_vertices_ = 10000000;
    u32 max_indices_ = 30000000;
    
    // Threading
    std::atomic<bool> update_thread_active_{false};
    std::thread ui_update_thread_;
    std::thread layout_thread_;
    std::thread animation_thread_;
};

GPUAcceleratedUI::GPUAcceleratedUI() : impl_(std::make_unique<Impl>()) {}

GPUAcceleratedUI::~GPUAcceleratedUI() {
    shutdown();
}

bool GPUAcceleratedUI::initialize(VkDevice device, VkPhysicalDevice physical_device,
                                 const UIAccelerationConfig& config) {
    impl_->device_ = device;
    impl_->physical_device_ = physical_device;
    impl_->config_ = config;
    
    log_info("Initializing GPU Accelerated UI System");
    
    // Create command pool
    if (!create_command_pool()) {
        log_error("Failed to create command pool");
        return false;
    }
    
    // Create descriptor pool
    if (!create_descriptor_pool()) {
        log_error("Failed to create descriptor pool");
        return false;
    }
    
    // Create render pass
    if (!create_render_pass()) {
        log_error("Failed to create render pass");
        return false;
    }
    
    // Create GPU buffers
    if (!create_gpu_buffers()) {
        log_error("Failed to create GPU buffers");
        return false;
    }
    
    // Create render targets
    if (!create_render_targets()) {
        log_error("Failed to create render targets");
        return false;
    }
    
    // Load shaders and create pipelines
    if (!create_rendering_pipelines()) {
        log_error("Failed to create rendering pipelines");
        return false;
    }
    
    // Initialize font system
    if (!initialize_font_system()) {
        log_error("Failed to initialize font system");
        return false;
    }
    
    // Initialize subsystems
    if (!initialize_subsystems()) {
        log_error("Failed to initialize subsystems");
        return false;
    }
    
    // Start background threads
    start_background_threads();
    
    log_info("GPU Accelerated UI System initialized successfully");
    return true;
}

void GPUAcceleratedUI::shutdown() {
    stop_background_threads();
    cleanup_subsystems();
    cleanup_pipelines();
    cleanup_buffers();
    cleanup_render_targets();
    cleanup_vulkan_objects();
    
    log_info("GPU Accelerated UI System shutdown complete");
}

bool GPUAcceleratedUI::create_command_pool() {
    // Find graphics queue family
    u32 queue_family_count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(impl_->physical_device_, &queue_family_count, nullptr);
    
    std::vector<VkQueueFamilyProperties> queue_families(queue_family_count);
    vkGetPhysicalDeviceQueueFamilyProperties(impl_->physical_device_, &queue_family_count, queue_families.data());
    
    u32 graphics_family = UINT32_MAX;
    for (u32 i = 0; i < queue_families.size(); i++) {
        if (queue_families[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphics_family = i;
            break;
        }
    }
    
    if (graphics_family == UINT32_MAX) {
        return false;
    }
    
    VkCommandPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_info.queueFamilyIndex = graphics_family;
    
    return vkCreateCommandPool(impl_->device_, &pool_info, nullptr, &impl_->command_pool_) == VK_SUCCESS;
}

bool GPUAcceleratedUI::create_descriptor_pool() {
    std::vector<VkDescriptorPoolSize> pool_sizes = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 500},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 200}
    };
    
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
    pool_info.maxSets = 5000;
    pool_info.poolSizeCount = static_cast<u32>(pool_sizes.size());
    pool_info.pPoolSizes = pool_sizes.data();
    
    return vkCreateDescriptorPool(impl_->device_, &pool_info, nullptr, &impl_->descriptor_pool_) == VK_SUCCESS;
}

bool GPUAcceleratedUI::create_render_pass() {
    std::vector<VkAttachmentDescription> attachments(2);
    
    // Color attachment
    attachments[0].format = VK_FORMAT_R8G8B8A8_UNORM;
    attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    // Depth attachment
    attachments[1].format = VK_FORMAT_D32_SFLOAT;
    attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
    attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    VkAttachmentReference color_attachment = {0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
    VkAttachmentReference depth_attachment = {1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
    
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment;
    subpass.pDepthStencilAttachment = &depth_attachment;
    
    VkRenderPassCreateInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = static_cast<u32>(attachments.size());
    render_pass_info.pAttachments = attachments.data();
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    
    return vkCreateRenderPass(impl_->device_, &render_pass_info, nullptr, &impl_->ui_render_pass_) == VK_SUCCESS;
}

bool GPUAcceleratedUI::create_gpu_buffers() {
    log_info("Creating GPU buffers for UI acceleration");
    
    // Vertex buffer
    VkBufferCreateInfo vertex_buffer_info = {};
    vertex_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertex_buffer_info.size = impl_->max_vertices_ * sizeof(UIVertex);
    vertex_buffer_info.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    vertex_buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(impl_->device_, &vertex_buffer_info, nullptr, &impl_->vertex_buffer_) != VK_SUCCESS) {
        return false;
    }
    
    // Index buffer
    VkBufferCreateInfo index_buffer_info = {};
    index_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    index_buffer_info.size = impl_->max_indices_ * sizeof(u32);
    index_buffer_info.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    index_buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(impl_->device_, &index_buffer_info, nullptr, &impl_->index_buffer_) != VK_SUCCESS) {
        return false;
    }
    
    // UI element buffer
    VkBufferCreateInfo ui_element_buffer_info = {};
    ui_element_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    ui_element_buffer_info.size = impl_->max_ui_elements_ * sizeof(UIElementData);
    ui_element_buffer_info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    ui_element_buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(impl_->device_, &ui_element_buffer_info, nullptr, &impl_->ui_element_buffer_) != VK_SUCCESS) {
        return false;
    }
    
    // Text/glyph buffer
    VkBufferCreateInfo text_buffer_info = {};
    text_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    text_buffer_info.size = impl_->max_text_glyphs_ * sizeof(GlyphData);
    text_buffer_info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    text_buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(impl_->device_, &text_buffer_info, nullptr, &impl_->text_buffer_) != VK_SUCCESS) {
        return false;
    }
    
    // Animation buffer
    VkBufferCreateInfo animation_buffer_info = {};
    animation_buffer_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    animation_buffer_info.size = impl_->max_ui_elements_ * sizeof(AnimationData);
    animation_buffer_info.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    animation_buffer_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(impl_->device_, &animation_buffer_info, nullptr, &impl_->animation_buffer_) != VK_SUCCESS) {
        return false;
    }
    
    // Allocate and bind memory for all buffers
    if (!allocate_buffer_memory()) {
        return false;
    }
    
    log_info("GPU buffers created successfully");
    return true;
}

bool GPUAcceleratedUI::allocate_buffer_memory() {
    VkMemoryRequirements mem_requirements;
    
    // Get memory requirements for vertex buffer
    vkGetBufferMemoryRequirements(impl_->device_, impl_->vertex_buffer_, &mem_requirements);
    
    VkPhysicalDeviceMemoryProperties mem_properties;
    vkGetPhysicalDeviceMemoryProperties(impl_->physical_device_, &mem_properties);
    
    u32 memory_type_index = UINT32_MAX;
    for (u32 i = 0; i < mem_properties.memoryTypeCount; i++) {
        if ((mem_requirements.memoryTypeBits & (1 << i)) &&
            (mem_properties.memoryTypes[i].propertyFlags & (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT))) {
            memory_type_index = i;
            break;
        }
    }
    
    if (memory_type_index == UINT32_MAX) {
        return false;
    }
    
    // Calculate total memory needed
    size_t total_memory = 0;
    total_memory += impl_->max_vertices_ * sizeof(UIVertex);
    total_memory += impl_->max_indices_ * sizeof(u32);
    total_memory += impl_->max_ui_elements_ * sizeof(UIElementData);
    total_memory += impl_->max_text_glyphs_ * sizeof(GlyphData);
    total_memory += impl_->max_ui_elements_ * sizeof(AnimationData);
    
    // Allocate single large memory block
    VkMemoryAllocateInfo alloc_info = {};
    alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_info.allocationSize = total_memory;
    alloc_info.memoryTypeIndex = memory_type_index;
    
    if (vkAllocateMemory(impl_->device_, &alloc_info, nullptr, &impl_->vertex_memory_) != VK_SUCCESS) {
        return false;
    }
    
    // Bind buffers to memory with offsets
    VkDeviceSize offset = 0;
    
    vkBindBufferMemory(impl_->device_, impl_->vertex_buffer_, impl_->vertex_memory_, offset);
    offset += impl_->max_vertices_ * sizeof(UIVertex);
    
    vkBindBufferMemory(impl_->device_, impl_->index_buffer_, impl_->vertex_memory_, offset);
    offset += impl_->max_indices_ * sizeof(u32);
    
    vkBindBufferMemory(impl_->device_, impl_->ui_element_buffer_, impl_->vertex_memory_, offset);
    offset += impl_->max_ui_elements_ * sizeof(UIElementData);
    
    vkBindBufferMemory(impl_->device_, impl_->text_buffer_, impl_->vertex_memory_, offset);
    offset += impl_->max_text_glyphs_ * sizeof(GlyphData);
    
    vkBindBufferMemory(impl_->device_, impl_->animation_buffer_, impl_->vertex_memory_, offset);
    
    return true;
}

bool GPUAcceleratedUI::create_render_targets() {
    u32 width = impl_->config_.render_width;
    u32 height = impl_->config_.render_height;
    
    // Create color target
    VkImageCreateInfo color_image_info = {};
    color_image_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    color_image_info.imageType = VK_IMAGE_TYPE_2D;
    color_image_info.extent.width = width;
    color_image_info.extent.height = height;
    color_image_info.extent.depth = 1;
    color_image_info.mipLevels = 1;
    color_image_info.arrayLayers = 1;
    color_image_info.format = VK_FORMAT_R8G8B8A8_UNORM;
    color_image_info.tiling = VK_IMAGE_TILING_OPTIMAL;
    color_image_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_image_info.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    color_image_info.samples = VK_SAMPLE_COUNT_1_BIT;
    color_image_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateImage(impl_->device_, &color_image_info, nullptr, &impl_->ui_color_target_) != VK_SUCCESS) {
        return false;
    }
    
    // Create depth target
    VkImageCreateInfo depth_image_info = color_image_info;
    depth_image_info.format = VK_FORMAT_D32_SFLOAT;
    depth_image_info.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    
    if (vkCreateImage(impl_->device_, &depth_image_info, nullptr, &impl_->ui_depth_target_) != VK_SUCCESS) {
        return false;
    }
    
    // Allocate memory for render targets
    if (!allocate_render_target_memory()) {
        return false;
    }
    
    // Create image views
    if (!create_render_target_views()) {
        return false;
    }
    
    // Create framebuffer
    std::vector<VkImageView> attachments = {impl_->ui_color_view_, impl_->ui_depth_view_};
    
    VkFramebufferCreateInfo framebuffer_info = {};
    framebuffer_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebuffer_info.renderPass = impl_->ui_render_pass_;
    framebuffer_info.attachmentCount = static_cast<u32>(attachments.size());
    framebuffer_info.pAttachments = attachments.data();
    framebuffer_info.width = width;
    framebuffer_info.height = height;
    framebuffer_info.layers = 1;
    
    return vkCreateFramebuffer(impl_->device_, &framebuffer_info, nullptr, &impl_->ui_framebuffer_) == VK_SUCCESS;
}

bool GPUAcceleratedUI::create_rendering_pipelines() {
    log_info("Creating UI rendering pipelines");
    
    // Load shaders
    if (!load_ui_shaders()) {
        return false;
    }
    
    // Create text rendering pipeline
    if (!create_text_rendering_pipeline()) {
        return false;
    }
    
    // Create button rendering pipeline
    if (!create_button_rendering_pipeline()) {
        return false;
    }
    
    // Create panel rendering pipeline
    if (!create_panel_rendering_pipeline()) {
        return false;
    }
    
    // Create compute pipelines
    if (!create_compute_pipelines()) {
        return false;
    }
    
    log_info("UI rendering pipelines created successfully");
    return true;
}

bool GPUAcceleratedUI::initialize_font_system() {
    log_info("Initializing GPU-accelerated font system");
    
    // Create font atlas texture
    if (!create_font_atlas()) {
        return false;
    }
    
    // Load default fonts
    if (!load_default_fonts()) {
        return false;
    }
    
    // Setup glyph cache
    if (!setup_glyph_cache()) {
        return false;
    }
    
    log_info("Font system initialized successfully");
    return true;
}

bool GPUAcceleratedUI::initialize_subsystems() {
    // Initialize layout engine
    impl_->layout_engine_ = std::make_unique<GPULayoutEngine>();
    if (!impl_->layout_engine_->initialize(impl_->device_, impl_->physical_device_)) {
        return false;
    }
    
    // Initialize text renderer
    impl_->text_renderer_ = std::make_unique<GPUTextRenderer>();
    if (!impl_->text_renderer_->initialize(impl_->device_, impl_->physical_device_)) {
        return false;
    }
    
    // Initialize effect processor
    impl_->effect_processor_ = std::make_unique<GPUEffectProcessor>();
    if (!impl_->effect_processor_->initialize(impl_->device_, impl_->physical_device_)) {
        return false;
    }
    
    return true;
}

void GPUAcceleratedUI::start_background_threads() {
    impl_->update_thread_active_ = true;
    
    impl_->ui_update_thread_ = std::thread([this]() {
        ui_update_loop();
    });
    
    impl_->layout_thread_ = std::thread([this]() {
        layout_update_loop();
    });
    
    impl_->animation_thread_ = std::thread([this]() {
        animation_update_loop();
    });
    
    log_info("UI background threads started");
}

void GPUAcceleratedUI::stop_background_threads() {
    impl_->update_thread_active_ = false;
    
    if (impl_->ui_update_thread_.joinable()) {
        impl_->ui_update_thread_.join();
    }
    if (impl_->layout_thread_.joinable()) {
        impl_->layout_thread_.join();
    }
    if (impl_->animation_thread_.joinable()) {
        impl_->animation_thread_.join();
    }
    
    log_info("UI background threads stopped");
}

// Main rendering function
void GPUAcceleratedUI::render_ui(const UIRenderParams& params) {
    auto start_time = std::chrono::high_resolution_clock::now();
    
    VkCommandBuffer cmd = params.command_buffer;
    
    // Begin render pass
    VkRenderPassBeginInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_info.renderPass = impl_->ui_render_pass_;
    render_pass_info.framebuffer = impl_->ui_framebuffer_;
    render_pass_info.renderArea.offset = {0, 0};
    render_pass_info.renderArea.extent = {impl_->config_.render_width, impl_->config_.render_height};
    
    std::vector<VkClearValue> clear_values(2);
    clear_values[0].color = {{0.0f, 0.0f, 0.0f, 0.0f}};
    clear_values[1].depthStencil = {1.0f, 0};
    
    render_pass_info.clearValueCount = static_cast<u32>(clear_values.size());
    render_pass_info.pClearValues = clear_values.data();
    
    vkCmdBeginRenderPass(cmd, &render_pass_info, VK_SUBPASS_CONTENTS_INLINE);
    
    // Set viewport
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<f32>(impl_->config_.render_width);
    viewport.height = static_cast<f32>(impl_->config_.render_height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(cmd, 0, 1, &viewport);
    
    VkRect2D scissor = {};
    scissor.offset = {0, 0};
    scissor.extent = {impl_->config_.render_width, impl_->config_.render_height};
    vkCmdSetScissor(cmd, 0, 1, &scissor);
    
    // Update UI elements on GPU
    update_ui_elements_gpu(cmd, params);
    
    // Perform GPU culling
    if (impl_->gpu_culling_enabled_) {
        perform_gpu_culling(cmd, params);
    }
    
    // Render UI elements by type
    render_panels(cmd, params);
    render_windows(cmd, params);
    render_buttons(cmd, params);
    render_text(cmd, params);
    render_icons(cmd, params);
    
    // Apply post-processing effects
    if (impl_->gpu_effects_enabled_) {
        apply_gpu_effects(cmd, params);
    }
    
    vkCmdEndRenderPass(cmd);
    
    auto end_time = std::chrono::high_resolution_clock::now();
    impl_->gpu_ui_time_ms_ = std::chrono::duration<f64, std::milli>(end_time - start_time).count();
    
    impl_->ui_elements_rendered_++;
}

void GPUAcceleratedUI::update_ui_elements_gpu(VkCommandBuffer cmd, const UIRenderParams& params) {
    // Update animation time
    impl_->animation_time_ += params.delta_time;
    
    // Dispatch compute shader to update UI element transformations
    if (impl_->async_compute_) {
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, impl_->layout_compute_pipeline_);
        
        // Bind descriptor sets for UI element data
        // ... descriptor set binding code ...
        
        // Dispatch compute for layout updates
        u32 group_count = (impl_->ui_elements_.size() + 63) / 64; // 64 threads per group
        vkCmdDispatch(cmd, group_count, 1, 1);
        
        // Memory barrier
        VkMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
        barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT;
        
        vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 
                           VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 1, &barrier, 0, nullptr, 0, nullptr);
    }
}

void GPUAcceleratedUI::perform_gpu_culling(VkCommandBuffer cmd, const UIRenderParams& params) {
    // GPU-based frustum culling for UI elements
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, impl_->ui_physics_pipeline_);
    
    // Bind buffers and dispatch culling compute shader
    u32 group_count = (impl_->ui_elements_.size() + 63) / 64;
    vkCmdDispatch(cmd, group_count, 1, 1);
    
    // Barrier to ensure culling is complete before rendering
    VkMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER;
    barrier.srcAccessMask = VK_ACCESS_SHADER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_INDIRECT_COMMAND_READ_BIT;
    
    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                       VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT, 0, 1, &barrier, 0, nullptr, 0, nullptr);
}

void GPUAcceleratedUI::render_text(VkCommandBuffer cmd, const UIRenderParams& params) {
    if (impl_->text_elements_.empty()) return;
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, impl_->text_rendering_pipeline_);
    
    // Bind vertex and index buffers
    VkBuffer vertex_buffers[] = {impl_->text_buffer_};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmd, 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(cmd, impl_->index_buffer_, 0, VK_INDEX_TYPE_UINT32);
    
    // Bind font atlas
    // ... descriptor set binding for font atlas ...
    
    // Render text with instanced drawing
    if (impl_->instanced_rendering_) {
        vkCmdDrawIndexed(cmd, impl_->text_glyphs_rendered_.load() * 6, impl_->text_elements_.size(), 0, 0, 0);
    } else {
        // Fallback to individual draw calls
        for (const auto& text_element : impl_->text_elements_) {
            // Individual text rendering
            vkCmdDrawIndexed(cmd, text_element.glyph_count * 6, 1, text_element.index_offset, 0, 0);
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    impl_->text_rendering_time_ms_ = std::chrono::duration<f64, std::milli>(end_time - start_time).count();
    
    impl_->text_glyphs_rendered_ += impl_->text_elements_.size();
    impl_->draw_calls_issued_++;
}

void GPUAcceleratedUI::render_buttons(VkCommandBuffer cmd, const UIRenderParams& params) {
    if (impl_->button_elements_.empty()) return;
    
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, impl_->button_rendering_pipeline_);
    
    // Bind buffers
    VkBuffer vertex_buffers[] = {impl_->vertex_buffer_};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(cmd, 0, 1, vertex_buffers, offsets);
    vkCmdBindIndexBuffer(cmd, impl_->index_buffer_, 0, VK_INDEX_TYPE_UINT32);
    
    // Batch render all buttons
    if (impl_->batch_rendering_) {
        u32 total_indices = 0;
        for (const auto& button : impl_->button_elements_) {
            total_indices += 6; // 2 triangles per button
        }
        vkCmdDrawIndexed(cmd, total_indices, 1, 0, 0, 0);
    } else {
        for (const auto& button : impl_->button_elements_) {
            vkCmdDrawIndexed(cmd, 6, 1, button.index_offset, 0, 0);
        }
    }
    
    impl_->draw_calls_issued_++;
}

void GPUAcceleratedUI::render_panels(VkCommandBuffer cmd, const UIRenderParams& params) {
    if (impl_->panel_elements_.empty()) return;
    
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, impl_->panel_rendering_pipeline_);
    
    // Similar rendering logic for panels
    // ... implementation ...
    
    impl_->draw_calls_issued_++;
}

void GPUAcceleratedUI::render_windows(VkCommandBuffer cmd, const UIRenderParams& params) {
    if (impl_->window_elements_.empty()) return;
    
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, impl_->window_rendering_pipeline_);
    
    // Window rendering with advanced effects
    // ... implementation ...
    
    impl_->draw_calls_issued_++;
}

void GPUAcceleratedUI::render_icons(VkCommandBuffer cmd, const UIRenderParams& params) {
    if (impl_->icon_elements_.empty()) return;
    
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, impl_->icon_rendering_pipeline_);
    
    // Icon rendering with atlas sampling
    // ... implementation ...
    
    impl_->draw_calls_issued_++;
}

void GPUAcceleratedUI::apply_gpu_effects(VkCommandBuffer cmd, const UIRenderParams& params) {
    // Apply blur effect
    if (impl_->config_.enable_blur) {
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, impl_->blur_pipeline_);
        vkCmdDispatch(cmd, (impl_->config_.render_width + 15) / 16, (impl_->config_.render_height + 15) / 16, 1);
    }
    
    // Apply glow effect
    if (impl_->config_.enable_glow) {
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, impl_->glow_pipeline_);
        vkCmdDispatch(cmd, (impl_->config_.render_width + 15) / 16, (impl_->config_.render_height + 15) / 16, 1);
    }
    
    // Apply shadow effect
    if (impl_->config_.enable_shadows) {
        vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, impl_->shadow_pipeline_);
        vkCmdDispatch(cmd, (impl_->config_.render_width + 15) / 16, (impl_->config_.render_height + 15) / 16, 1);
    }
}

// Background update loops
void GPUAcceleratedUI::ui_update_loop() {
    while (impl_->update_thread_active_) {
        // Update UI element states
        update_ui_element_states();
        
        // Update animations
        update_animations();
        
        // Sleep for frame time
        std::this_thread::sleep_for(std::chrono::microseconds(4166)); // ~240 FPS
    }
}

void GPUAcceleratedUI::layout_update_loop() {
    while (impl_->update_thread_active_) {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // Update layouts using GPU compute
        if (impl_->layout_engine_) {
            impl_->layout_engine_->update_layouts(impl_->ui_elements_);
        }
        
        auto end_time = std::chrono::high_resolution_clock::now();
        impl_->layout_time_ms_ = std::chrono::duration<f64, std::milli>(end_time - start_time).count();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 Hz layout updates
    }
}

void GPUAcceleratedUI::animation_update_loop() {
    while (impl_->update_thread_active_) {
        // Update all active animations
        for (auto& animation : impl_->active_animations_) {
            animation.update(1.0 / 240.0); // 240 FPS animation updates
        }
        
        // Remove completed animations
        impl_->active_animations_.erase(
            std::remove_if(impl_->active_animations_.begin(), impl_->active_animations_.end(),
                         [](const UIAnimation& anim) { return anim.is_complete(); }),
            impl_->active_animations_.end());
        
        std::this_thread::sleep_for(std::chrono::microseconds(4166)); // ~240 FPS
    }
}

UIAccelerationStats GPUAcceleratedUI::get_performance_stats() const {
    UIAccelerationStats stats;
    stats.ui_elements_rendered = impl_->ui_elements_rendered_;
    stats.text_glyphs_rendered = impl_->text_glyphs_rendered_;
    stats.draw_calls_issued = impl_->draw_calls_issued_;
    stats.gpu_ui_time_ms = impl_->gpu_ui_time_ms_;
    stats.layout_time_ms = impl_->layout_time_ms_;
    stats.text_rendering_time_ms = impl_->text_rendering_time_ms_;
    stats.gpu_culling_enabled = impl_->gpu_culling_enabled_;
    stats.gpu_layout_enabled = impl_->gpu_layout_enabled_;
    stats.gpu_text_rendering = impl_->gpu_text_rendering_;
    stats.batch_rendering = impl_->batch_rendering_;
    stats.instanced_rendering = impl_->instanced_rendering_;
    stats.async_compute = impl_->async_compute_;
    return stats;
}

// Cleanup functions
void GPUAcceleratedUI::cleanup_vulkan_objects() {
    if (impl_->command_pool_ != VK_NULL_HANDLE) {
        vkDestroyCommandPool(impl_->device_, impl_->command_pool_, nullptr);
    }
    if (impl_->descriptor_pool_ != VK_NULL_HANDLE) {
        vkDestroyDescriptorPool(impl_->device_, impl_->descriptor_pool_, nullptr);
    }
    if (impl_->ui_render_pass_ != VK_NULL_HANDLE) {
        vkDestroyRenderPass(impl_->device_, impl_->ui_render_pass_, nullptr);
    }
}

// Placeholder implementations for helper functions
bool GPUAcceleratedUI::allocate_render_target_memory() { return true; }
bool GPUAcceleratedUI::create_render_target_views() { return true; }
bool GPUAcceleratedUI::load_ui_shaders() { return true; }
bool GPUAcceleratedUI::create_text_rendering_pipeline() { return true; }
bool GPUAcceleratedUI::create_button_rendering_pipeline() { return true; }
bool GPUAcceleratedUI::create_panel_rendering_pipeline() { return true; }
bool GPUAcceleratedUI::create_compute_pipelines() { return true; }
bool GPUAcceleratedUI::create_font_atlas() { return true; }
bool GPUAcceleratedUI::load_default_fonts() { return true; }
bool GPUAcceleratedUI::setup_glyph_cache() { return true; }
void GPUAcceleratedUI::cleanup_subsystems() {}
void GPUAcceleratedUI::cleanup_pipelines() {}
void GPUAcceleratedUI::cleanup_buffers() {}
void GPUAcceleratedUI::cleanup_render_targets() {}
void GPUAcceleratedUI::update_ui_element_states() {}
void GPUAcceleratedUI::update_animations() {}

} // namespace SU1
