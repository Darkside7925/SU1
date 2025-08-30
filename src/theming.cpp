#include "su1/theming.hpp"
#include "su1/logging.hpp"
#include "su1/profiler.hpp"
#include <algorithm>
#include <cmath>
#include <fstream>

namespace su1 {

// ColorPalette implementation
ColorPalette::ColorPalette()
    : primary{0.2f, 0.6f, 1.0f, 1.0f}
    , secondary{0.6f, 0.2f, 1.0f, 1.0f}
    , accent{1.0f, 0.4f, 0.2f, 1.0f}
    , background{0.1f, 0.1f, 0.1f, 1.0f}
    , surface{0.15f, 0.15f, 0.15f, 1.0f}
    , error{1.0f, 0.3f, 0.3f, 1.0f}
    , warning{1.0f, 0.7f, 0.2f, 1.0f}
    , info{0.3f, 0.7f, 1.0f, 1.0f}
    , success{0.3f, 0.8f, 0.3f, 1.0f}
    , text_primary{1.0f, 1.0f, 1.0f, 1.0f}
    , text_secondary{0.7f, 0.7f, 0.7f, 1.0f}
    , text_disabled{0.5f, 0.5f, 0.5f, 1.0f}
    , divider{0.3f, 0.3f, 0.3f, 1.0f}
    , outline{0.4f, 0.4f, 0.4f, 1.0f}
    , shadow{0.0f, 0.0f, 0.0f, 0.5f}
    , highlight{1.0f, 1.0f, 1.0f, 0.3f}
    , selection{0.2f, 0.6f, 1.0f, 0.8f}
    , focus{0.4f, 0.8f, 1.0f, 1.0f}
    , hover{0.3f, 0.3f, 0.3f, 1.0f}
    , pressed{0.1f, 0.1f, 0.1f, 1.0f}
    , disabled{0.5f, 0.5f, 0.5f, 0.5f}
    , link{0.3f, 0.7f, 1.0f, 1.0f}
    , visited{0.7f, 0.3f, 1.0f, 1.0f}
    , active{0.2f, 0.8f, 0.3f, 1.0f}
    , inactive{0.6f, 0.6f, 0.6f, 1.0f} {
}

void ColorPalette::apply_color_scheme(ColorScheme scheme) {
    switch (scheme) {
        case ColorScheme::Monochrome:
            // Convert all colors to grayscale
            primary = Color{0.5f, 0.5f, 0.5f, primary.a};
            secondary = Color{0.4f, 0.4f, 0.4f, secondary.a};
            accent = Color{0.7f, 0.7f, 0.7f, accent.a};
            break;
            
        case ColorScheme::HighContrast:
            primary = Color{1.0f, 1.0f, 1.0f, 1.0f};
            secondary = Color{0.0f, 0.0f, 0.0f, 1.0f};
            background = Color{0.0f, 0.0f, 0.0f, 1.0f};
            text_primary = Color{1.0f, 1.0f, 1.0f, 1.0f};
            break;
            
        case ColorScheme::Warm:
            primary = Color{1.0f, 0.5f, 0.2f, 1.0f};
            secondary = Color{1.0f, 0.7f, 0.3f, 1.0f};
            accent = Color{1.0f, 0.3f, 0.1f, 1.0f};
            break;
            
        case ColorScheme::Cool:
            primary = Color{0.2f, 0.5f, 1.0f, 1.0f};
            secondary = Color{0.3f, 0.7f, 1.0f, 1.0f};
            accent = Color{0.1f, 0.3f, 1.0f, 1.0f};
            break;
            
        default:
            break;
    }
}

void ColorPalette::apply_liquid_glass_palette() {
    background = Color{0.1f, 0.1f, 0.15f, 0.7f};
    surface = Color{0.15f, 0.15f, 0.2f, 0.8f};
    primary = Color{0.6f, 0.8f, 1.0f, 0.9f};
    secondary = Color{0.8f, 0.6f, 1.0f, 0.8f};
    accent = Color{1.0f, 0.8f, 0.6f, 0.9f};
    
    // Adjust all colors for glass effect
    highlight = Color{1.0f, 1.0f, 1.0f, 0.4f};
    shadow = Color{0.0f, 0.0f, 0.0f, 0.3f};
}

void ColorPalette::apply_holographic_palette() {
    primary = Color{0.8f, 0.9f, 1.0f, 0.8f};
    secondary = Color{1.0f, 0.8f, 0.9f, 0.8f};
    accent = Color{0.9f, 1.0f, 0.8f, 0.8f};
    background = Color{0.05f, 0.05f, 0.1f, 0.9f};
    
    // Add iridescent effects
    gradients = {
        Color{1.0f, 0.0f, 0.0f, 1.0f},
        Color{1.0f, 0.5f, 0.0f, 1.0f},
        Color{1.0f, 1.0f, 0.0f, 1.0f},
        Color{0.0f, 1.0f, 0.0f, 1.0f},
        Color{0.0f, 0.0f, 1.0f, 1.0f},
        Color{0.5f, 0.0f, 1.0f, 1.0f},
        Color{1.0f, 0.0f, 1.0f, 1.0f}
    };
}

void ColorPalette::apply_quantum_palette() {
    // Quantum superposition inspired colors
    primary = Color{0.4f, 0.6f, 0.9f, 0.85f};
    secondary = Color{0.6f, 0.4f, 0.9f, 0.85f};
    accent = Color{0.9f, 0.6f, 0.4f, 0.85f};
    background = Color{0.02f, 0.05f, 0.1f, 0.95f};
    
    // Uncertainty principle colors
    highlight = Color{0.8f, 0.9f, 1.0f, 0.6f};
    glow = Color{0.6f, 0.8f, 1.0f, 0.4f};
}

void ColorPalette::apply_consciousness_palette() {
    // Consciousness field inspired colors
    primary = Color{0.9f, 0.8f, 1.0f, 0.9f};
    secondary = Color{0.8f, 0.9f, 1.0f, 0.9f};
    accent = Color{1.0f, 0.9f, 0.8f, 0.9f};
    background = Color{0.05f, 0.02f, 0.08f, 0.95f};
    
    // Awareness spectrum
    spectrum = {
        Color{0.5f, 0.0f, 0.5f, 1.0f}, // Deep awareness
        Color{0.7f, 0.3f, 0.7f, 1.0f}, // Growing consciousness
        Color{0.9f, 0.6f, 0.9f, 1.0f}, // Expanding awareness
        Color{1.0f, 0.8f, 1.0f, 1.0f}, // Pure consciousness
        Color{1.0f, 1.0f, 1.0f, 1.0f}  // Enlightenment
    };
}

void ColorPalette::apply_transcendent_palette() {
    // Beyond ordinary perception
    primary = Color{1.0f, 1.0f, 1.0f, 0.9f};
    secondary = Color{0.95f, 0.98f, 1.0f, 0.9f};
    accent = Color{1.0f, 0.98f, 0.95f, 0.9f};
    background = Color{0.01f, 0.01f, 0.02f, 0.98f};
    
    // Pure light spectrum
    highlight = Color{1.0f, 1.0f, 1.0f, 0.8f};
    glow = Color{1.0f, 1.0f, 1.0f, 0.5f};
}

void ColorPalette::apply_ultimate_palette() {
    // Perfect combination of all themes
    apply_liquid_glass_palette();
    
    // Add holographic elements
    gradients = {
        Color{1.0f, 0.8f, 0.9f, 0.8f},
        Color{0.9f, 0.9f, 1.0f, 0.8f},
        Color{0.8f, 1.0f, 0.9f, 0.8f},
        Color{1.0f, 1.0f, 0.8f, 0.8f}
    };
    
    // Quantum consciousness overlay
    primary = Color{0.9f, 0.9f, 1.0f, 0.85f};
    accent = Color{1.0f, 0.9f, 0.9f, 0.85f};
    
    // Transcendent purity
    highlight = Color{1.0f, 1.0f, 1.0f, 0.6f};
}

Color ColorPalette::get_color(const String& name) const {
    auto it = custom_colors.find(name);
    if (it != custom_colors.end()) {
        return it->second;
    }
    
    // Return primary as default
    return primary;
}

void ColorPalette::set_color(const String& name, const Color& color) {
    custom_colors[name] = color;
}

// Typography implementation
Typography::Typography()
    : primary_font("Arial")
    , secondary_font("Helvetica")
    , monospace_font("Consolas")
    , display_font("Arial")
    , body_font("Arial")
    , caption_font("Arial")
    , scale_factor(1.0f)
    , base_size(14.0f)
    , line_height_ratio(1.4f)
    , letter_spacing(0.0f)
    , word_spacing(0.0f) {
    
    // Initialize font sizes
    font_sizes["tiny"] = 10.0f;
    font_sizes["small"] = 12.0f;
    font_sizes["medium"] = 14.0f;
    font_sizes["large"] = 16.0f;
    font_sizes["huge"] = 18.0f;
    font_sizes["display"] = 24.0f;
    font_sizes["title"] = 20.0f;
    font_sizes["heading"] = 18.0f;
    font_sizes["body"] = 14.0f;
    font_sizes["caption"] = 12.0f;
    
    // Initialize font weights
    font_weights["thin"] = 100.0f;
    font_weights["light"] = 300.0f;
    font_weights["normal"] = 400.0f;
    font_weights["medium"] = 500.0f;
    font_weights["bold"] = 700.0f;
    font_weights["black"] = 900.0f;
}

f32 Typography::get_font_size(const String& variant) const {
    auto it = font_sizes.find(variant);
    return (it != font_sizes.end()) ? it->second * scale_factor : base_size * scale_factor;
}

f32 Typography::get_font_weight(const String& variant) const {
    auto it = font_weights.find(variant);
    return (it != font_weights.end()) ? it->second : 400.0f;
}

void Typography::apply_liquid_glass_typography() {
    // Softer, more translucent feeling
    letter_spacing = 0.5f;
    line_height_ratio = 1.5f;
    
    // Slightly larger for better readability through glass
    scale_factor = 1.1f;
}

void Typography::apply_holographic_typography() {
    // Futuristic, glowing text
    letter_spacing = 1.0f;
    line_height_ratio = 1.6f;
    
    // Monospace for tech feel
    primary_font = "JetBrains Mono";
    body_font = "JetBrains Mono";
}

void Typography::apply_consciousness_typography() {
    // Flowing, organic text
    letter_spacing = 0.3f;
    line_height_ratio = 1.8f;
    
    // Serif fonts for wisdom
    primary_font = "Times New Roman";
    body_font = "Georgia";
}

void Typography::apply_ultimate_typography() {
    // Perfect combination
    apply_liquid_glass_typography();
    letter_spacing = 0.8f;
    line_height_ratio = 1.7f;
    scale_factor = 1.15f;
}

// Spacing implementation
Spacing::Spacing()
    : base_unit(8.0f)
    , density_factor(1.0f) {
    
    // Initialize margins
    margins["tiny"] = 2.0f;
    margins["small"] = 4.0f;
    margins["medium"] = 8.0f;
    margins["large"] = 16.0f;
    margins["huge"] = 32.0f;
    
    // Initialize paddings
    paddings["tiny"] = 2.0f;
    paddings["small"] = 4.0f;
    paddings["medium"] = 8.0f;
    paddings["large"] = 12.0f;
    paddings["huge"] = 24.0f;
    
    // Initialize gaps
    gaps["tiny"] = 2.0f;
    gaps["small"] = 4.0f;
    gaps["medium"] = 8.0f;
    gaps["large"] = 16.0f;
    gaps["huge"] = 24.0f;
}

f32 Spacing::get_margin(const String& variant) const {
    auto it = margins.find(variant);
    return (it != margins.end()) ? it->second * density_factor : base_unit * density_factor;
}

f32 Spacing::get_padding(const String& variant) const {
    auto it = paddings.find(variant);
    return (it != paddings.end()) ? it->second * density_factor : base_unit * density_factor;
}

void Spacing::apply_liquid_glass_spacing() {
    // More generous spacing for glass effect
    density_factor = 1.2f;
}

void Spacing::apply_consciousness_spacing() {
    // Flowing, organic spacing
    density_factor = 1.4f;
}

void Spacing::apply_ultimate_spacing() {
    // Perfect golden ratio spacing
    density_factor = 1.618f;
}

// Effects implementation
Effects::Effects()
    : shadows_enabled(true)
    , blur_enabled(false)
    , glow_enabled(false)
    , transparency_enabled(true)
    , gradients_enabled(true)
    , animations_enabled(true)
    , particles_enabled(false)
    , distortions_enabled(false)
    , shadow_intensity(0.5f)
    , blur_intensity(0.0f)
    , glow_intensity(0.0f)
    , transparency_level(1.0f)
    , animation_speed(1.0f)
    , particle_density(0.0f)
    , distortion_amount(0.0f) {
}

void Effects::apply_liquid_glass_effects() {
    blur_enabled = true;
    transparency_enabled = true;
    
    blur_intensity = 0.6f;
    transparency_level = 0.8f;
    shadow_intensity = 0.3f;
}

void Effects::apply_holographic_effects() {
    glow_enabled = true;
    gradients_enabled = true;
    distortions_enabled = true;
    
    glow_intensity = 0.8f;
    distortion_amount = 0.2f;
}

void Effects::apply_quantum_effects() {
    particles_enabled = true;
    distortions_enabled = true;
    
    particle_density = 0.3f;
    distortion_amount = 0.1f;
}

void Effects::apply_consciousness_effects() {
    glow_enabled = true;
    particles_enabled = true;
    
    glow_intensity = 0.6f;
    particle_density = 0.2f;
}

void Effects::apply_ultimate_effects() {
    enable_all_effects();
    
    blur_intensity = 0.4f;
    glow_intensity = 0.7f;
    transparency_level = 0.85f;
    particle_density = 0.25f;
    distortion_amount = 0.15f;
    shadow_intensity = 0.4f;
}

void Effects::enable_all_effects() {
    shadows_enabled = true;
    blur_enabled = true;
    glow_enabled = true;
    transparency_enabled = true;
    gradients_enabled = true;
    animations_enabled = true;
    particles_enabled = true;
    distortions_enabled = true;
}

// Theme implementation
Theme::Theme(ThemeType type)
    : type_(type)
    , name_("Theme")
    , description_("A beautiful theme")
    , author_("SU1")
    , version_("1.0.0") {
    
    switch (type) {
        case ThemeType::Default:
            initialize_default_theme();
            break;
        case ThemeType::Dark:
            initialize_dark_theme();
            break;
        case ThemeType::Light:
            initialize_light_theme();
            break;
        case ThemeType::LiquidGlass:
            initialize_liquid_glass_theme();
            break;
        case ThemeType::Holographic:
            initialize_holographic_theme();
            break;
        case ThemeType::Quantum:
            initialize_quantum_theme();
            break;
        case ThemeType::Consciousness:
            initialize_consciousness_theme();
            break;
        case ThemeType::Transcendent:
            initialize_transcendent_theme();
            break;
        case ThemeType::Ultimate:
            initialize_ultimate_theme();
            break;
        default:
            initialize_default_theme();
            break;
    }
    
    generate_component_styles();
}

Theme::~Theme() = default;

ComponentStyle Theme::create_component_style(ComponentType component_type) const {
    auto it = component_styles_.find(component_type);
    if (it != component_styles_.end()) {
        return it->second;
    }
    
    // Create default style
    ComponentStyle style;
    style.background_color = colors_.surface;
    style.foreground_color = colors_.text_primary;
    style.border_color = colors_.outline;
    style.shadow_color = colors_.shadow;
    style.highlight_color = colors_.highlight;
    style.selection_color = colors_.selection;
    style.focus_color = colors_.focus;
    style.hover_color = colors_.hover;
    style.pressed_color = colors_.pressed;
    
    style.font_family = typography_.primary_font;
    style.font_size = typography_.get_font_size("medium");
    style.font_weight = typography_.get_font_weight("normal");
    
    style.padding_top = spacing_.get_padding("medium");
    style.padding_right = spacing_.get_padding("medium");
    style.padding_bottom = spacing_.get_padding("medium");
    style.padding_left = spacing_.get_padding("medium");
    
    if (effects_.blur_enabled) {
        style.blur_radius = effects_.blur_intensity * 10.0f;
    }
    
    if (effects_.glow_enabled) {
        style.glow_radius = effects_.glow_intensity * 15.0f;
    }
    
    style.opacity = effects_.transparency_level;
    
    return style;
}

void Theme::apply_to_component(ComponentPtr component) const {
    if (!component) return;
    
    ComponentStyle style = create_component_style(component->get_type());
    component->set_style(style);
}

void Theme::apply_to_component_recursive(ComponentPtr component) const {
    if (!component) return;
    
    apply_to_component(component);
    
    for (const auto& child : component->get_children()) {
        apply_to_component_recursive(child);
    }
}

void Theme::apply_liquid_glass_theme() {
    name_ = "Liquid Glass";
    description_ = "A beautiful liquid glass inspired theme";
    
    colors_.apply_liquid_glass_palette();
    typography_.apply_liquid_glass_typography();
    spacing_.apply_liquid_glass_spacing();
    effects_.apply_liquid_glass_effects();
    
    generate_component_styles();
    theme_changed.emit();
}

void Theme::apply_holographic_theme() {
    name_ = "Holographic";
    description_ = "A futuristic holographic theme";
    
    colors_.apply_holographic_palette();
    typography_.apply_holographic_typography();
    effects_.apply_holographic_effects();
    
    generate_component_styles();
    theme_changed.emit();
}

void Theme::apply_quantum_theme() {
    name_ = "Quantum";
    description_ = "A quantum mechanics inspired theme";
    
    colors_.apply_quantum_palette();
    effects_.apply_quantum_effects();
    
    generate_component_styles();
    theme_changed.emit();
}

void Theme::apply_consciousness_theme() {
    name_ = "Consciousness";
    description_ = "A consciousness and awareness inspired theme";
    
    colors_.apply_consciousness_palette();
    typography_.apply_consciousness_typography();
    spacing_.apply_consciousness_spacing();
    effects_.apply_consciousness_effects();
    
    generate_component_styles();
    theme_changed.emit();
}

void Theme::apply_transcendent_theme() {
    name_ = "Transcendent";
    description_ = "A transcendent, enlightened theme";
    
    colors_.apply_transcendent_palette();
    effects_.apply_consciousness_effects();
    
    generate_component_styles();
    theme_changed.emit();
}

void Theme::apply_ultimate_theme() {
    name_ = "Ultimate";
    description_ = "The ultimate perfect theme";
    
    colors_.apply_ultimate_palette();
    typography_.apply_ultimate_typography();
    spacing_.apply_ultimate_spacing();
    effects_.apply_ultimate_effects();
    
    generate_component_styles();
    theme_changed.emit();
}

void Theme::initialize_default_theme() {
    name_ = "Default";
    description_ = "Default SU1 theme";
}

void Theme::initialize_dark_theme() {
    name_ = "Dark";
    description_ = "Dark mode theme";
    
    colors_.background = Color{0.1f, 0.1f, 0.1f, 1.0f};
    colors_.surface = Color{0.15f, 0.15f, 0.15f, 1.0f};
    colors_.text_primary = Color{1.0f, 1.0f, 1.0f, 1.0f};
}

void Theme::initialize_light_theme() {
    name_ = "Light";
    description_ = "Light mode theme";
    
    colors_.background = Color{0.95f, 0.95f, 0.95f, 1.0f};
    colors_.surface = Color{1.0f, 1.0f, 1.0f, 1.0f};
    colors_.text_primary = Color{0.1f, 0.1f, 0.1f, 1.0f};
}

void Theme::initialize_liquid_glass_theme() {
    apply_liquid_glass_theme();
}

void Theme::initialize_holographic_theme() {
    apply_holographic_theme();
}

void Theme::initialize_quantum_theme() {
    apply_quantum_theme();
}

void Theme::initialize_consciousness_theme() {
    apply_consciousness_theme();
}

void Theme::initialize_transcendent_theme() {
    apply_transcendent_theme();
}

void Theme::initialize_ultimate_theme() {
    apply_ultimate_theme();
}

void Theme::generate_component_styles() {
    component_styles_[ComponentType::Button] = create_button_style();
    component_styles_[ComponentType::Panel] = create_panel_style();
    component_styles_[ComponentType::Label] = create_label_style();
    component_styles_[ComponentType::Menu] = create_menu_style();
    component_styles_[ComponentType::Bar] = create_bar_style();
    component_styles_[ComponentType::Dock] = create_dock_style();
}

ComponentStyle Theme::create_button_style() const {
    ComponentStyle style = create_component_style(ComponentType::Button);
    
    // Button-specific adjustments
    style.corner_radius = 6.0f;
    style.padding_top = spacing_.get_padding("small");
    style.padding_bottom = spacing_.get_padding("small");
    style.padding_left = spacing_.get_padding("medium");
    style.padding_right = spacing_.get_padding("medium");
    
    return style;
}

ComponentStyle Theme::create_panel_style() const {
    ComponentStyle style = create_component_style(ComponentType::Panel);
    
    // Panel-specific adjustments
    style.corner_radius = 8.0f;
    
    return style;
}

ComponentStyle Theme::create_label_style() const {
    ComponentStyle style = create_component_style(ComponentType::Label);
    
    // Label-specific adjustments
    style.background_color.a = 0.0f; // Transparent background
    style.border_width = 0.0f;
    style.focusable = false;
    
    return style;
}

ComponentStyle Theme::create_menu_style() const {
    ComponentStyle style = create_component_style(ComponentType::Menu);
    
    // Menu-specific adjustments
    style.corner_radius = 4.0f;
    style.shadow_radius = 8.0f;
    
    return style;
}

ComponentStyle Theme::create_bar_style() const {
    ComponentStyle style = create_component_style(ComponentType::Bar);
    
    // Bar-specific adjustments
    style.corner_radius = 0.0f;
    
    return style;
}

ComponentStyle Theme::create_dock_style() const {
    ComponentStyle style = create_component_style(ComponentType::Dock);
    
    // Dock-specific adjustments
    style.corner_radius = 12.0f;
    style.shadow_radius = 12.0f;
    
    return style;
}

// ThemeManager implementation
ThemeManager::ThemeManager()
    : current_theme_(nullptr)
    , default_theme_(nullptr)
    , initialized_(false)
    , dynamic_theming_enabled_(false)
    , auto_dark_mode_enabled_(false)
    , accessibility_mode_enabled_(false)
    , high_contrast_mode_enabled_(false)
    , color_blind_mode_enabled_(false)
    , adaptive_theming_enabled_(false)
    , time_based_theming_enabled_(false)
    , weather_based_theming_enabled_(false)
    , mood_based_theming_enabled_(false)
    , activity_based_theming_enabled_(false)
    , context_based_theming_enabled_(false)
    , consciousness_theming_enabled_(false)
    , transcendent_theming_enabled_(false)
    , ultimate_theming_enabled_(false)
    , theme_animating_(false)
    , animation_progress_(0.0f)
    , animation_duration_(1.0f)
    , ambient_light_level_(0.5f)
    , consciousness_level_(0.5f)
    , should_update_(false) {
    SU1_PROFILE_FUNCTION();
}

ThemeManager::~ThemeManager() {
    SU1_PROFILE_FUNCTION();
    shutdown();
}

bool ThemeManager::initialize() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (initialized_) {
        return true;
    }
    
