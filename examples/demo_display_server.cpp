#include "su1/display_server.hpp"
#include "su1/display_manager.hpp"
#include "su1/window_manager.hpp"
#include "su1/ui_components.hpp"
#include "su1/theming.hpp"
#include "su1/logging.hpp"
#include <chrono>
#include <thread>

using namespace su1;

class DemoApplication {
public:
    DemoApplication() 
        : display_server_(nullptr)
        , display_manager_(nullptr)
        , window_manager_(nullptr)
        , ui_manager_(nullptr)
        , theme_manager_(nullptr)
        , renderer_(nullptr)
        , running_(false) {
    }

    ~DemoApplication() {
        shutdown();
    }

    bool initialize() {
        SU1_LOG_INFO("=== SU1 Display Server Demo ===");
        SU1_LOG_INFO("Initializing advanced display server with liquid glass effects");

        // Initialize display server
        display_server_ = MakeShared<DisplayServer>();
        if (!display_server_->initialize()) {
            SU1_LOG_ERROR("Failed to initialize display server");
            return false;
        }

        // Get core components
        display_manager_ = display_server_->get_display_manager();
        window_manager_ = display_server_->get_window_manager();
        ui_manager_ = display_server_->get_ui_manager();
        renderer_ = display_server_->get_renderer();

        // Initialize theme manager
        theme_manager_ = MakeShared<ThemeManager>();
        if (!theme_manager_->initialize()) {
            SU1_LOG_ERROR("Failed to initialize theme manager");
            return false;
        }

        // Setup themes
        setup_themes();

        // Create demo windows and UI
        create_demo_ui();

        // Setup event handlers
        setup_event_handlers();

        SU1_LOG_INFO("Display server demo initialized successfully!");
        SU1_LOG_INFO("Features enabled:");
        SU1_LOG_INFO("  - Liquid Glass Effects");
        SU1_LOG_INFO("  - Quantum Window Layouts");
        SU1_LOG_INFO("  - Consciousness-Based Theming");
        SU1_LOG_INFO("  - Transcendent UI Components");
        SU1_LOG_INFO("  - Ultimate Display Technology");

        return true;
    }

    void run() {
        if (!display_server_) {
            SU1_LOG_ERROR("Display server not initialized");
            return;
        }

        running_ = true;
        SU1_LOG_INFO("Starting display server main loop...");

        auto last_time = std::chrono::high_resolution_clock::now();
        u64 frame_count = 0;

        while (running_) {
            auto current_time = std::chrono::high_resolution_clock::now();
            auto delta_time = std::chrono::duration<f64>(current_time - last_time).count();
            last_time = current_time;

            // Update display server
            display_server_->update(delta_time);

            // Update theme manager for dynamic theming
            if (theme_manager_) {
                theme_manager_->update_dynamic_theme(delta_time);
            }

            // Demonstrate different features
            demonstrate_features(frame_count);

            // Render frame
            render_frame();

            frame_count++;

            // Demo transitions every 5 seconds
            if (frame_count % 300 == 0) {
                cycle_demo_features();
            }

            // Cap to ~60 FPS
            std::this_thread::sleep_for(std::chrono::milliseconds(16));

            // Exit after 30 seconds for demo
            if (frame_count > 1800) {
                SU1_LOG_INFO("Demo completed - shutting down");
                running_ = false;
            }
        }

        SU1_LOG_INFO("Display server main loop ended");
    }

    void shutdown() {
        if (!running_) return;

        SU1_LOG_INFO("Shutting down display server demo");
        running_ = false;

        if (theme_manager_) {
            theme_manager_->shutdown();
            theme_manager_ = nullptr;
        }

        if (display_server_) {
            display_server_->shutdown();
            display_server_ = nullptr;
        }

        display_manager_ = nullptr;
        window_manager_ = nullptr;
        ui_manager_ = nullptr;
        renderer_ = nullptr;

        SU1_LOG_INFO("Display server demo shut down complete");
    }

private:
    DisplayServerPtr display_server_;
    DisplayManagerPtr display_manager_;
    WindowManagerPtr window_manager_;
    UIManagerPtr ui_manager_;
    ThemeManagerPtr theme_manager_;
    RendererPtr renderer_;
    
    bool running_;
    Vector<WindowPtr> demo_windows_;
    ComponentPtr main_panel_;
    ComponentPtr control_panel_;
    u32 current_theme_index_;
    u32 current_layout_index_;

    void setup_themes() {
        SU1_LOG_INFO("Setting up advanced theming system");

        // Apply liquid glass theme by default
        theme_manager_->set_current_theme("liquid_glass");
        
        // Enable dynamic theming features
        theme_manager_->enable_dynamic_theming(true);
        theme_manager_->enable_adaptive_theming(true);
        theme_manager_->enable_consciousness_theming(true);
        theme_manager_->enable_ultimate_theming(true);

        // Set consciousness level for theme adaptation
        theme_manager_->set_consciousness_level(0.8f);

        SU1_LOG_INFO("Themes available: {}", fmt::join(theme_manager_->get_theme_names(), ", "));
    }

