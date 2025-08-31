#pragma once

#include "su1/core.hpp"
#include "su1/window.hpp"
#include "su1/renderer.hpp"

namespace su1 {

enum class ComponentType : u32 {
    Panel,
    Bar,
    Dock,
    Menu,
    Button,
    Label,
    TextBox,
    Slider,
    ProgressBar,
    Checkbox,
    RadioButton,
    ComboBox,
    ListBox,
    TreeView,
    TabControl,
    Splitter,
    ScrollBar,
    StatusBar,
    ToolBar,
    MenuBar,
    ContextMenu,
    Tooltip,
    Dialog,
    MessageBox,
    FileDialog,
    ColorPicker,
    DatePicker,
    TimePicker,
    Calendar,
    Chart,
    Graph,
    Image,
    Video,
    WebView,
    Canvas,
    Terminal,
    CodeEditor,
    RichTextEditor,
    Markdown,
    PDF,
    Spreadsheet,
    Database,
    Form,
    Wizard,
    Accordion,
    Carousel,
    Gallery,
    Timeline,
    Kanban,
    Dashboard,
    Workspace,
    Desktop,
    Taskbar,
    SystemTray,
    Notification,
    PopupMenu,
    FloatingWindow,
    ModalDialog,
    SidePanel,
    HeaderBar,
    FooterBar,
    NavigationBar,
    SearchBar,
    FilterBar,
    ToolPanel,
    PropertyPanel,
    InspectorPanel,
    LayerPanel,
    HistoryPanel,
    LibraryPanel,
    PreviewPanel,
    OutputPanel,
    ConsolePanel,
    DebugPanel,
    ProfilePanel,
    AnalyticsPanel,
    MonitorPanel,
    ControlPanel,
    SettingsPanel,
    PreferencesPanel,
    ConfigPanel,
    AdminPanel,
    UserPanel,
    AccountPanel,
    SecurityPanel,
    PrivacyPanel,
    HelpPanel,
    AboutPanel,
    LegalPanel,
    ContactPanel,
    FeedbackPanel,
    SupportPanel,
    DocumentationPanel,
    TutorialPanel,
    GuidePanel,
    TipPanel,
    NewsPanel,
    UpdatePanel,
    DownloadPanel,
    UploadPanel,
    SharePanel,
    ExportPanel,
    ImportPanel,
    BackupPanel,
    RestorePanel,
    SyncPanel,
    CloudPanel,
    NetworkPanel,
    ServerPanel,
    ClientPanel,
    APIPanel,
    WebhookPanel,
    PluginPanel,
    ExtensionPanel,
    ThemePanel,
    LanguagePanel,
    LocalizationPanel,
    AccessibilityPanel,
    PerformancePanel,
    MemoryPanel,
    CPUPanel,
    GPUPanel,
    StoragePanel,
    BandwidthPanel,
    LatencyPanel,
    ThroughputPanel,
    QualityPanel,
    ReliabilityPanel,
    SecurityMetricsPanel,
    PrivacyMetricsPanel,
    CompliancePanel,
    AuditPanel,
    LogPanel,
    EventPanel,
    AlertPanel,
    WarningPanel,
    ErrorPanel,
    InfoPanel,
    SuccessPanel,
    FailurePanel,
    PendingPanel,
    InProgressPanel,
    CompletedPanel,
    CancelledPanel,
    ScheduledPanel,
    DelayedPanel,
    ExpiredPanel,
    ArchivedPanel,
    DeletedPanel,
    RestoredPanel,
    PublishedPanel,
    DraftPanel,
    ReviewPanel,
    ApprovedPanel,
    RejectedPanel,
    OnHoldPanel,
    ActivePanel,
    InactivePanel,
    SuspendedPanel,
    BlockedPanel,
    BannedPanel,
    VerifiedPanel,
    UnverifiedPanel,
    AuthenticatedPanel,
    UnauthenticatedPanel,
    AuthorizedPanel,
    UnauthorizedPanel,
    PermittedPanel,
    DeniedPanel,
    GrantedPanel,
    RevokedPanel,
    EnabledPanel,
    DisabledPanel,
    VisiblePanel,
    HiddenPanel,
    PublicPanel,
    PrivatePanel,
    SharedPanel,
    PersonalPanel,
    GroupPanel,
    TeamPanel,
    OrganizationPanel,
    CommunityPanel,
    GlobalPanel,
    LocalPanel,
    RegionalPanel,
    NationalPanel,
    InternationalPanel,
    UniversalPanel,
    CosmicPanel,
    QuantumPanel,
    HolographicPanel,
    VirtualPanel,
    AugmentedPanel,
    MixedPanel,
    ExtendedPanel,
    ImmersivePanel,
    InteractivePanel,
    ResponsivePanel,
    AdaptivePanel,
    IntelligentPanel,
    SmartPanel,
    AutomatedPanel,
    ManualPanel,
    SemiAutomatedPanel,
    AIAssistedPanel,
    MLPoweredPanel,
    NeuralPanel,
    DeepLearningPanel,
    ReinforcementLearningPanel,
    UnsupervisedLearningPanel,
    SupervisedLearningPanel,
    TransferLearningPanel,
    FederatedLearningPanel,
    OnlineeLearningPanel,
    OfflineLearningPanel,
    RealTimeLearningPanel,
    BatchLearningPanel,
    StreamingLearningPanel,
    ContinualLearningPanel,
    LifelongLearningPanel,
    MetaLearningPanel,
    FewShotLearningPanel,
    ZeroShotLearningPanel,
    OnehotsLearningPanel,
    MultiTaskLearningPanel,
    MultiModalLearningPanel,
    CrossModalLearningPanel,
    SelfSupervisedLearningPanel,
    ContrastiveLearningPanel,
    GenerativePanel,
    DiscriminativePanel,
    AdversarialPanel,
    CooperativePanel,
    CompetitivePanel,
    CollaborativePanel,
    DistributedPanel,
    CentralizedPanel,
    DecentralizedPanel,
    FederatedPanel,
    P2PPanel,
    BlockchainPanel,
    CryptographicPanel,
    QuantumCryptographicPanel,
    PostQuantumPanel,
    QuantumResistantPanel,
    ConsciousnessPanel,
    AwarenessPanel,
    IntuitionPanel,
    WisdomPanel,
    EnlightenmentPanel,
    TranscendencePanel,
    UltimatePanel
};

enum class Alignment : u32 {
    TopLeft,
    TopCenter,
    TopRight,
    MiddleLeft,
    MiddleCenter,
    MiddleRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
    Stretch,
    Fill,
    Center,
    Justify
};

enum class Orientation : u32 {
    Horizontal,
    Vertical
};

enum class LayoutMode : u32 {
    Fixed,
    Relative,
    Absolute,
    Flow,
    Grid,
    Flex,
    Stack,
    Wrap,
    Masonry,
    Isotope,
    Packery,
    Mosaic,
    Waterfall,
    Spiral,
    Circular,
    Radial,
    Tree,
    Network,
    Force,
    Spring,
    Particle,
    Fluid,
    Elastic,
    Magnetic,
    Gravitational,
    Quantum,
    Consciousness,
    Transcendent,
    Ultimate
};

struct ComponentStyle {
    Color background_color;
    Color foreground_color;
    Color border_color;
    Color shadow_color;
    Color highlight_color;
    Color selection_color;
    Color focus_color;
    Color hover_color;
    Color pressed_color;
    Color disabled_color;
    
