#include "su1/display_manager.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>
#include <chrono>

namespace su1 {

Display::Display(const DisplayInfo& info)
    : info_(info)
    , initialized_(false) {
    std::cout << "[DISPLAY] Display created: " << info_.name << " (" << info_.width << "x" << info_.height << ")" << std::endl;
}

Display::~Display() {
    if (initialized_) {
        shutdown();
    }
}

bool Display::initialize() {
    std::cout << "[DISPLAY] Initializing display: " << info_.name << std::endl;

    initialized_ = true;

    std::cout << "[DISPLAY] Display initialized successfully" << std::endl;
    std::cout << "[DISPLAY] Resolution: " << info_.width << "x" << info_.height << std::endl;
    std::cout << "[DISPLAY] Refresh rate: " << info_.refresh_rate << "Hz" << std::endl;

    return true;
}

void Display::shutdown() {
    if (initialized_) {
        std::cout << "[DISPLAY] Shutting down display: " << info_.name << std::endl;
        initialized_ = false;
        std::cout << "[DISPLAY] Display shutdown complete" << std::endl;
    }
}

bool Display::is_initialized() const {
    return initialized_;
}

const DisplayInfo& Display::get_info() const {
    return info_;
}

bool Display::set_mode(u32 width, u32 height, u32 refresh_rate) {
    info_.width = width;
    info_.height = height;
    info_.refresh_rate = refresh_rate;

    std::cout << "[DISPLAY] Mode set to: " << width << "x" << height << "@" << refresh_rate << "Hz" << std::endl;

    return true;
}

DisplayManager::DisplayManager()
    : initialized_(false) {
    std::cout << "[DISPLAY_MANAGER] DisplayManager created" << std::endl;
}

DisplayManager::~DisplayManager() {
    if (initialized_) {
        shutdown();
    }
}

bool DisplayManager::initialize() {
    std::cout << "[DISPLAY_MANAGER] Initializing display manager..." << std::endl;

    // Simulate display detection
    DisplayInfo primary_display;
    primary_display.name = "Primary Display";
    primary_display.width = 1920;
    primary_display.height = 1080;
    primary_display.refresh_rate = 60;
    primary_display.type = DisplayType::Monitor;
    primary_display.connected = true;

    displays_.push_back(std::make_unique<Display>(primary_display));
    displays_[0]->initialize();

    initialized_ = true;

    std::cout << "[DISPLAY_MANAGER] Display manager initialized successfully" << std::endl;
    std::cout << "[DISPLAY_MANAGER] Found " << displays_.size() << " display(s)" << std::endl;

    return true;
}

void DisplayManager::shutdown() {
    if (initialized_) {
        std::cout << "[DISPLAY_MANAGER] Shutting down display manager..." << std::endl;

        displays_.clear();

        initialized_ = false;
        std::cout << "[DISPLAY_MANAGER] Display manager shutdown complete" << std::endl;
    }
}

bool DisplayManager::is_initialized() const {
    return initialized_;
}

size_t DisplayManager::get_display_count() const {
    return displays_.size();
}

Display* DisplayManager::get_display(size_t index) {
    if (index < displays_.size()) {
        return displays_[index].get();
    }
    return nullptr;
}

Display* DisplayManager::get_primary_display() {
    return displays_.empty() ? nullptr : displays_[0].get();
}

bool DisplayManager::set_display_mode(size_t display_index, u32 width, u32 height, u32 refresh_rate) {
    Display* display = get_display(display_index);
    if (display) {
        return display->set_mode(width, height, refresh_rate);
    }
    return false;
}

} // namespace su1

Display::~Display() {
    SU1_PROFILE_FUNCTION();
    shutdown();
}