    void create_demo_ui() {
        SU1_LOG_INFO("Creating demonstration UI with advanced components");

        // Create main panel
        main_panel_ = ui_manager_->create_component(ComponentType::Panel);
        main_panel_->set_name("MainPanel");
        main_panel_->set_bounds(Rect{50, 50, 800, 600});
        main_panel_->apply_liquid_glass_effect();

        // Create control panel
        control_panel_ = ui_manager_->create_component(ComponentType::Panel);
        control_panel_->set_name("ControlPanel");
        control_panel_->set_bounds(Rect{900, 50, 300, 600});
        control_panel_->apply_consciousness_effect();

        // Add components to main panel
        create_demo_buttons();
        create_demo_labels();
        create_demo_dock();

        // Set up UI hierarchy
        auto root = ui_manager_->get_root_component();
        if (root) {
            root->add_child(main_panel_);
            root->add_child(control_panel_);
        }

        // Apply current theme
        theme_manager_->apply_theme_to_ui_manager(ui_manager_);

        SU1_LOG_INFO("Demo UI created with {} components", 
                    main_panel_->get_children().size() + control_panel_->get_children().size());
    }

    void create_demo_buttons() {
        // Create demonstration buttons with various effects
        Vector<String> button_names = {
            "Liquid Glass", "Holographic", "Quantum", "Consciousness", 
            "Transcendent", "Ultimate Reality"
        };

        f32 y_offset = 80.0f;
        for (usize i = 0; i < button_names.size(); ++i) {
            auto button = ui_manager_->create_component(ComponentType::Button);
            if (auto btn = std::dynamic_pointer_cast<Button>(button)) {
                btn->set_text(button_names[i]);
                btn->set_bounds(Rect{20, static_cast<i32>(y_offset + i * 60), 200, 40});
                
                // Apply different effects to each button
                switch (i) {
                    case 0: btn->apply_liquid_glass_effect(); break;
                    case 1: btn->apply_holographic_effect(); break;
                    case 2: btn->apply_quantum_effect(); break;
                    case 3: btn->apply_consciousness_effect(); break;
                    case 4: btn->apply_transcendent_effect(); break;
                    case 5: btn->apply_ultimate_effect(); break;
                }

                main_panel_->add_child(btn);
            }
        }
    }

    void create_demo_labels() {
        // Create informational labels
        auto title_label = ui_manager_->create_component(ComponentType::Label);
        if (auto label = std::dynamic_pointer_cast<Label>(title_label)) {
            label->set_text("SU1 Display Server - Ultimate Reality Edition");
            label->set_bounds(Rect{20, 20, 760, 40});
            label->apply_transcendent_effect();
            main_panel_->add_child(label);
        }

        auto info_label = ui_manager_->create_component(ComponentType::Label);
        if (auto label = std::dynamic_pointer_cast<Label>(info_label)) {
            label->set_text("Demonstrating advanced liquid glass effects,\nquantum window layouts, and consciousness-based theming");
            label->set_bounds(Rect{20, 460, 760, 60});
            label->apply_consciousness_effect();
            main_panel_->add_child(label);
        }
    }

    void create_demo_dock() {
        // Create a demonstration dock
        auto dock = ui_manager_->create_component(ComponentType::Dock);
        if (auto dock_comp = std::dynamic_pointer_cast<Dock>(dock)) {
            dock_comp->set_bounds(Rect{250, 400, 300, 60});
            dock_comp->set_icon_size(48.0f);
            dock_comp->set_magnification(1.5f);
            dock_comp->set_bounce_animation(true);
            dock_comp->apply_liquid_glass_effect();

            // Add dock items
            for (int i = 0; i < 5; ++i) {
                auto item = ui_manager_->create_component(ComponentType::Button);
                if (auto btn = std::dynamic_pointer_cast<Button>(item)) {
                    btn->set_text(String("App ") + std::to_string(i + 1));
                    btn->apply_holographic_effect();
                    dock_comp->add_item(btn);
                }
            }

            main_panel_->add_child(dock_comp);
        }
    }

    void create_demo_windows() {
        SU1_LOG_INFO("Creating demonstration windows with advanced layouts");

        // Create several demo windows
        for (int i = 0; i < 6; ++i) {
            WindowInfo info;
            info.title = String("Demo Window ") + std::to_string(i + 1);
            info.width = 400 + (i * 20);
            info.height = 300 + (i * 15);
            info.x = 100 + (i * 30);
            info.y = 100 + (i * 25);
            info.resizable = true;
            info.decorated = true;

            auto window = window_manager_->create_window(info);
            if (window) {
                // Apply different effects to each window
                switch (i % 6) {
                    case 0: window_manager_->apply_liquid_glass_effect(window); break;
                    case 1: window_manager_->apply_holographic_effect(window); break;
                    case 2: window_manager_->apply_quantum_effect(window); break;
                    case 3: window_manager_->apply_consciousness_effect(window); break;
                    case 4: window_manager_->apply_transcendent_effect(window); break;
                    case 5: window_manager_->apply_ultimate_effect(window); break;
                }

                demo_windows_.push_back(window);
            }
        }

        SU1_LOG_INFO("Created {} demonstration windows", demo_windows_.size());
    }