    f32 opacity;
    f32 border_width;
    f32 corner_radius;
    f32 shadow_radius;
    f32 shadow_offset_x;
    f32 shadow_offset_y;
    f32 highlight_intensity;
    f32 glow_radius;
    f32 blur_radius;
    f32 saturation;
    f32 brightness;
    f32 contrast;
    f32 hue_shift;
    
    String font_family;
    f32 font_size;
    f32 font_weight;
    bool font_italic;
    bool font_underline;
    bool font_strikethrough;
    
    Alignment text_alignment;
    f32 line_height;
    f32 letter_spacing;
    f32 word_spacing;
    
    f32 padding_top;
    f32 padding_right;
    f32 padding_bottom;
    f32 padding_left;
    
    f32 margin_top;
    f32 margin_right;
    f32 margin_bottom;
    f32 margin_left;
    
    bool visible;
    bool enabled;
    bool focusable;
    bool selectable;
    bool draggable;
    bool resizable;
    bool scrollable;
    bool clippable;
    bool animatable;
    
    String background_image;
    String background_pattern;
    String background_gradient;
    String texture;
    String material;
    String shader;
    String filter;
    String transform;
    String animation;
    String transition;
    
    Map<String, Any> custom_properties;
    
    ComponentStyle();
    void apply_liquid_glass_effect();
    void apply_holographic_effect();
    void apply_quantum_effect();
    void apply_consciousness_effect();
    void apply_transcendent_effect();
    void apply_ultimate_effect();
};

class Component {
public:
    Component(ComponentType type);
    virtual ~Component();

