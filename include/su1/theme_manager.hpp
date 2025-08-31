#pragma once

#include "su1/core.hpp"
#include "su1/manager_base.hpp"

namespace su1 {

class Theme;
using ThemePtr = std::shared_ptr<Theme>;

class ThemeManager : public Manager {
public:
    ThemeManager();
    ~ThemeManager() override;

    bool initialize() override;
    void shutdown() override;

    void set_current_theme(const String& name);
    ThemePtr get_current_theme() const;
    std::vector<String> get_theme_names() const;

    void enable_dynamic_theming(bool enable);
    void enable_adaptive_theming(bool enable);
    void enable_consciousness_theming(bool enable);
    void enable_ultimate_theming(bool enable);

    void set_consciousness_level(f32 level);
    f32 get_consciousness_level() const;

    void apply_theme_to_ui_manager(class UIManager* ui_manager);

    // Signals
    Signal<const String&> theme_changed;

private:
    std::unordered_map<String, ThemePtr> themes_;
    ThemePtr current_theme_;
    bool dynamic_theming_enabled_;
    bool adaptive_theming_enabled_;
    bool consciousness_theming_enabled_;
    bool ultimate_theming_enabled_;
    f32 consciousness_level_;
};

} // namespace su1