    SU1_LOG_INFO("Initializing theme manager");
    
    // Create built-in themes
    create_default_themes();
    
    // Set default theme
    default_theme_ = themes_["default"];
    current_theme_ = default_theme_;
    
    // Start update thread if dynamic theming is enabled
    if (dynamic_theming_enabled_) {
        should_update_.store(true);
        update_thread_ = Thread([this]() { update_theme_loop(); });
    }
    
    initialized_ = true;
    SU1_LOG_INFO("Theme manager initialized with {} themes", themes_.size());
    
    return true;
}

void ThemeManager::shutdown() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (!initialized_) {
        return;
    }
    
    SU1_LOG_INFO("Shutting down theme manager");
    
    // Stop update thread
    should_update_.store(false);
    if (update_thread_.joinable()) {
        update_thread_.join();
    }
    
    themes_.clear();
    current_theme_ = nullptr;
    default_theme_ = nullptr;
    
    initialized_ = false;
    SU1_LOG_INFO("Theme manager shut down");
}

void ThemeManager::register_theme(ThemePtr theme) {
    if (!theme) return;
    
    LOCK_GUARD(mutex_);
    themes_[theme->get_name()] = theme;
    
    SU1_LOG_INFO("Registered theme: {}", theme->get_name());
}

ThemePtr ThemeManager::get_theme(const String& name) const {
    LOCK_GUARD(mutex_);
    
    auto it = themes_.find(name);
    return (it != themes_.end()) ? it->second : nullptr;
}