    ComponentType get_type() const { return type_; }
    u32 get_id() const { return id_; }
    const String& get_name() const { return name_; }
    void set_name(const String& name) { name_ = name; }
    
    const Rect& get_bounds() const { return bounds_; }
    void set_bounds(const Rect& bounds);
    
    Point get_position() const { return bounds_.position; }
    void set_position(const Point& position);
    
    Size get_size() const { return bounds_.size; }
    void set_size(const Size& size);
    
    bool is_visible() const { return style_.visible; }
    void set_visible(bool visible);
    void show() { set_visible(true); }
    void hide() { set_visible(false); }
    
    bool is_enabled() const { return style_.enabled; }
    void set_enabled(bool enabled) { style_.enabled = enabled; }
    void enable() { set_enabled(true); }
    void disable() { set_enabled(false); }
    
    bool is_focused() const { return focused_; }
    void set_focused(bool focused);
    void focus() { set_focused(true); }
    void unfocus() { set_focused(false); }
    
    bool is_hovered() const { return hovered_; }
    bool is_pressed() const { return pressed_; }
    
    Component* get_parent() const { return parent_; }
    void set_parent(Component* parent);
    
    const Vector<SharedPtr<Component>>& get_children() const { return children_; }
    void add_child(SharedPtr<Component> child);
    void remove_child(SharedPtr<Component> child);
    void clear_children();
    
    SharedPtr<Component> find_child(const String& name) const;
    SharedPtr<Component> find_child_by_id(u32 id) const;
    Vector<SharedPtr<Component>> find_children_by_type(ComponentType type) const;
    
    const ComponentStyle& get_style() const { return style_; }
    void set_style(const ComponentStyle& style) { style_ = style; }
    ComponentStyle& get_mutable_style() { return style_; }
    
    void set_property(const String& name, const Any& value);
    Any get_property(const String& name) const;
    bool has_property(const String& name) const;
    
    virtual void update(f64 delta_time);
    virtual void render(RendererPtr renderer);
    virtual void layout();
    
    virtual bool handle_mouse_move(const Point& position);
    virtual bool handle_mouse_down(const Point& position, u32 button);
    virtual bool handle_mouse_up(const Point& position, u32 button);
    virtual bool handle_mouse_wheel(const Point& position, f32 delta);
    virtual bool handle_key_down(u32 key, u32 modifiers);
    virtual bool handle_key_up(u32 key, u32 modifiers);
    virtual bool handle_text_input(const String& text);
    
    virtual void on_mouse_enter();
    virtual void on_mouse_leave();
    virtual void on_focus_gained();
    virtual void on_focus_lost();
    virtual void on_size_changed();
    virtual void on_position_changed();
    virtual void on_style_changed();
    virtual void on_property_changed(const String& name);
    
