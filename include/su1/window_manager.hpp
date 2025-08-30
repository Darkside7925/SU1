#pragma once

#include "su1/core.hpp"
#include "su1/window.hpp"
#include "su1/display_manager.hpp"

namespace su1 {

enum class LayoutType : u32 {
    Floating,
    Tiled,
    Stacked,
    Tabbed,
    Fullscreen,
    Grid,
    Spiral,
    Binary_Space_Partition,
    Master_Stack,
    Fibonacci,
    Golden_Ratio,
    Neural_Network,
    Quantum,
    Consciousness_Based,
    Ultimate
};

enum class WindowState : u32 {
    Normal,
    Minimized,
    Maximized,
    Fullscreen,
    Shaded,
    Sticky,
    Modal,
    Above,
    Below,
    Hidden,
    Focused,
    Active,
    Urgent
};

enum class WindowAnimationType : u32 {
    None,
    Fade,
    Scale,
    Slide,
    Bounce,
    Elastic,
    Wobble,
    Glow,
    Ripple,
    Transcendence,
    Quantum_Tunneling,
    Reality_Warping
};

struct WindowNode {
    WindowPtr window;
    WindowNode* parent;
    WindowNode* left;
    WindowNode* right;
    Rect bounds;
    f32 split_ratio;
    bool is_horizontal_split;
    bool is_leaf;
    LayoutType layout_type;
    Vector<WindowPtr> stacked_windows;
    
    WindowNode();
    ~WindowNode();
    
    bool is_container() const { return !is_leaf; }
    bool has_window() const { return window != nullptr; }
    Vector<WindowPtr> get_all_windows() const;
    void add_window(WindowPtr window);
    void remove_window(WindowPtr window);
    WindowNode* find_window_node(WindowPtr window);
    void calculate_bounds(const Rect& parent_bounds);
    void balance_tree();
    void optimize_layout();
};

struct Layout {
    LayoutType type;
    String name;
    WindowNode* root;
    Rect workspace_bounds;
    Vector<WindowPtr> floating_windows;
    Vector<WindowPtr> fullscreen_windows;
    WindowPtr focused_window;
    bool auto_balance;
    bool smart_gaps;
    f32 gap_size;
    f32 border_size;
    Color gap_color;
    Color border_color;
    
    Layout(LayoutType type = LayoutType::Floating);
    ~Layout();
    
    void add_window(WindowPtr window, const Point& position = Point{});
    void remove_window(WindowPtr window);
    void move_window(WindowPtr window, const Point& position);
    void resize_window(WindowPtr window, const Size& size);
    void focus_window(WindowPtr window);
    void arrange_windows();
    void update_layout();
    
    WindowPtr get_window_at(const Point& position) const;
    Vector<WindowPtr> get_all_windows() const;
    Vector<WindowPtr> get_visible_windows() const;
    Rect get_window_bounds(WindowPtr window) const;
    
    void apply_tiling();
    void apply_stacking();
    void apply_grid();
    void apply_spiral();
    void apply_fibonacci();
    void apply_golden_ratio();
    void apply_neural_network_layout();
    void apply_quantum_layout();
    void apply_consciousness_layout();
    void apply_ultimate_layout();
};

using LayoutPtr = SharedPtr<Layout>;

struct Workspace {
    u32 id;
    String name;
    LayoutPtr layout;
    Vector<WindowPtr> windows;
    WindowPtr focused_window;
    DisplayPtr display;
    Rect bounds;
    bool is_active;
    bool is_visible;
    f32 opacity;
    Color background_color;
    
    Workspace(u32 id, const String& name = "");
    ~Workspace();
    
    void add_window(WindowPtr window);
    void remove_window(WindowPtr window);
    void focus_window(WindowPtr window);
    void arrange_windows();
    void update_layout();
    
    void set_layout(LayoutType type);
    void set_layout(LayoutPtr layout);
    
    WindowPtr get_window_at(const Point& position) const;
    Vector<WindowPtr> get_visible_windows() const;
    
    void activate();
    void deactivate();
    void show();
    void hide();
};

using WorkspacePtr = SharedPtr<Workspace>;

class WindowManager {
public:
    WindowManager();
    ~WindowManager();

    bool initialize();
    void shutdown();

    // Window management
    WindowPtr create_window(const WindowInfo& info);
    bool destroy_window(WindowPtr window);
    bool add_window(WindowPtr window);
    bool remove_window(WindowPtr window);
    
    void show_window(WindowPtr window);
    void hide_window(WindowPtr window);
    void minimize_window(WindowPtr window);
    void maximize_window(WindowPtr window);
    void restore_window(WindowPtr window);
    void fullscreen_window(WindowPtr window);
    void close_window(WindowPtr window);
    
    void move_window(WindowPtr window, const Point& position);
    void resize_window(WindowPtr window, const Size& size);
    void set_window_bounds(WindowPtr window, const Rect& bounds);
    
    void focus_window(WindowPtr window);
    void raise_window(WindowPtr window);
    void lower_window(WindowPtr window);
    
    void set_window_state(WindowPtr window, WindowState state);
    void add_window_state(WindowPtr window, WindowState state);
    void remove_window_state(WindowPtr window, WindowState state);
    bool has_window_state(WindowPtr window, WindowState state) const;
    
    void set_window_opacity(WindowPtr window, f32 opacity);
    void set_window_always_on_top(WindowPtr window, bool on_top);
    
    // Layout management
    void set_layout(LayoutType type);
    void set_layout(LayoutPtr layout);
    LayoutPtr get_current_layout() const { return current_layout_; }
    