Vector<String> ThemeManager::get_theme_names() const {
    LOCK_GUARD(mutex_);
    
    Vector<String> names;
    for (const auto& pair : themes_) {
        names.push_back(pair.first);
    }
    
    return names;
}

void ThemeManager::set_current_theme(const String& name) {
    ThemePtr theme = get_theme(name);
    if (theme) {
        set_current_theme(theme);
    }
}

void ThemeManager::set_current_theme(ThemePtr theme) {
    if (!theme || current_theme_ == theme) {
        return;
    }
    
    LOCK_GUARD(mutex_);
    current_theme_ = theme;
    
    SU1_LOG_INFO("Switched to theme: {}", theme->get_name());
    theme_changed.emit(theme);
}

void ThemeManager::apply_theme_to_component(ComponentPtr component) const {
    if (current_theme_ && component) {
        current_theme_->apply_to_component_recursive(component);
    }
}

void ThemeManager::create_default_themes() {
    register_theme(create_default_theme());
    register_theme(create_dark_theme());
    register_theme(create_light_theme());
    register_theme(create_high_contrast_theme());
    register_theme(create_liquid_glass_theme());
    register_theme(create_holographic_theme());
    register_theme(create_quantum_theme());
    register_theme(create_consciousness_theme());
    register_theme(create_transcendent_theme());
    register_theme(create_ultimate_theme());
}