    // Animation and effects
    void animate_to(const String& property, const Any& target_value, f32 duration);
    void animate_property(const String& property, const Any& from, const Any& to, f32 duration);
    void stop_animation(const String& property);
    void stop_all_animations();
    
    void apply_liquid_glass_effect();
    void apply_holographic_effect();
    void apply_quantum_effect();
    void apply_consciousness_effect();
    void apply_transcendent_effect();
    void apply_ultimate_effect();
    
    // Signals
    Signal<void(Component*)> clicked;
    Signal<void(Component*)> double_clicked;
    Signal<void(Component*)> right_clicked;
    Signal<void(Component*)> mouse_entered;
    Signal<void(Component*)> mouse_left;
    Signal<void(Component*)> focus_gained;
    Signal<void(Component*)> focus_lost;
    Signal<void(Component*, const Size&)> size_changed;
    Signal<void(Component*, const Point&)> position_changed;
    Signal<void(Component*, const String&)> property_changed;

protected:
    ComponentType type_;
    u32 id_;
    String name_;
    Rect bounds_;
    ComponentStyle style_;
    Component* parent_;
    Vector<SharedPtr<Component>> children_;
    
    bool focused_;
    bool hovered_;
    bool pressed_;
    
    Map<String, Any> properties_;
    Map<String, Any> animations_;
    
    static u32 next_id_;
    
    virtual void render_background(RendererPtr renderer);
    virtual void render_border(RendererPtr renderer);
    virtual void render_shadow(RendererPtr renderer);
    virtual void render_glow(RendererPtr renderer);
    virtual void render_content(RendererPtr renderer);
    virtual void render_overlay(RendererPtr renderer);
    
    void update_hover_state(const Point& position);
    void update_animations(f64 delta_time);
    void invalidate_layout();
    void invalidate_render();
};

using ComponentPtr = SharedPtr<Component>;

class Panel : public Component {
public:
    Panel();
    ~Panel() override;
    
    void set_layout_mode(LayoutMode mode) { layout_mode_ = mode; }
    LayoutMode get_layout_mode() const { return layout_mode_; }
    
    void set_orientation(Orientation orientation) { orientation_ = orientation; }
    Orientation get_orientation() const { return orientation_; }
    
    void set_spacing(f32 spacing) { spacing_ = spacing; }
    f32 get_spacing() const { return spacing_; }
    
    void set_auto_resize(bool auto_resize) { auto_resize_ = auto_resize; }
    bool get_auto_resize() const { return auto_resize_; }
    
    void layout() override;
    void render(RendererPtr renderer) override;

private:
    LayoutMode layout_mode_;
    Orientation orientation_;
    f32 spacing_;
    bool auto_resize_;
    
    void layout_fixed();
    void layout_flow();
    void layout_grid();
    void layout_flex();
    void layout_stack();
    void layout_quantum();
    void layout_consciousness();
    void layout_ultimate();
};

class Bar : public Panel {
public:
    Bar();
    ~Bar() override;
    
    void set_dock_side(Alignment dock_side) { dock_side_ = dock_side; }
    Alignment get_dock_side() const { return dock_side_; }
    
    void set_auto_hide(bool auto_hide) { auto_hide_ = auto_hide; }
    bool get_auto_hide() const { return auto_hide_; }
    
    void set_always_on_top(bool always_on_top) { always_on_top_ = always_on_top; }
    bool get_always_on_top() const { return always_on_top_; }
    
    void layout() override;
    void render(RendererPtr renderer) override;

private:
    Alignment dock_side_;
    bool auto_hide_;
    bool always_on_top_;
    bool hidden_;
    f32 hide_timer_;
    
    void update_auto_hide(f64 delta_time);
    void slide_in();
    void slide_out();
};

class Dock : public Panel {
public:
    Dock();
    ~Dock() override;
    
    void add_item(ComponentPtr item);
    void remove_item(ComponentPtr item);
    void clear_items();
    