    void tile_windows();
    void float_windows();
    void stack_windows();
    void cascade_windows();
    void arrange_windows_grid();
    void arrange_windows_spiral();
    void arrange_windows_fibonacci();
    void arrange_windows_golden_ratio();
    void arrange_windows_neural();
    void arrange_windows_quantum();
    void arrange_windows_consciousness();
    void arrange_windows_ultimate();
    
    // Workspace management
    WorkspacePtr create_workspace(const String& name = "");
    bool destroy_workspace(WorkspacePtr workspace);
    void switch_to_workspace(WorkspacePtr workspace);
    void switch_to_workspace(u32 id);
    void move_window_to_workspace(WindowPtr window, WorkspacePtr workspace);
    
    WorkspacePtr get_current_workspace() const { return current_workspace_; }
    WorkspacePtr get_workspace(u32 id) const;
    Vector<WorkspacePtr> get_all_workspaces() const { return workspaces_; }
    
    // Focus management
    void focus_next_window();
    void focus_previous_window();
    void cycle_windows();
    
    WindowPtr get_focused_window() const { return focused_window_; }
    void set_focus_follows_mouse(bool follows);
    void set_auto_raise(bool auto_raise);
    void set_click_to_focus(bool click_to_focus);
    
    // Window finding
    WindowPtr get_window_at(const Point& position) const;
    WindowPtr get_window_by_id(u32 id) const;
    WindowPtr find_window(const String& title) const;
    Vector<WindowPtr> find_windows(const String& class_name) const;
    Vector<WindowPtr> get_all_windows() const;
    Vector<WindowPtr> get_visible_windows() const;
    Vector<WindowPtr> get_minimized_windows() const;
    
    // Animation and effects
    void animate_window(WindowPtr window, WindowAnimationType animation, f32 duration = 0.3f);
    void set_window_animation_enabled(bool enabled) { animations_enabled_ = enabled; }
    void set_animation_duration(f32 duration) { animation_duration_ = duration; }
    
    void apply_liquid_glass_effect(WindowPtr window);
    void apply_holographic_effect(WindowPtr window);
    void apply_quantum_effect(WindowPtr window);
    void apply_consciousness_effect(WindowPtr window);
    void apply_transcendent_effect(WindowPtr window);
    void apply_ultimate_effect(WindowPtr window);
    
    // Configuration
    void set_gap_size(f32 size);
    void set_border_size(f32 size);
    void set_snap_threshold(f32 threshold);
    
    void enable_smart_gaps(bool enabled);
    void enable_auto_arrange(bool enabled);
    void enable_window_snapping(bool enabled);
    
    // Performance monitoring
    f64 get_average_frame_time() const { return average_frame_time_; }
    u64 get_frame_count() const { return frame_count_; }
    u64 get_window_count() const { return windows_.size(); }
    
    // Events
    Signal<void(WindowPtr)> window_created;
    Signal<void(WindowPtr)> window_destroyed;
    Signal<void(WindowPtr)> window_focused;
    Signal<void(WindowPtr)> window_moved;
    Signal<void(WindowPtr)> window_resized;
    Signal<void(WorkspacePtr)> workspace_switched;
    Signal<void(LayoutType)> layout_changed;

    bool is_initialized() const { return initialized_; }

private:
    Vector<WindowPtr> windows_;
    Map<u32, WindowPtr> window_map_;
    Vector<WorkspacePtr> workspaces_;
    Map<u32, WorkspacePtr> workspace_map_;
    
    WindowPtr focused_window_;
    WorkspacePtr current_workspace_;
    LayoutPtr current_layout_;
    
    u32 next_window_id_;
    u32 next_workspace_id_;
    
    bool initialized_;
    bool animations_enabled_;
    bool smart_gaps_enabled_;
    bool auto_arrange_enabled_;
    bool window_snapping_enabled_;
    bool focus_follows_mouse_;
    bool auto_raise_;
    bool click_to_focus_;
    
    f32 gap_size_;
    f32 border_size_;
    f32 snap_threshold_;
    f32 animation_duration_;
    
    Timer frame_timer_;
    u64 frame_count_;
    f64 total_frame_time_;
    f64 average_frame_time_;
    
    Mutex mutex_;
    Thread layout_thread_;
    Atomic<bool> should_update_layout_;
    
    void update_layout();
    void update_window_states();
    void update_focus_chain();
    
    void apply_window_placement(WindowPtr window);
    void apply_window_snapping(WindowPtr window);
    
    void calculate_optimal_layout();
    void optimize_window_arrangement();
    void balance_window_tree();
    
    void render_window_decorations(WindowPtr window);
    void render_window_shadow(WindowPtr window);
    void render_window_border(WindowPtr window);
    
    void start_window_animation(WindowPtr window, WindowAnimationType type, f32 duration);
    void update_window_animation(WindowPtr window, f64 delta_time);
    
    bool is_window_tiled(WindowPtr window) const;
    bool is_window_floating(WindowPtr window) const;
    
    void tile_window(WindowPtr window);
    void float_window(WindowPtr window);
    
    void create_default_workspace();
    void setup_default_layout();
    
    Point calculate_optimal_position(WindowPtr window);
    Size calculate_optimal_size(WindowPtr window);
    
    void apply_consciousness_layout();
    void apply_transcendent_arrangement();
    void apply_quantum_positioning();
    void apply_ultimate_perfection();
    
    f32 calculate_golden_ratio_position(u32 index, u32 total);
    f32 calculate_fibonacci_position(u32 index);
    Point calculate_fractal_position(u32 index, u32 depth);
    Rect calculate_neural_bounds(WindowPtr window);
    Rect calculate_quantum_superposition(WindowPtr window);
    
    void harmonize_consciousness_field();
    void align_transcendent_geometry();
    void optimize_quantum_state();
    void perfect_ultimate_reality();
};

using WindowManagerPtr = SharedPtr<WindowManager>;

}

#endif