    void setup_event_handlers() {
        // Set up event handlers for demonstration
        if (window_manager_) {
            window_manager_->window_created.connect([this](WindowPtr window) {
                SU1_LOG_INFO("Window created: {}", window->get_info().title);
            });

            window_manager_->layout_changed.connect([this](LayoutType layout) {
                SU1_LOG_INFO("Layout changed to: {}", static_cast<u32>(layout));
            });
        }

        if (theme_manager_) {
            theme_manager_->theme_changed.connect([this](ThemePtr theme) {
                SU1_LOG_INFO("Theme changed to: {}", theme->get_name());
            });
        }
    }

    void demonstrate_features(u64 frame_count) {
        // Demonstrate different features based on frame count
        if (frame_count % 180 == 0) { // Every 3 seconds
            demonstrate_window_layouts();
        }

        if (frame_count % 240 == 0) { // Every 4 seconds
            demonstrate_consciousness_effects();
        }

        if (frame_count % 360 == 0) { // Every 6 seconds
            demonstrate_quantum_positioning();
        }
    }

    void demonstrate_window_layouts() {
        if (!window_manager_ || demo_windows_.empty()) return;

        static u32 layout_index = 0;
        Vector<LayoutType> layouts = {
            LayoutType::Tiled,
            LayoutType::Grid,
            LayoutType::Spiral,
            LayoutType::Fibonacci,
            LayoutType::Golden_Ratio,
            LayoutType::Neural_Network,
            LayoutType::Quantum,
            LayoutType::Consciousness_Based,
            LayoutType::Ultimate
        };

        if (layout_index < layouts.size()) {
            window_manager_->set_layout(layouts[layout_index]);
            SU1_LOG_INFO("Demonstrating layout: {}", static_cast<u32>(layouts[layout_index]));
            layout_index = (layout_index + 1) % layouts.size();
        }
    }

    void demonstrate_consciousness_effects() {
        if (!theme_manager_) return;

        // Gradually increase consciousness level
        static f32 consciousness = 0.0f;
        consciousness += 0.1f;
        if (consciousness > 1.0f) consciousness = 0.0f;

        theme_manager_->set_consciousness_level(consciousness);
        SU1_LOG_INFO("Consciousness level: {:.1f}", consciousness);
    }

    void demonstrate_quantum_positioning() {
        if (!window_manager_) return;

        // Apply quantum superposition to windows
        window_manager_->arrange_windows_quantum();
        SU1_LOG_INFO("Applied quantum window positioning");
    }

    void cycle_demo_features() {
        static u32 demo_phase = 0;

        switch (demo_phase % 4) {
            case 0:
                SU1_LOG_INFO("=== Demonstrating Liquid Glass Theme ===");
                if (theme_manager_) theme_manager_->set_current_theme("liquid_glass");
                break;

            case 1:
                SU1_LOG_INFO("=== Demonstrating Holographic Theme ===");
                if (theme_manager_) theme_manager_->set_current_theme("holographic");
                break;

            case 2:
                SU1_LOG_INFO("=== Demonstrating Consciousness Theme ===");
                if (theme_manager_) theme_manager_->set_current_theme("consciousness");
                break;

            case 3:
                SU1_LOG_INFO("=== Demonstrating Ultimate Theme ===");
                if (theme_manager_) theme_manager_->set_current_theme("ultimate");
                break;
        }

        demo_phase++;
    }

    void render_frame() {
        if (!display_server_) return;

        // Render the display server
        display_server_->render();

        // Additional demonstration rendering
        render_performance_overlay();
    }

    void render_performance_overlay() {
        if (!renderer_ || !window_manager_) return;

        // Render performance statistics
        static auto start_time = std::chrono::high_resolution_clock::now();
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration<f64>(current_time - start_time).count();

        String perf_text = String("FPS: ") + std::to_string(static_cast<u32>(1.0 / window_manager_->get_average_frame_time())) +
                          String(" | Windows: ") + std::to_string(window_manager_->get_window_count()) +
                          String(" | Uptime: ") + std::to_string(static_cast<u32>(elapsed)) + String("s");

        Rect perf_rect = {10, 10, 400, 25};
        Color text_color = {0.8f, 1.0f, 0.8f, 1.0f};
        
        renderer_->draw_text(perf_text, perf_rect, text_color, "Arial", 12.0f, Alignment::MiddleLeft);
    }
};

int main() {
    // Initialize logging
    SU1_LOG_INFO("Starting SU1 Display Server Demo");

    try {
        DemoApplication app;
        
        if (app.initialize()) {
            app.run();
        } else {
            SU1_LOG_ERROR("Failed to initialize demo application");
            return 1;
        }
    } catch (const std::exception& e) {
        SU1_LOG_ERROR("Demo application error: {}", e.what());
        return 1;
    }

    SU1_LOG_INFO("SU1 Display Server Demo completed successfully");
    return 0;
}