    void set_icon_size(f32 size) { icon_size_ = size; }
    f32 get_icon_size() const { return icon_size_; }
    
    void set_magnification(f32 magnification) { magnification_ = magnification; }
    f32 get_magnification() const { return magnification_; }
    
    void set_bounce_animation(bool bounce) { bounce_animation_ = bounce; }
    bool get_bounce_animation() const { return bounce_animation_; }
    
    void layout() override;
    void render(RendererPtr renderer) override;
    bool handle_mouse_move(const Point& position) override;

private:
    Vector<ComponentPtr> items_;
    f32 icon_size_;
    f32 magnification_;
    bool bounce_animation_;
    Point mouse_position_;
    
    void update_magnification_effect();
    void animate_item_bounce(ComponentPtr item);
    void render_reflection(RendererPtr renderer);
    void render_glow_effect(RendererPtr renderer);
};

class Menu : public Panel {
public:
    Menu();
    ~Menu() override;
    
    void add_item(const String& text, const String& icon = "");
    void add_separator();
    void add_submenu(const String& text, SharedPtr<Menu> submenu);
    
    void set_selected_index(i32 index) { selected_index_ = index; }
    i32 get_selected_index() const { return selected_index_; }
    
    void show_at(const Point& position);
    void hide();
    
    void layout() override;
    void render(RendererPtr renderer) override;
    bool handle_mouse_move(const Point& position) override;
    bool handle_mouse_down(const Point& position, u32 button) override;
    bool handle_key_down(u32 key, u32 modifiers) override;
    
    Signal<void(i32)> item_selected;
    Signal<void()> menu_closed;

private:
    struct MenuItem {
        String text;
        String icon;
        bool is_separator;
        SharedPtr<Menu> submenu;
        Rect bounds;
        bool hovered;
        bool enabled;
    };
    
    Vector<MenuItem> items_;
    i32 selected_index_;
    SharedPtr<Menu> open_submenu_;
    bool is_open_;
    f32 item_height_;
    
    void update_item_bounds();
    void render_item(RendererPtr renderer, const MenuItem& item, usize index);
    void select_item(i32 index);
    void activate_item(i32 index);
    void close_submenus();
};

class Button : public Component {
public:
    Button();
    ~Button() override;
    
    const String& get_text() const { return text_; }
    void set_text(const String& text);
    
    const String& get_icon() const { return icon_; }
    void set_icon(const String& icon) { icon_ = icon; }
    
    bool is_checkable() const { return checkable_; }
    void set_checkable(bool checkable) { checkable_ = checkable; }
    
    bool is_checked() const { return checked_; }
    void set_checked(bool checked);
    
    void render(RendererPtr renderer) override;
    bool handle_mouse_down(const Point& position, u32 button) override;
    bool handle_mouse_up(const Point& position, u32 button) override;
    
    Signal<void()> pressed;
    Signal<void(bool)> toggled;

private:
    String text_;
    String icon_;
    bool checkable_;
    bool checked_;
    
    void render_button_background(RendererPtr renderer);
    void render_button_text(RendererPtr renderer);
    void render_button_icon(RendererPtr renderer);
    void render_check_indicator(RendererPtr renderer);
};

class Label : public Component {
public:
    Label();
    ~Label() override;
    
    const String& get_text() const { return text_; }
    void set_text(const String& text);
    
    Alignment get_text_alignment() const { return text_alignment_; }
    void set_text_alignment(Alignment alignment) { text_alignment_ = alignment; }
    
    bool get_word_wrap() const { return word_wrap_; }
    void set_word_wrap(bool wrap) { word_wrap_ = wrap; }
    
    void render(RendererPtr renderer) override;

private:
    String text_;
    Alignment text_alignment_;
    bool word_wrap_;
    
    void render_text(RendererPtr renderer);
    Size calculate_text_size() const;
    Vector<String> wrap_text(const String& text, f32 max_width) const;
};

class UIManager {
public:
    UIManager();
    ~UIManager();