bool Display::initialize() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (initialized_) {
        return true;
    }
    
    SU1_LOG_INFO("Initializing display {}: {}", info_.id, info_.name);
    
    // Setup DRM
    if (!setup_drm()) {
        SU1_LOG_ERROR("Failed to setup DRM for display {}", info_.id);
        return false;
    }
    
    // Setup GBM
    if (!setup_gbm()) {
        SU1_LOG_ERROR("Failed to setup GBM for display {}", info_.id);
        cleanup_drm();
        return false;
    }
    
    // Setup EGL
    if (!setup_egl()) {
        SU1_LOG_ERROR("Failed to setup EGL for display {}", info_.id);
        cleanup_gbm();
        cleanup_drm();
        return false;
    }
    
    // Setup OpenGL
    setup_opengl();
    
    // Initialize gamma ramps
    usize gamma_size = 256;
    gamma_red_.resize(gamma_size);
    gamma_green_.resize(gamma_size);
    gamma_blue_.resize(gamma_size);
    
    for (usize i = 0; i < gamma_size; ++i) {
        f32 value = static_cast<f32>(i) / static_cast<f32>(gamma_size - 1);
        u16 gamma_value = static_cast<u16>(value * 65535.0f);
        gamma_red_[i] = gamma_value;
        gamma_green_[i] = gamma_value;
        gamma_blue_[i] = gamma_value;
    }
    
    initialized_ = true;
    SU1_LOG_INFO("Display {} initialized successfully", info_.id);
    return true;
}

void Display::shutdown() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (!initialized_) {
        return;
    }
    
    SU1_LOG_INFO("Shutting down display {}", info_.id);
    
    cleanup_opengl();
    cleanup_egl();
    cleanup_gbm();
    cleanup_drm();
    
    initialized_ = false;
    SU1_LOG_INFO("Display {} shut down", info_.id);
}

bool Display::connect() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (!initialized_) {
        SU1_LOG_ERROR("Display {} not initialized", info_.id);
        return false;
    }
    
    if (info_.is_connected) {
        return true;
    }
    
    SU1_LOG_INFO("Connecting display {}", info_.id);
    
    // Update connection status
    info_.is_connected = true;
    info_.connection_time = Timer::get_current_time();
    
    SU1_LOG_INFO("Display {} connected", info_.id);
    return true;
}

void Display::disconnect() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (!info_.is_connected) {
        return;
    }
    
    SU1_LOG_INFO("Disconnecting display {}", info_.id);
    
    disable();
    info_.is_connected = false;
    
    SU1_LOG_INFO("Display {} disconnected", info_.id);
}

bool Display::enable() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (!info_.is_connected) {
        SU1_LOG_ERROR("Cannot enable disconnected display {}", info_.id);
        return false;
    }
    
    if (info_.is_enabled) {
        return true;
    }
    
    SU1_LOG_INFO("Enabling display {}", info_.id);
    
    // Set CRTC mode
    if (!set_crtc_mode()) {
        SU1_LOG_ERROR("Failed to set CRTC mode for display {}", info_.id);
        return false;
    }
    
    info_.is_enabled = true;
    SU1_LOG_INFO("Display {} enabled", info_.id);
    return true;
}

void Display::disable() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (!info_.is_enabled) {
        return;
    }
    
    SU1_LOG_INFO("Disabling display {}", info_.id);
    
    // Disable CRTC
    if (drm_fd_ >= 0 && crtc_) {
        drmModeSetCrtc(drm_fd_, crtc_->crtc_id, 0, 0, 0, nullptr, 0, nullptr);
    }
    
    info_.is_enabled = false;
    SU1_LOG_INFO("Display {} disabled", info_.id);
}

bool Display::set_mode(const DisplayMode& mode) {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (!is_mode_supported(mode)) {
        SU1_LOG_ERROR("Display mode {}x{}@{}Hz not supported on display {}", 
                     mode.width, mode.height, mode.refresh_rate, info_.id);
        return false;
    }
    
    SU1_LOG_INFO("Setting display mode {}x{}@{}Hz on display {}", 
                mode.width, mode.height, mode.refresh_rate, info_.id);
    
    info_.current_mode = mode;
    
    if (info_.is_enabled) {
        return set_crtc_mode();
    }
    
    return true;
}

