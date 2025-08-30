#pragma once

#include "su1/core.hpp"
#include "su1/ui_components.hpp"

namespace su1 {

enum class ThemeType : u32 {
    Default,
    Dark,
    Light,
    HighContrast,
    LiquidGlass,
    Holographic,
    Neon,
    Material,
    Fluent,
    Neumorphism,
    Glassmorphism,
    Brutalism,
    Minimalist,
    Maximalist,
    Retro,
    Futuristic,
    Organic,
    Geometric,
    Abstract,
    Realistic,
    Surreal,
    Psychedelic,
    Cyberpunk,
    Steampunk,
    Vaporwave,
    Synthwave,
    Outrun,
    Vaporcore,
    Retrowave,
    Darkwave,
    Newwave,
    Techno,
    Trance,
    Ambient,
    Chillout,
    Lofi,
    Hifi,
    VHS,
    Analog,
    Digital,
    Quantum,
    Neural,
    Genetic,
    Evolutionary,
    Consciousness,
    Awareness,
    Mindfulness,
    Meditation,
    Zen,
    Buddhist,
    Hindu,
    Taoist,
    Sufi,
    Kabbalah,
    Christian,
    Islamic,
    Jewish,
    Pagan,
    Wiccan,
    Shamanic,
    Druidic,
    Celtic,
    Norse,
    Greek,
    Roman,
    Egyptian,
    Mayan,
    Aztec,
    Incan,
    Native_American,
    African,
    Asian,
    European,
    American,
    Australian,
    Antarctic,
    Arctic,
    Tropical,
    Desert,
    Forest,
    Mountain,
    Ocean,
    Space,
    Cosmic,
    Galactic,
    Universal,
    Multiverse,
    Omniverse,
    Dimensional,
    Interdimensional,
    Extradimensional,
    Hyperdimensional,
    Metadimensional,
    Paradimensional,
    Transdimensional,
    Omnidimensional,
    Transcendent,
    Enlightened,
    Ascended,
    Divine,
    Sacred,
    Holy,
    Blessed,
    Pure,
    Perfect,
    Ultimate
};

enum class ColorScheme : u32 {
    Monochrome,
    Complementary,
    Analogous,
    Triadic,
    Tetradic,
    Monochromatic,
    Achromatic,
    Warm,
    Cool,
    Neutral,
    Pastel,
    Vibrant,
    Muted,
    Saturated,
    Desaturated,
    HighContrast,
    LowContrast,
    Gradient,
    Rainbow,
    Spectrum,
    Prismatic,
    Chromatic,
    Iridescent,
    Opalescent,
    Pearlescent,
    Metallic,
    Fluorescent,
    Phosphorescent,
    Luminescent,
    Incandescent,
    Bioluminescent,
    Chemiluminescent,
    Electroluminescent,
    Photoluminescent,
    Thermoluminescent,
    Triboluminescent,
    Sonoluminescent,
    Crystalloluminescent,
    Radioluminescent,
    Cathodoluminescent,
    Fractoluminescent,
    Quantum_Luminescent,
    Consciousness_Spectrum,
    Awareness_Palette,
    Enlightenment_Colors,
    Transcendent_Hues,
    Divine_Radiance,
    Cosmic_Brilliance,
    Universal_Luminance,
    Ultimate_Spectrum
};

struct ColorPalette {
    Color primary;
    Color secondary;
    Color accent;
    Color background;
    Color surface;
    Color error;
    Color warning;
    Color info;
    Color success;
    Color text_primary;
    Color text_secondary;
    Color text_disabled;
    Color divider;
    Color outline;
    Color shadow;
    Color highlight;
    Color selection;
    Color focus;
    Color hover;
    Color pressed;
    Color disabled;
    Color link;
    Color visited;
    Color active;
    Color inactive;
    
    Vector<Color> gradients;
    Vector<Color> spectrum;
    Map<String, Color> custom_colors;
    
    ColorPalette();
    void apply_color_scheme(ColorScheme scheme);
    void generate_harmonious_colors();
    void apply_accessibility_adjustments();
    Color get_color(const String& name) const;
    void set_color(const String& name, const Color& color);
    
    void apply_liquid_glass_palette();
    void apply_holographic_palette();
    void apply_quantum_palette();
    void apply_consciousness_palette();
    void apply_transcendent_palette();
    void apply_ultimate_palette();
};

struct Typography {
    String primary_font;
    String secondary_font;
    String monospace_font;
    String display_font;
    String body_font;
    String caption_font;
    
    f32 scale_factor;
    f32 base_size;
    f32 line_height_ratio;
    f32 letter_spacing;
    f32 word_spacing;
    