void ThemeManager::update_theme_loop() {
    while (should_update_.load()) {
        if (adaptive_theming_enabled_) {
            adapt_theme_to_environment();
        }
        
        if (consciousness_theming_enabled_) {
            evolve_theme_consciousness();
        }
        
        if (ultimate_theming_enabled_) {
            achieve_ultimate_theming();
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void ThemeManager::adapt_theme_to_environment() {
    // Adapt based on various environmental factors
    if (time_based_theming_enabled_) {
        adapt_theme_to_time();
    }
    
    if (context_based_theming_enabled_) {
        adapt_theme_to_context();
    }
}

void ThemeManager::evolve_theme_consciousness() {
    // Evolve theme based on consciousness level
    if (consciousness_level_ > 0.8f) {
        set_current_theme("transcendent");
    } else if (consciousness_level_ > 0.6f) {
        set_current_theme("consciousness");
    } else if (consciousness_level_ > 0.4f) {
        set_current_theme("quantum");
    }
}

void ThemeManager::achieve_ultimate_theming() {
    // Perfect theme experience
    if (ultimate_theming_enabled_) {
        set_current_theme("ultimate");
    }
}

void ThemeManager::adapt_theme_to_time() {
    // Simple day/night cycle
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    int hour = tm.tm_hour;
    
    if (hour >= 6 && hour < 18) {
        // Daytime
        if (current_theme_->get_name() != "light") {
            set_current_theme("light");
        }
    } else {
        // Nighttime
        if (current_theme_->get_name() != "dark") {
            set_current_theme("dark");
        }
    }
}

void ThemeManager::adapt_theme_to_context() {
    // Adapt based on current context
    // This would be implemented based on application state
}

// Built-in theme creators
ThemePtr create_default_theme() {
    return MakeShared<Theme>(ThemeType::Default);
}

ThemePtr create_dark_theme() {
    return MakeShared<Theme>(ThemeType::Dark);
}

ThemePtr create_light_theme() {
    return MakeShared<Theme>(ThemeType::Light);
}

ThemePtr create_high_contrast_theme() {
    return MakeShared<Theme>(ThemeType::HighContrast);
}

ThemePtr create_liquid_glass_theme() {
    return MakeShared<Theme>(ThemeType::LiquidGlass);
}

ThemePtr create_holographic_theme() {
    return MakeShared<Theme>(ThemeType::Holographic);
}

ThemePtr create_quantum_theme() {
    return MakeShared<Theme>(ThemeType::Quantum);
}

ThemePtr create_consciousness_theme() {
    return MakeShared<Theme>(ThemeType::Consciousness);
}

ThemePtr create_transcendent_theme() {
    return MakeShared<Theme>(ThemeType::Transcendent);
}

ThemePtr create_ultimate_theme() {
    return MakeShared<Theme>(ThemeType::Ultimate);
}

// Theme utility functions
Color adjust_color_brightness(const Color& color, f32 factor) {
    return Color{
        std::clamp(color.r * factor, 0.0f, 1.0f),
        std::clamp(color.g * factor, 0.0f, 1.0f),
        std::clamp(color.b * factor, 0.0f, 1.0f),
        color.a
    };
}

Color blend_colors(const Color& color1, const Color& color2, f32 factor) {
    f32 inv_factor = 1.0f - factor;
    return Color{
        color1.r * inv_factor + color2.r * factor,
        color1.g * inv_factor + color2.g * factor,
        color1.b * inv_factor + color2.b * factor,
        color1.a * inv_factor + color2.a * factor
    };
}

f32 calculate_color_luminance(const Color& color) {
    // Standard luminance calculation
    return 0.299f * color.r + 0.587f * color.g + 0.114f * color.b;
}

f32 calculate_color_contrast_ratio(const Color& color1, const Color& color2) {
    f32 lum1 = calculate_color_luminance(color1);
    f32 lum2 = calculate_color_luminance(color2);
    
    f32 lighter = std::max(lum1, lum2);
    f32 darker = std::min(lum1, lum2);
    
    return (lighter + 0.05f) / (darker + 0.05f);
}

bool is_color_accessible(const Color& foreground, const Color& background) {
    f32 contrast_ratio = calculate_color_contrast_ratio(foreground, background);
    return contrast_ratio >= 4.5f; // WCAG AA standard
}

void apply_consciousness_color_harmony(ColorPalette& palette) {
    // Apply consciousness-based color harmony
    palette.apply_consciousness_palette();
}

void apply_transcendent_color_alignment(ColorPalette& palette) {
    // Apply transcendent color alignment
    palette.apply_transcendent_palette();
}

void apply_ultimate_color_perfection(ColorPalette& palette) {
    // Apply ultimate color perfection
    palette.apply_ultimate_palette();
}

} // namespace su1