void Display::start_frame() {
    SU1_PROFILE_FUNCTION();
    
    frame_timer_.start();
    
    if (egl_display_ != EGL_NO_DISPLAY && egl_context_ != EGL_NO_CONTEXT) {
        eglMakeCurrent(egl_display_, egl_surface_, egl_surface_, egl_context_);
    }
    
    // Clear the framebuffer
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Display::end_frame() {
    SU1_PROFILE_FUNCTION();
    
    frame_timer_.stop();
    f64 frame_time = frame_timer_.get_elapsed_seconds();
    
    total_frame_time_ += frame_time;
    frame_count_++;
    average_frame_time_ = total_frame_time_ / static_cast<f64>(frame_count_);
    
    if (frame_time > peak_frame_time_) {
        peak_frame_time_ = frame_time;
    }
    
    info_.frame_count = frame_count_;
    info_.average_frame_time = average_frame_time_;
    info_.peak_frame_time = peak_frame_time_;
    info_.total_frame_time = total_frame_time_;
}

void Display::present() {
    SU1_PROFILE_FUNCTION();
    
    if (egl_display_ != EGL_NO_DISPLAY && egl_surface_ != EGL_NO_SURFACE) {
        if (vsync_enabled_.load()) {
            eglSwapInterval(egl_display_, 1);
        } else {
            eglSwapInterval(egl_display_, 0);
        }
        
        eglSwapBuffers(egl_display_, egl_surface_);
    }
}

void Display::apply_liquid_glass_effect() {
    SU1_PROFILE_FUNCTION();
    
    // Render liquid surface with glass material properties
    render_liquid_surface();
    render_glass_material();
    
    // Apply advanced lighting and reflections
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Enable multisampling for smooth edges
    glEnable(GL_MULTISAMPLE);
    
    // Apply depth testing for 3D effects
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

void Display::apply_holographic_projection() {
    SU1_PROFILE_FUNCTION();
    
    render_holographic_interference();
    
    // Apply interference patterns
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_COLOR, GL_DST_COLOR);
    
    // Enable complex number calculations in shaders
    // (This would require custom shaders)
}

void Display::setup_drm() {
    SU1_PROFILE_FUNCTION();
    
    // Open DRM device
    drm_fd_ = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (drm_fd_ < 0) {
        SU1_LOG_ERROR("Failed to open DRM device");
        return false;
    }
    
    // Find connector, encoder, and CRTC
    if (!find_connector() || !find_encoder() || !find_crtc()) {
        close(drm_fd_);
        drm_fd_ = -1;
        return false;
    }
    
    return true;
}

void Display::cleanup_drm() {
    SU1_PROFILE_FUNCTION();
    
    if (connector_) {
        drmModeFreeConnector(connector_);
        connector_ = nullptr;
    }
    
    if (encoder_) {
        drmModeFreeEncoder(encoder_);
        encoder_ = nullptr;
    }
    
    if (crtc_) {
        drmModeFreeCrtc(crtc_);
        crtc_ = nullptr;
    }
    
    if (drm_fd_ >= 0) {
        close(drm_fd_);
        drm_fd_ = -1;
    }
}

bool Display::setup_gbm() {
    SU1_PROFILE_FUNCTION();
    
    gbm_device_ = gbm_create_device(drm_fd_);
    if (!gbm_device_) {
        SU1_LOG_ERROR("Failed to create GBM device");
        return false;
    }
    
    gbm_surface_ = gbm_surface_create(gbm_device_,
                                     info_.current_mode.width,
                                     info_.current_mode.height,
                                     GBM_FORMAT_XRGB8888,
                                     GBM_BO_USE_SCANOUT | GBM_BO_USE_RENDERING);
    if (!gbm_surface_) {
        SU1_LOG_ERROR("Failed to create GBM surface");
        gbm_device_destroy(gbm_device_);
        gbm_device_ = nullptr;
        return false;
    }
    
    return true;
}

void Display::cleanup_gbm() {
    SU1_PROFILE_FUNCTION();
    
    if (gbm_surface_) {
        gbm_surface_destroy(gbm_surface_);
        gbm_surface_ = nullptr;
    }
    
    if (gbm_device_) {
        gbm_device_destroy(gbm_device_);
        gbm_device_ = nullptr;
    }
}

bool Display::setup_egl() {
    SU1_PROFILE_FUNCTION();
    
    egl_display_ = eglGetPlatformDisplay(EGL_PLATFORM_GBM_MESA, gbm_device_, nullptr);
    if (egl_display_ == EGL_NO_DISPLAY) {
        SU1_LOG_ERROR("Failed to get EGL display");
        return false;
    }
    
    if (!eglInitialize(egl_display_, nullptr, nullptr)) {
        SU1_LOG_ERROR("Failed to initialize EGL");
        return false;
    }
    
    EGLConfig config;
    EGLint num_configs;
    EGLint config_attribs[] = {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_SAMPLES, 4,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    
    if (!eglChooseConfig(egl_display_, config_attribs, &config, 1, &num_configs) || num_configs == 0) {
        SU1_LOG_ERROR("Failed to choose EGL config");
        eglTerminate(egl_display_);
        egl_display_ = EGL_NO_DISPLAY;
        return false;
    }
    
    EGLint context_attribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    
    egl_context_ = eglCreateContext(egl_display_, config, EGL_NO_CONTEXT, context_attribs);
    if (egl_context_ == EGL_NO_CONTEXT) {
        SU1_LOG_ERROR("Failed to create EGL context");
        eglTerminate(egl_display_);
        egl_display_ = EGL_NO_DISPLAY;
        return false;
    }
    
    egl_surface_ = eglCreateWindowSurface(egl_display_, config, 
                                         reinterpret_cast<EGLNativeWindowType>(gbm_surface_), nullptr);
    if (egl_surface_ == EGL_NO_SURFACE) {
        SU1_LOG_ERROR("Failed to create EGL surface");
        eglDestroyContext(egl_display_, egl_context_);
        egl_context_ = EGL_NO_CONTEXT;
        eglTerminate(egl_display_);
        egl_display_ = EGL_NO_DISPLAY;
        return false;
    }
    
    return true;
}

void Display::cleanup_egl() {
    SU1_PROFILE_FUNCTION();
    
    if (egl_surface_ != EGL_NO_SURFACE) {
        eglDestroySurface(egl_display_, egl_surface_);
        egl_surface_ = EGL_NO_SURFACE;
    }
    
    if (egl_context_ != EGL_NO_CONTEXT) {
        eglDestroyContext(egl_display_, egl_context_);
        egl_context_ = EGL_NO_CONTEXT;
    }
    
    if (egl_display_ != EGL_NO_DISPLAY) {
        eglTerminate(egl_display_);
        egl_display_ = EGL_NO_DISPLAY;
    }
}

void Display::setup_opengl() {
    SU1_PROFILE_FUNCTION();
    
    if (egl_display_ != EGL_NO_DISPLAY && egl_context_ != EGL_NO_CONTEXT) {
        eglMakeCurrent(egl_display_, egl_surface_, egl_surface_, egl_context_);
        
        // Enable advanced OpenGL features
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_TEXTURE_2D);
        
        // Set initial viewport
        glViewport(0, 0, info_.current_mode.width, info_.current_mode.height);
        
        SU1_LOG_INFO("OpenGL initialized for display {}", info_.id);
        SU1_LOG_INFO("  Vendor: {}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
        SU1_LOG_INFO("  Renderer: {}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
        SU1_LOG_INFO("  Version: {}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    }
}

void Display::cleanup_opengl() {
    SU1_PROFILE_FUNCTION();
    
    // Cleanup is handled by EGL cleanup
}

void Display::render_liquid_surface() {
    SU1_PROFILE_FUNCTION();
    
    // Advanced liquid surface rendering with fluid dynamics
    // This would require complex shaders and physics simulation
    
    // For now, implement basic ripple effects
    f32 time = static_cast<f32>(Timer::get_current_time());
    
    // Generate ripple patterns
    for (u32 y = 0; y < info_.current_mode.height; y += 4) {
        for (u32 x = 0; x < info_.current_mode.width; x += 4) {
            f32 dx = static_cast<f32>(x) - static_cast<f32>(info_.current_mode.width) * 0.5f;
            f32 dy = static_cast<f32>(y) - static_cast<f32>(info_.current_mode.height) * 0.5f;
            f32 distance = std::sqrt(dx * dx + dy * dy);
            f32 wave = std::sin(distance * 0.02f - time * 2.0f) * 0.1f + 0.9f;
            
            // Apply wave distortion (this would be done in a shader in practice)
            glColor4f(wave, wave, wave, 0.8f);
            glBegin(GL_QUADS);
            glVertex2f(static_cast<f32>(x), static_cast<f32>(y));
            glVertex2f(static_cast<f32>(x + 4), static_cast<f32>(y));
            glVertex2f(static_cast<f32>(x + 4), static_cast<f32>(y + 4));
            glVertex2f(static_cast<f32>(x), static_cast<f32>(y + 4));
            glEnd();
        }
    }
}

void Display::render_glass_material() {
    SU1_PROFILE_FUNCTION();
    
    // Render glass material with refraction and reflection
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Apply glass transparency
    glColor4f(1.0f, 1.0f, 1.0f, 0.7f);
    
    // Render glass surface
    glBegin(GL_QUADS);
    glVertex2f(0.0f, 0.0f);
    glVertex2f(static_cast<f32>(info_.current_mode.width), 0.0f);
    glVertex2f(static_cast<f32>(info_.current_mode.width), static_cast<f32>(info_.current_mode.height));
    glVertex2f(0.0f, static_cast<f32>(info_.current_mode.height));
    glEnd();
}

// DisplayManager implementation

DisplayManager::DisplayManager()
    : primary_display_id_(0)
    , initialized_(false)
    , monitoring_enabled_(false) {
    SU1_PROFILE_FUNCTION();
}

DisplayManager::~DisplayManager() {
    SU1_PROFILE_FUNCTION();
    shutdown();
}

bool DisplayManager::initialize() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (initialized_) {
        return true;
    }
    
    SU1_LOG_INFO("Initializing display manager");
    
    // Scan for displays
    scan_displays();
    
    // Start monitoring thread
    monitoring_enabled_.store(true);
    monitor_thread_ = Thread([this]() { monitor_displays(); });
    
    initialized_ = true;
    SU1_LOG_INFO("Display manager initialized with {} displays", displays_.size());
    
    return true;
}

void DisplayManager::shutdown() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (!initialized_) {
        return;
    }
    
    SU1_LOG_INFO("Shutting down display manager");
    
    // Stop monitoring
    monitoring_enabled_.store(false);
    if (monitor_thread_.joinable()) {
        monitor_thread_.join();
    }
    
    // Shutdown all displays
    for (auto& display : displays_) {
        display->shutdown();
    }
    
    displays_.clear();
    display_map_.clear();
    
    initialized_ = false;
    SU1_LOG_INFO("Display manager shut down");
}

void DisplayManager::scan_displays() {
    SU1_PROFILE_FUNCTION();
    
    SU1_LOG_INFO("Scanning for displays");
    
    int drm_fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (drm_fd < 0) {
        SU1_LOG_ERROR("Failed to open DRM device for scanning");
        return;
    }
    
    drmModeRes* resources = drmModeGetResources(drm_fd);
    if (!resources) {
        SU1_LOG_ERROR("Failed to get DRM resources");
        close(drm_fd);
        return;
    }
    
    for (int i = 0; i < resources->count_connectors; ++i) {
        drmModeConnector* connector = drmModeGetConnector(drm_fd, resources->connectors[i]);
        if (!connector) {
            continue;
        }
        
        if (connector->connection == DRM_MODE_CONNECTED) {
            DisplayPtr display = create_display_from_connector(connector);
            if (display) {
                add_display(display);
                SU1_LOG_INFO("Found display: {} ({}x{})", 
                           display->get_info().name,
                           display->get_info().current_mode.width,
                           display->get_info().current_mode.height);
            }
        }
        
        drmModeFreeConnector(connector);
    }
    
    drmModeFreeResources(resources);
    close(drm_fd);
    
    // Set first display as primary if none set
    if (primary_display_id_ == 0 && !displays_.empty()) {
        set_primary_display(displays_[0]->get_id());
    }
    
    SU1_LOG_INFO("Display scan complete, found {} displays", displays_.size());
}

DisplayPtr DisplayManager::create_display_from_connector(drmModeConnector* connector) {
    SU1_PROFILE_FUNCTION();
    
    DisplayInfo info = {};
    info.id = connector->connector_id;
    info.name = String("Display-") + std::to_string(connector->connector_id);
    info.connector_type = String("Unknown");
    info.is_connected = (connector->connection == DRM_MODE_CONNECTED);
    info.is_enabled = false;
    info.is_primary = false;
    
    // Set default mode if available
    if (connector->count_modes > 0) {
        drmModeModeInfo* mode_info = &connector->modes[0];
        info.current_mode.width = mode_info->hdisplay;
        info.current_mode.height = mode_info->vdisplay;
        info.current_mode.refresh_rate = mode_info->vrefresh;
        info.current_mode.name = String(mode_info->name);
        info.current_mode.is_preferred = (mode_info->type & DRM_MODE_TYPE_PREFERRED) != 0;
        info.preferred_mode = info.current_mode;
        
        // Populate all modes
        for (int i = 0; i < connector->count_modes; ++i) {
            drmModeModeInfo* mode = &connector->modes[i];
            DisplayMode display_mode = {};
            display_mode.width = mode->hdisplay;
            display_mode.height = mode->vdisplay;
            display_mode.refresh_rate = mode->vrefresh;
            display_mode.name = String(mode->name);
            display_mode.is_preferred = (mode->type & DRM_MODE_TYPE_PREFERRED) != 0;
            display_mode.is_supported = true;
            info.modes.push_back(display_mode);
        }
    }
    
    return MakeShared<Display>(info);
}

bool DisplayManager::add_display(DisplayPtr display) {
    SU1_PROFILE_FUNCTION();
    
    if (!display) {
        return false;
    }
    
    u32 id = display->get_id();
    
    // Check if display already exists
    if (display_map_.find(id) != display_map_.end()) {
        return false;
    }
    
    displays_.push_back(display);
    display_map_[id] = display;
    
    // Initialize the display
    display->initialize();
    
    SU1_LOG_INFO("Added display {} to manager", id);
    
    // Emit signal
    display_connected.emit(display);
    
    return true;
}

DisplayPtr DisplayManager::get_display(u32 id) const {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    auto it = display_map_.find(id);
    if (it != display_map_.end()) {
        return it->second;
    }
    
    return nullptr;
}

DisplayPtr DisplayManager::get_primary_display() const {
    SU1_PROFILE_FUNCTION();
    return get_display(primary_display_id_);
}

Vector<DisplayPtr> DisplayManager::get_all_displays() const {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    return displays_;
}

Vector<DisplayPtr> DisplayManager::get_connected_displays() const {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    Vector<DisplayPtr> connected;
    for (const auto& display : displays_) {
        if (display->is_connected()) {
            connected.push_back(display);
        }
    }
    
    return connected;
}

Vector<DisplayPtr> DisplayManager::get_enabled_displays() const {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    Vector<DisplayPtr> enabled;
    for (const auto& display : displays_) {
        if (display->is_enabled()) {
            enabled.push_back(display);
        }
    }
    
    return enabled;
}

bool DisplayManager::set_primary_display(u32 id) {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    DisplayPtr display = get_display(id);
    if (!display || !display->is_connected()) {
        return false;
    }
    
    // Update old primary
    if (primary_display_id_ != 0) {
        DisplayPtr old_primary = get_display(primary_display_id_);
        if (old_primary) {
            old_primary->get_info().is_primary = false;
        }
    }
    
    // Set new primary
    primary_display_id_ = id;
    display->get_info().is_primary = true;
    
    SU1_LOG_INFO("Set display {} as primary", id);
    
    // Emit signal
    primary_display_changed.emit(display);
    
    return true;
}

void DisplayManager::monitor_displays() {
    SU1_PROFILE_FUNCTION();
    
    SU1_LOG_INFO("Display monitoring thread started");
    
    while (monitoring_enabled_.load()) {
        // Check for display hotplug events
        handle_display_hotplug();
        
        // Update display information
        update_displays();
        
        // Sleep for a short interval
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    SU1_LOG_INFO("Display monitoring thread stopped");
}

void DisplayManager::handle_display_hotplug() {
    SU1_PROFILE_FUNCTION();
    
    // This would typically involve listening to udev events
    // For now, we'll just rescan periodically
    static auto last_scan = std::chrono::steady_clock::now();
    auto now = std::chrono::steady_clock::now();
    
    if (now - last_scan > std::chrono::seconds(5)) {
        scan_displays();
        last_scan = now;
    }
}

void DisplayManager::update_displays() {
    SU1_PROFILE_FUNCTION();
    
    for (auto& display : displays_) {
        display->update_info();
    }
}

} // namespace su1