    Map<String, f32> font_sizes;
    Map<String, f32> font_weights;
    Map<String, String> font_styles;
    
    Typography();
    f32 get_font_size(const String& variant) const;
    f32 get_font_weight(const String& variant) const;
    String get_font_style(const String& variant) const;
    
    void set_font_size(const String& variant, f32 size);
    void set_font_weight(const String& variant, f32 weight);
    void set_font_style(const String& variant, const String& style);
    
    void apply_responsive_scaling(f32 screen_density);
    void apply_accessibility_sizing();
    
    void apply_liquid_glass_typography();
    void apply_holographic_typography();
    void apply_quantum_typography();
    void apply_consciousness_typography();
    void apply_transcendent_typography();
    void apply_ultimate_typography();
};

struct Spacing {
    f32 base_unit;
    f32 density_factor;
    
    Map<String, f32> margins;
    Map<String, f32> paddings;
    Map<String, f32> gaps;
    Map<String, f32> sizes;
    
    Spacing();
    f32 get_margin(const String& variant) const;
    f32 get_padding(const String& variant) const;
    f32 get_gap(const String& variant) const;
    f32 get_size(const String& variant) const;
    
    void set_margin(const String& variant, f32 value);
    void set_padding(const String& variant, f32 value);
    void set_gap(const String& variant, f32 value);
    void set_size(const String& variant, f32 value);
    
    void apply_density_scaling(f32 density);
    void apply_responsive_scaling(f32 screen_size);
    
    void apply_liquid_glass_spacing();
    void apply_consciousness_spacing();
    void apply_ultimate_spacing();
};

struct Effects {
    bool shadows_enabled;
    bool blur_enabled;
    bool glow_enabled;
    bool transparency_enabled;
    bool gradients_enabled;
    bool animations_enabled;
    bool particles_enabled;
    bool distortions_enabled;
    
    f32 shadow_intensity;
    f32 blur_intensity;
    f32 glow_intensity;
    f32 transparency_level;
    f32 animation_speed;
    f32 particle_density;
    f32 distortion_amount;
    
    Map<String, f32> effect_parameters;
    Map<String, bool> effect_toggles;
    
    Effects();
    void enable_all_effects();
    void disable_all_effects();
    void set_quality_level(u32 level);
    void optimize_for_performance();
    void maximize_quality();
    
    void apply_liquid_glass_effects();
    void apply_holographic_effects();
    void apply_quantum_effects();
    void apply_consciousness_effects();
    void apply_transcendent_effects();
    void apply_ultimate_effects();
};

class Theme {
public:
    Theme(ThemeType type = ThemeType::Default);
    ~Theme();

    ThemeType get_type() const { return type_; }
    const String& get_name() const { return name_; }
    void set_name(const String& name) { name_ = name; }
    
    const String& get_description() const { return description_; }
    void set_description(const String& description) { description_ = description; }
    
    const String& get_author() const { return author_; }
    void set_author(const String& author) { author_ = author; }
    
    const String& get_version() const { return version_; }
    void set_version(const String& version) { version_ = version; }
    
    const ColorPalette& get_colors() const { return colors_; }
    ColorPalette& get_mutable_colors() { return colors_; }
    void set_colors(const ColorPalette& colors) { colors_ = colors; }
    
    const Typography& get_typography() const { return typography_; }
    Typography& get_mutable_typography() { return typography_; }
    void set_typography(const Typography& typography) { typography_ = typography; }
    
    const Spacing& get_spacing() const { return spacing_; }
    Spacing& get_mutable_spacing() { return spacing_; }
    void set_spacing(const Spacing& spacing) { spacing_ = spacing; }
    
    const Effects& get_effects() const { return effects_; }
    Effects& get_mutable_effects() { return effects_; }
    void set_effects(const Effects& effects) { effects_ = effects; }
    
    ComponentStyle create_component_style(ComponentType component_type) const;
    void apply_to_component(ComponentPtr component) const;
    void apply_to_component_recursive(ComponentPtr component) const;
    
    void set_property(const String& key, const Any& value);
    Any get_property(const String& key) const;
    bool has_property(const String& key) const;
    
    void inherit_from(const Theme& parent_theme);
    void merge_with(const Theme& other_theme);
    Theme derive_variant(const String& variant_name) const;
    
    bool load_from_file(const String& file_path);
    bool save_to_file(const String& file_path) const;
    
    String serialize() const;
    bool deserialize(const String& data);
    
    void apply_dark_mode();
    void apply_light_mode();
    void apply_high_contrast();
    void apply_color_blind_friendly();
    void apply_accessibility_features();
    