    bool initialize();
    void shutdown();

    void set_root_component(ComponentPtr root) { root_component_ = root; }
    ComponentPtr get_root_component() const { return root_component_; }

    void update(f64 delta_time);
    void render(RendererPtr renderer);
    void layout();

    bool handle_mouse_move(const Point& position);
    bool handle_mouse_down(const Point& position, u32 button);
    bool handle_mouse_up(const Point& position, u32 button);
    bool handle_mouse_wheel(const Point& position, f32 delta);
    bool handle_key_down(u32 key, u32 modifiers);
    bool handle_key_up(u32 key, u32 modifiers);
    bool handle_text_input(const String& text);

    void set_focused_component(ComponentPtr component);
    ComponentPtr get_focused_component() const { return focused_component_; }

    void set_hovered_component(ComponentPtr component);
    ComponentPtr get_hovered_component() const { return hovered_component_; }

    void set_captured_component(ComponentPtr component);
    ComponentPtr get_captured_component() const { return captured_component_; }

    ComponentPtr find_component_at(const Point& position);
    ComponentPtr find_component_by_id(u32 id);
    ComponentPtr find_component_by_name(const String& name);

    void register_component_factory(ComponentType type, Function<ComponentPtr()> factory);
    ComponentPtr create_component(ComponentType type);

    // Theme and styling
    void set_global_style(const ComponentStyle& style) { global_style_ = style; }
    const ComponentStyle& get_global_style() const { return global_style_; }

    void load_theme(const String& theme_path);
    void save_theme(const String& theme_path);
    void apply_theme(const String& theme_name);

    // Layout management
    void invalidate_layout() { layout_dirty_ = true; }
    void force_layout();

    // Animation system
    void start_global_animation();
    void stop_global_animation();
    void set_animation_speed(f32 speed) { animation_speed_ = speed; }

    // Effects system
    void enable_liquid_glass_effects(bool enabled) { liquid_glass_enabled_ = enabled; }
    void enable_holographic_effects(bool enabled) { holographic_enabled_ = enabled; }
    void enable_quantum_effects(bool enabled) { quantum_enabled_ = enabled; }
    void enable_consciousness_effects(bool enabled) { consciousness_enabled_ = enabled; }
    void enable_transcendent_effects(bool enabled) { transcendent_enabled_ = enabled; }
    void enable_ultimate_effects(bool enabled) { ultimate_effects_enabled_ = enabled; }

    bool is_initialized() const { return initialized_; }

private:
    ComponentPtr root_component_;
    ComponentPtr focused_component_;
    ComponentPtr hovered_component_;
    ComponentPtr captured_component_;
    
    ComponentStyle global_style_;
    Map<ComponentType, Function<ComponentPtr()>> component_factories_;
    Map<String, ComponentStyle> themes_;
    String current_theme_;
    
    bool initialized_;
    bool layout_dirty_;
    f32 animation_speed_;
    
    bool liquid_glass_enabled_;
    bool holographic_enabled_;
    bool quantum_enabled_;
    bool consciousness_enabled_;
    bool transcendent_enabled_;
    bool ultimate_effects_enabled_;
    
    Mutex mutex_;
    
    void update_component_recursive(ComponentPtr component, f64 delta_time);
    void render_component_recursive(ComponentPtr component, RendererPtr renderer);
    void layout_component_recursive(ComponentPtr component);
    
    ComponentPtr find_component_at_recursive(ComponentPtr component, const Point& position);
    ComponentPtr find_component_by_id_recursive(ComponentPtr component, u32 id);
    ComponentPtr find_component_by_name_recursive(ComponentPtr component, const String& name);
    
    void register_default_factories();
    void setup_default_theme();
    void apply_global_effects();
    
    void harmonize_ui_consciousness();
    void align_transcendent_interface();
    void perfect_ultimate_user_experience();
};

using UIManagerPtr = SharedPtr<UIManager>;

}