    void apply_liquid_glass_theme();
    void apply_holographic_theme();
    void apply_quantum_theme();
    void apply_consciousness_theme();
    void apply_transcendent_theme();
    void apply_ultimate_theme();
    
    Signal<void()> theme_changed;

private:
    ThemeType type_;
    String name_;
    String description_;
    String author_;
    String version_;
    
    ColorPalette colors_;
    Typography typography_;
    Spacing spacing_;
    Effects effects_;
    
    Map<String, Any> properties_;
    Map<ComponentType, ComponentStyle> component_styles_;
    
    void initialize_default_theme();
    void initialize_dark_theme();
    void initialize_light_theme();
    void initialize_liquid_glass_theme();
    void initialize_holographic_theme();
    void initialize_quantum_theme();
    void initialize_consciousness_theme();
    void initialize_transcendent_theme();
    void initialize_ultimate_theme();
    
    void generate_component_styles();
    ComponentStyle create_button_style() const;
    ComponentStyle create_panel_style() const;
    ComponentStyle create_label_style() const;
    ComponentStyle create_menu_style() const;
    ComponentStyle create_bar_style() const;
    ComponentStyle create_dock_style() const;
    
    void apply_accessibility_to_colors();
    void apply_accessibility_to_typography();
    void apply_accessibility_to_spacing();
    void optimize_for_screen_reader();
    void optimize_for_motor_impairment();
    void optimize_for_cognitive_load();
    
    void harmonize_consciousness_theme();
    void align_transcendent_aesthetics();
    void perfect_ultimate_beauty();
};

using ThemePtr = SharedPtr<Theme>;

class ThemeManager {
public:
    ThemeManager();
    ~ThemeManager();

    bool initialize();
    void shutdown();

    void register_theme(ThemePtr theme);
    void unregister_theme(const String& name);
    ThemePtr get_theme(const String& name) const;
    Vector<String> get_theme_names() const;
    Vector<ThemePtr> get_all_themes() const;
    
    void set_current_theme(const String& name);
    void set_current_theme(ThemePtr theme);
    ThemePtr get_current_theme() const { return current_theme_; }
    
    void apply_theme_to_component(ComponentPtr component) const;
    void apply_theme_to_ui_manager(UIManagerPtr ui_manager) const;
    
    bool load_theme_from_file(const String& file_path);
    bool save_theme_to_file(ThemePtr theme, const String& file_path) const;
    
    void load_themes_from_directory(const String& directory_path);
    void save_all_themes_to_directory(const String& directory_path) const;
    
    ThemePtr create_theme_variant(const String& base_theme_name, const String& variant_name) const;
    ThemePtr blend_themes(const String& theme1_name, const String& theme2_name, f32 blend_factor) const;
    
    void enable_dynamic_theming(bool enabled) { dynamic_theming_enabled_ = enabled; }
    bool is_dynamic_theming_enabled() const { return dynamic_theming_enabled_; }
    
    void set_auto_dark_mode(bool enabled) { auto_dark_mode_enabled_ = enabled; }
    bool is_auto_dark_mode_enabled() const { return auto_dark_mode_enabled_; }
    
    void set_accessibility_mode(bool enabled);
    bool is_accessibility_mode_enabled() const { return accessibility_mode_enabled_; }
    
    void set_high_contrast_mode(bool enabled);
    bool is_high_contrast_mode_enabled() const { return high_contrast_mode_enabled_; }
    
    void set_color_blind_mode(bool enabled);
    bool is_color_blind_mode_enabled() const { return color_blind_mode_enabled_; }
    
    void apply_user_preferences(const Map<String, Any>& preferences);
    Map<String, Any> get_user_preferences() const;
    
    void start_theme_animation(const String& target_theme, f32 duration = 1.0f);
    void stop_theme_animation();
    bool is_theme_animating() const { return theme_animating_; }
    
    void enable_adaptive_theming(bool enabled) { adaptive_theming_enabled_ = enabled; }
    void set_ambient_light_sensor(f32 light_level);
    void set_time_based_theming(bool enabled) { time_based_theming_enabled_ = enabled; }
    void set_weather_based_theming(bool enabled) { weather_based_theming_enabled_ = enabled; }
    void set_mood_based_theming(bool enabled) { mood_based_theming_enabled_ = enabled; }
    void set_activity_based_theming(bool enabled) { activity_based_theming_enabled_ = enabled; }
    void set_context_based_theming(bool enabled) { context_based_theming_enabled_ = enabled; }
    
    void enable_consciousness_theming(bool enabled) { consciousness_theming_enabled_ = enabled; }
    void set_consciousness_level(f32 level) { consciousness_level_ = level; }
    void enable_transcendent_theming(bool enabled) { transcendent_theming_enabled_ = enabled; }
    void enable_ultimate_theming(bool enabled) { ultimate_theming_enabled_ = enabled; }
    
    void update_dynamic_theme(f64 delta_time);
    void harmonize_theme_with_environment();
    void attune_theme_to_consciousness();
    void perfect_theme_experience();
    
    Signal<void(ThemePtr)> theme_changed;
    Signal<void()> theme_animation_started;
    Signal<void()> theme_animation_finished;

    bool is_initialized() const { return initialized_; }

private:
    Map<String, ThemePtr> themes_;
    ThemePtr current_theme_;
    ThemePtr default_theme_;
    
    bool initialized_;
    bool dynamic_theming_enabled_;
    bool auto_dark_mode_enabled_;
    bool accessibility_mode_enabled_;
    bool high_contrast_mode_enabled_;
    bool color_blind_mode_enabled_;
    bool adaptive_theming_enabled_;
    bool time_based_theming_enabled_;
    bool weather_based_theming_enabled_;
    bool mood_based_theming_enabled_;
    bool activity_based_theming_enabled_;
    bool context_based_theming_enabled_;
    bool consciousness_theming_enabled_;
    bool transcendent_theming_enabled_;
    bool ultimate_theming_enabled_;
    
    bool theme_animating_;
    f32 animation_progress_;
    f32 animation_duration_;
    ThemePtr animation_start_theme_;
    ThemePtr animation_target_theme_;
    
    f32 ambient_light_level_;
    f32 consciousness_level_;
    
    Map<String, Any> user_preferences_;
    
    Mutex mutex_;
    Thread update_thread_;
    Atomic<bool> should_update_;
    
    void create_default_themes();
    void setup_built_in_themes();
    void register_built_in_theme(ThemeType type);
    
    void update_theme_loop();
    void check_auto_dark_mode();
    void check_ambient_lighting();
    void check_time_based_changes();
    void check_context_changes();
    void check_consciousness_level();
    
    ThemePtr interpolate_themes(ThemePtr theme1, ThemePtr theme2, f32 factor) const;
    ColorPalette interpolate_color_palettes(const ColorPalette& palette1, const ColorPalette& palette2, f32 factor) const;
    Color interpolate_colors(const Color& color1, const Color& color2, f32 factor) const;
    
    void apply_accessibility_modifications(ThemePtr theme);
    void apply_high_contrast_modifications(ThemePtr theme);
    void apply_color_blind_modifications(ThemePtr theme);
    
    void adapt_theme_to_environment();
    void adapt_theme_to_time();
    void adapt_theme_to_weather();
    void adapt_theme_to_mood();
    void adapt_theme_to_activity();
    void adapt_theme_to_context();
    void adapt_theme_to_consciousness();
    
    void evolve_theme_consciousness();
    void transcend_theme_limitations();
    void achieve_ultimate_theming();
    
    f32 calculate_optimal_brightness();
    f32 calculate_optimal_contrast();
    f32 calculate_optimal_saturation();
    ColorScheme calculate_optimal_color_scheme();
    
    void harmonize_theme_energy();
    void balance_theme_elements();
    void align_theme_vibrations();
    void attune_theme_frequencies();
    void perfect_theme_resonance();
};

using ThemeManagerPtr = SharedPtr<ThemeManager>;

// Built-in theme creators
ThemePtr create_default_theme();
ThemePtr create_dark_theme();
ThemePtr create_light_theme();
ThemePtr create_high_contrast_theme();
ThemePtr create_liquid_glass_theme();
ThemePtr create_holographic_theme();
ThemePtr create_quantum_theme();
ThemePtr create_consciousness_theme();
ThemePtr create_transcendent_theme();
ThemePtr create_ultimate_theme();

// Theme utilities
Color adjust_color_brightness(const Color& color, f32 factor);
Color adjust_color_saturation(const Color& color, f32 factor);
Color adjust_color_contrast(const Color& color, f32 factor);
Color blend_colors(const Color& color1, const Color& color2, f32 factor);
bool is_color_accessible(const Color& foreground, const Color& background);
f32 calculate_color_contrast_ratio(const Color& color1, const Color& color2);
f32 calculate_color_luminance(const Color& color);

// Advanced theme functions
void apply_consciousness_color_harmony(ColorPalette& palette);
void apply_transcendent_color_alignment(ColorPalette& palette);
void apply_ultimate_color_perfection(ColorPalette& palette);
void harmonize_quantum_color_field(ColorPalette& palette);
void balance_cosmic_color_energies(ColorPalette& palette);
void attune_universal_color_frequencies(ColorPalette& palette);

}

#endif
