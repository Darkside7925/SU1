#include "su1/ui_components.hpp"
#include "su1/core.hpp"
#include <iostream>
#include <memory>

namespace su1 {

UIComponents::UIComponents()
    : initialized_(false) {
    std::cout << "[UI_COMPONENTS] UIComponents created" << std::endl;
}

UIComponents::~UIComponents() {
    if (initialized_) {
        shutdown();
    }
}

bool UIComponents::initialize() {
    std::cout << "[UI_COMPONENTS] Initializing UI components..." << std::endl;

    initialized_ = true;

    std::cout << "[UI_COMPONENTS] UI components initialized successfully" << std::endl;
    return true;
}

void UIComponents::shutdown() {
    if (initialized_) {
        std::cout << "[UI_COMPONENTS] Shutting down UI components..." << std::endl;

        initialized_ = false;
        std::cout << "[UI_COMPONENTS] UI components shutdown complete" << std::endl;
    }
}

bool UIComponents::is_initialized() const {
    return initialized_;
}

} // namespace su1
    , foreground_color{1.0f, 1.0f, 1.0f, 1.0f}
    , border_color{0.4f, 0.4f, 0.4f, 1.0f}
    , shadow_color{0.0f, 0.0f, 0.0f, 0.5f}
    , highlight_color{0.6f, 0.8f, 1.0f, 1.0f}
    , selection_color{0.2f, 0.6f, 1.0f, 0.8f}
    , focus_color{0.4f, 0.8f, 1.0f, 1.0f}
    , hover_color{0.3f, 0.3f, 0.3f, 1.0f}
    , pressed_color{0.1f, 0.1f, 0.1f, 1.0f}
    , disabled_color{0.5f, 0.5f, 0.5f, 0.5f}
    , opacity(1.0f)
    , border_width(1.0f)
    , corner_radius(4.0f)
    , shadow_radius(4.0f)
    , shadow_offset_x(2.0f)
    , shadow_offset_y(2.0f)
    , highlight_intensity(0.3f)
    , glow_radius(0.0f)
    , blur_radius(0.0f)
    , saturation(1.0f)
    , brightness(1.0f)
    , contrast(1.0f)
    , hue_shift(0.0f)
    , font_family("Arial")
    , font_size(12.0f)
    , font_weight(400.0f)
    , font_italic(false)
    , font_underline(false)
    , font_strikethrough(false)
    , text_alignment(Alignment::MiddleLeft)
    , line_height(1.2f)
    , letter_spacing(0.0f)
    , word_spacing(0.0f)
    , padding_top(4.0f)
    , padding_right(4.0f)
    , padding_bottom(4.0f)
    , padding_left(4.0f)
    , margin_top(0.0f)
    , margin_right(0.0f)
    , margin_bottom(0.0f)
    , margin_left(0.0f)
    , visible(true)
    , enabled(true)
    , focusable(true)
    , selectable(false)
    , draggable(false)
    , resizable(false)
    , scrollable(false)
    , clippable(true)
    , animatable(true) {
}

void ComponentStyle::apply_liquid_glass_effect() {
    background_color.a = 0.7f;
    blur_radius = 10.0f;
    glow_radius = 5.0f;
    highlight_intensity = 0.6f;
    corner_radius = 12.0f;
    border_width = 1.5f;
    border_color = Color{1.0f, 1.0f, 1.0f, 0.3f};
    shadow_radius = 8.0f;
    shadow_color = Color{0.0f, 0.0f, 0.0f, 0.3f};
}

void ComponentStyle::apply_holographic_effect() {
    background_color = Color{0.8f, 0.9f, 1.0f, 0.4f};
    border_color = Color{0.6f, 0.8f, 1.0f, 0.8f};
    glow_radius = 15.0f;
    highlight_intensity = 0.8f;
    hue_shift = 0.1f;
    saturation = 1.3f;
}

void ComponentStyle::apply_quantum_effect() {
    opacity = 0.85f;
    glow_radius = 8.0f;
    blur_radius = 3.0f;
    background_color = Color{0.4f, 0.6f, 0.9f, 0.6f};
    border_color = Color{0.6f, 0.8f, 1.0f, 0.9f};
    // Quantum superposition visual effects would be handled in rendering
}

void ComponentStyle::apply_consciousness_effect() {
    background_color = Color{0.9f, 0.8f, 1.0f, 0.5f};
    glow_radius = 20.0f;
    highlight_intensity = 0.9f;
    corner_radius = 16.0f;
    // Consciousness field visualization would be handled in rendering
}

void ComponentStyle::apply_transcendent_effect() {
    background_color = Color{1.0f, 1.0f, 1.0f, 0.3f};
    border_color = Color{1.0f, 1.0f, 1.0f, 0.7f};
    glow_radius = 25.0f;
    highlight_intensity = 1.0f;
    brightness = 1.2f;
    saturation = 0.8f;
}

void ComponentStyle::apply_ultimate_effect() {
    apply_liquid_glass_effect();
    apply_holographic_effect();
    apply_quantum_effect();
    apply_consciousness_effect();
    apply_transcendent_effect();
    
    // Ultimate combination with perfect balance
    opacity = 0.8f;
    glow_radius = 30.0f;
    blur_radius = 15.0f;
    corner_radius = 20.0f;
    highlight_intensity = 1.0f;
    brightness = 1.1f;
    saturation = 1.1f;
    contrast = 1.05f;
}

// Component implementation
u32 Component::next_id_ = 1;

Component::Component(ComponentType type)
    : type_(type)
    , id_(next_id_++)
    , name_("Component_" + std::to_string(id_))
    , bounds_{0, 0, 100, 30}
    , style_{}
    , parent_(nullptr)
    , focused_(false)
    , hovered_(false)
    , pressed_(false) {
    SU1_PROFILE_FUNCTION();
}

Component::~Component() {
    SU1_PROFILE_FUNCTION();
    
    // Remove from parent
    if (parent_) {
        parent_->remove_child(shared_from_this());
    }
    
    // Clear children
    clear_children();
}

void Component::set_bounds(const Rect& bounds) {
    if (bounds_ != bounds) {
        Rect old_bounds = bounds_;
        bounds_ = bounds;
        
        // Emit signals
        if (old_bounds.width != bounds.width || old_bounds.height != bounds.height) {
            size_changed.emit(this, Size{bounds.width, bounds.height});
            on_size_changed();
        }
        
        if (old_bounds.x != bounds.x || old_bounds.y != bounds.y) {
            position_changed.emit(this, Point{bounds.x, bounds.y});
            on_position_changed();
        }
        
        invalidate_layout();
    }
}

void Component::set_position(const Point& position) {
    set_bounds(Rect{position.x, position.y, bounds_.width, bounds_.height});
}

void Component::set_size(const Size& size) {
    set_bounds(Rect{bounds_.x, bounds_.y, size.width, size.height});
}

void Component::set_visible(bool visible) {
    if (style_.visible != visible) {
        style_.visible = visible;
        on_style_changed();
        invalidate_render();
    }
}

void Component::set_focused(bool focused) {
    if (focused_ != focused) {
        focused_ = focused;
        
        if (focused) {
            focus_gained.emit(this);
            on_focus_gained();
        } else {
            focus_lost.emit(this);
            on_focus_lost();
        }
        
        invalidate_render();
    }
}

void Component::set_parent(Component* parent) {
    if (parent_ != parent) {
        if (parent_) {
            parent_->remove_child(shared_from_this());
        }
        
        parent_ = parent;
        
        if (parent_) {
            parent_->add_child(shared_from_this());
        }
    }
}

void Component::add_child(SharedPtr<Component> child) {
    if (!child || child->parent_ == this) {
        return;
    }
    
    // Remove from previous parent
    if (child->parent_) {
        child->parent_->remove_child(child);
    }
    
    children_.push_back(child);
    child->parent_ = this;
    
    invalidate_layout();
}

void Component::remove_child(SharedPtr<Component> child) {
    if (!child) {
        return;
    }
    
    auto it = std::find(children_.begin(), children_.end(), child);
    if (it != children_.end()) {
        (*it)->parent_ = nullptr;
        children_.erase(it);
        invalidate_layout();
    }
}

void Component::clear_children() {
    for (auto& child : children_) {
        child->parent_ = nullptr;
    }
    children_.clear();
    invalidate_layout();
}

SharedPtr<Component> Component::find_child(const String& name) const {
    for (const auto& child : children_) {
        if (child->name_ == name) {
            return child;
        }
        
        // Search recursively
        auto found = child->find_child(name);
        if (found) {
            return found;
        }
    }
    
    return nullptr;
}

SharedPtr<Component> Component::find_child_by_id(u32 id) const {
    for (const auto& child : children_) {
        if (child->id_ == id) {
            return child;
        }
        
        // Search recursively
        auto found = child->find_child_by_id(id);
        if (found) {
            return found;
        }
    }
    
    return nullptr;
}

Vector<SharedPtr<Component>> Component::find_children_by_type(ComponentType type) const {
    Vector<SharedPtr<Component>> result;
    
    for (const auto& child : children_) {
        if (child->type_ == type) {
            result.push_back(child);
        }
        
        // Search recursively
        auto child_results = child->find_children_by_type(type);
        result.insert(result.end(), child_results.begin(), child_results.end());
    }
    
    return result;
}

void Component::set_property(const String& name, const Any& value) {
    properties_[name] = value;
    property_changed.emit(this, name);
    on_property_changed(name);
}

Any Component::get_property(const String& name) const {
    auto it = properties_.find(name);
    return (it != properties_.end()) ? it->second : Any{};
}

bool Component::has_property(const String& name) const {
    return properties_.find(name) != properties_.end();
}

void Component::update(f64 delta_time) {
    SU1_PROFILE_FUNCTION();
    
    if (!style_.visible) {
        return;
    }
    
    update_animations(delta_time);
    
    // Update children
    for (auto& child : children_) {
        child->update(delta_time);
    }
}

void Component::render(RendererPtr renderer) {
    SU1_PROFILE_FUNCTION();
    
    if (!style_.visible || !renderer) {
        return;
    }
    
    // Set clipping if enabled
    if (style_.clippable) {
        renderer->push_clip_rect(bounds_);
    }
    
    // Render component layers
    render_shadow(renderer);
    render_background(renderer);
    render_content(renderer);
    render_border(renderer);
    render_glow(renderer);
    render_overlay(renderer);
    
    // Render children
    for (auto& child : children_) {
        child->render(renderer);
    }
    
    // Restore clipping
    if (style_.clippable) {
        renderer->pop_clip_rect();
    }
}

void Component::layout() {
    // Base layout implementation - override in derived classes
    for (auto& child : children_) {
        child->layout();
    }
}

bool Component::handle_mouse_move(const Point& position) {
    if (!style_.visible || !style_.enabled) {
        return false;
    }
    
    update_hover_state(position);
    
    // Handle children first (reverse order for proper z-order)
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if ((*it)->handle_mouse_move(position)) {
            return true;
        }
    }
    
    return false;
}

bool Component::handle_mouse_down(const Point& position, u32 button) {
    if (!style_.visible || !style_.enabled) {
        return false;
    }
    
    // Handle children first (reverse order for proper z-order)
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if ((*it)->handle_mouse_down(position, button)) {
            return true;
        }
    }
    
    // Check if click is within bounds
    if (position.x >= bounds_.x && position.x < bounds_.x + static_cast<i32>(bounds_.width) &&
        position.y >= bounds_.y && position.y < bounds_.y + static_cast<i32>(bounds_.height)) {
        
        pressed_ = true;
        
        if (button == 1) { // Left click
            if (style_.focusable) {
                set_focused(true);
            }
            clicked.emit(this);
        } else if (button == 3) { // Right click
            right_clicked.emit(this);
        }
        
        return true;
    }
    
    return false;
}

bool Component::handle_mouse_up(const Point& position, u32 button) {
    if (!style_.visible || !style_.enabled) {
        return false;
    }
    
    bool was_pressed = pressed_;
    pressed_ = false;
    
    // Handle children first
    for (auto it = children_.rbegin(); it != children_.rend(); ++it) {
        if ((*it)->handle_mouse_up(position, button)) {
            return true;
        }
    }
    
    return was_pressed;
}

bool Component::handle_key_down(u32 key, u32 modifiers) {
    if (!style_.visible || !style_.enabled || !focused_) {
        return false;
    }
    
    // Handle children first
    for (auto& child : children_) {
        if (child->handle_key_down(key, modifiers)) {
            return true;
        }
    }
    
    return false;
}

void Component::on_mouse_enter() {
    mouse_entered.emit(this);
}

void Component::on_mouse_leave() {
    mouse_left.emit(this);
}

void Component::on_focus_gained() {
    // Override in derived classes
}

void Component::on_focus_lost() {
    // Override in derived classes
}

void Component::on_size_changed() {
    layout();
}

void Component::on_position_changed() {
    // Override in derived classes
}

void Component::on_style_changed() {
    invalidate_render();
}

void Component::on_property_changed(const String& name) {
    // Override in derived classes
}

void Component::apply_liquid_glass_effect() {
    style_.apply_liquid_glass_effect();
    on_style_changed();
}

void Component::apply_holographic_effect() {
    style_.apply_holographic_effect();
    on_style_changed();
}

void Component::apply_quantum_effect() {
    style_.apply_quantum_effect();
    on_style_changed();
}

void Component::apply_consciousness_effect() {
    style_.apply_consciousness_effect();
    on_style_changed();
}

void Component::apply_transcendent_effect() {
    style_.apply_transcendent_effect();
    on_style_changed();
}

void Component::apply_ultimate_effect() {
    style_.apply_ultimate_effect();
    on_style_changed();
}

void Component::render_background(RendererPtr renderer) {
    if (style_.background_color.a > 0.0f) {
        if (style_.corner_radius > 0.0f) {
            renderer->fill_rounded_rect(bounds_, style_.corner_radius, style_.background_color);
        } else {
            renderer->fill_rect(bounds_, style_.background_color);
        }
    }
}

void Component::render_border(RendererPtr renderer) {
    if (style_.border_width > 0.0f && style_.border_color.a > 0.0f) {
        if (style_.corner_radius > 0.0f) {
            renderer->draw_rounded_rect(bounds_, style_.corner_radius, style_.border_color, style_.border_width);
        } else {
            renderer->draw_rect(bounds_, style_.border_color, style_.border_width);
        }
    }
}

void Component::render_shadow(RendererPtr renderer) {
    if (style_.shadow_radius > 0.0f && style_.shadow_color.a > 0.0f) {
        Rect shadow_rect = {
            bounds_.x + static_cast<i32>(style_.shadow_offset_x),
            bounds_.y + static_cast<i32>(style_.shadow_offset_y),
            bounds_.width,
            bounds_.height
        };
        
        renderer->fill_rounded_rect_with_shadow(shadow_rect, style_.corner_radius, 
                                              style_.shadow_color, style_.shadow_radius);
    }
}

void Component::render_glow(RendererPtr renderer) {
    if (style_.glow_radius > 0.0f) {
        Color glow_color = focused_ ? style_.focus_color : 
                          hovered_ ? style_.hover_color : style_.highlight_color;
        
        if (glow_color.a > 0.0f) {
            renderer->fill_rounded_rect_with_glow(bounds_, style_.corner_radius, 
                                                glow_color, style_.glow_radius);
        }
    }
}

void Component::render_content(RendererPtr renderer) {
    // Override in derived classes
}

void Component::render_overlay(RendererPtr renderer) {
    // Render focus indicator
    if (focused_ && style_.focus_color.a > 0.0f) {
        Rect focus_rect = {
            bounds_.x - 2,
            bounds_.y - 2,
            bounds_.width + 4,
            bounds_.height + 4
        };
        
        renderer->draw_rounded_rect(focus_rect, style_.corner_radius + 2, 
                                  style_.focus_color, 2.0f);
    }
}

void Component::update_hover_state(const Point& position) {
    bool in_bounds = (position.x >= bounds_.x && position.x < bounds_.x + static_cast<i32>(bounds_.width) &&
                      position.y >= bounds_.y && position.y < bounds_.y + static_cast<i32>(bounds_.height));
    
    if (in_bounds && !hovered_) {
        hovered_ = true;
        on_mouse_enter();
    } else if (!in_bounds && hovered_) {
        hovered_ = false;
        on_mouse_leave();
    }
}

void Component::update_animations(f64 delta_time) {
    // Update component animations
    // Animation system would be implemented here
}

void Component::invalidate_layout() {
    // Mark layout as dirty
}

void Component::invalidate_render() {
    // Mark render as dirty
}

// Panel implementation
Panel::Panel()
    : Component(ComponentType::Panel)
    , layout_mode_(LayoutMode::Flow)
    , orientation_(Orientation::Vertical)
    , spacing_(4.0f)
    , auto_resize_(false) {
}

Panel::~Panel() = default;

void Panel::layout() {
    switch (layout_mode_) {
        case LayoutMode::Fixed:
            layout_fixed();
            break;
        case LayoutMode::Flow:
            layout_flow();
            break;
        case LayoutMode::Grid:
            layout_grid();
            break;
        case LayoutMode::Flex:
            layout_flex();
            break;
        case LayoutMode::Stack:
            layout_stack();
            break;
        case LayoutMode::Quantum:
            layout_quantum();
            break;
        case LayoutMode::Consciousness:
            layout_consciousness();
            break;
        case LayoutMode::Ultimate:
            layout_ultimate();
            break;
        default:
            layout_flow();
            break;
    }
    
    // Layout children
    Component::layout();
}

void Panel::render(RendererPtr renderer) {
    Component::render(renderer);
}

void Panel::layout_fixed() {
    // Fixed layout - children positioned manually
    // No automatic positioning
}

void Panel::layout_flow() {
    if (children_.empty()) return;
    
    f32 current_x = bounds_.x + style_.padding_left;
    f32 current_y = bounds_.y + style_.padding_top;
    f32 max_height = 0.0f;
    
    for (auto& child : children_) {
        if (!child->is_visible()) continue;
        
        Size child_size = child->get_size();
        
        if (orientation_ == Orientation::Horizontal) {
            // Check if we need to wrap to next line
            if (current_x + child_size.width > bounds_.x + bounds_.width - style_.padding_right) {
                current_x = bounds_.x + style_.padding_left;
                current_y += max_height + spacing_;
                max_height = 0.0f;
            }
            
            child->set_position(Point{static_cast<i32>(current_x), static_cast<i32>(current_y)});
            current_x += child_size.width + spacing_;
            max_height = std::max(max_height, static_cast<f32>(child_size.height));
        } else {
            child->set_position(Point{static_cast<i32>(current_x), static_cast<i32>(current_y)});
            current_y += child_size.height + spacing_;
        }
    }
}

void Panel::layout_grid() {
    if (children_.empty()) return;
    
    // Calculate grid dimensions
    u32 cols = static_cast<u32>(std::ceil(std::sqrt(children_.size())));
    u32 rows = static_cast<u32>(std::ceil(static_cast<f32>(children_.size()) / static_cast<f32>(cols)));
    
    f32 available_width = bounds_.width - style_.padding_left - style_.padding_right;
    f32 available_height = bounds_.height - style_.padding_top - style_.padding_bottom;
    
    f32 cell_width = (available_width - (cols - 1) * spacing_) / cols;
    f32 cell_height = (available_height - (rows - 1) * spacing_) / rows;
    
    for (usize i = 0; i < children_.size(); ++i) {
        if (!children_[i]->is_visible()) continue;
        
        u32 col = static_cast<u32>(i % cols);
        u32 row = static_cast<u32>(i / cols);
        
        f32 x = bounds_.x + style_.padding_left + col * (cell_width + spacing_);
        f32 y = bounds_.y + style_.padding_top + row * (cell_height + spacing_);
        
        children_[i]->set_bounds(Rect{
            static_cast<i32>(x),
            static_cast<i32>(y),
            static_cast<u32>(cell_width),
            static_cast<u32>(cell_height)
        });
    }
}

void Panel::layout_flex() {
    // Flexbox-style layout
    layout_flow(); // Simplified implementation
}

void Panel::layout_stack() {
    // Stack layout - all children same size
    for (auto& child : children_) {
        if (!child->is_visible()) continue;
        
        Rect child_bounds = {
            bounds_.x + static_cast<i32>(style_.padding_left),
            bounds_.y + static_cast<i32>(style_.padding_top),
            bounds_.width - static_cast<u32>(style_.padding_left + style_.padding_right),
            bounds_.height - static_cast<u32>(style_.padding_top + style_.padding_bottom)
        };
        
        child->set_bounds(child_bounds);
    }
}

void Panel::layout_quantum() {
    // Quantum superposition layout
    for (usize i = 0; i < children_.size(); ++i) {
        if (!children_[i]->is_visible()) continue;
        
        // Quantum probability distribution
        f32 phase = static_cast<f32>(i) * 2.0f * 3.14159f / static_cast<f32>(children_.size());
        f32 amplitude = std::cos(phase);
        f32 probability = amplitude * amplitude;
        
        f32 x_ratio = (probability + 1.0f) * 0.5f;
        f32 y_ratio = (std::sin(phase * 2) + 1.0f) * 0.5f;
        
        f32 x = bounds_.x + style_.padding_left + x_ratio * (bounds_.width - style_.padding_left - style_.padding_right);
        f32 y = bounds_.y + style_.padding_top + y_ratio * (bounds_.height - style_.padding_top - style_.padding_bottom);
        
        children_[i]->set_position(Point{static_cast<i32>(x), static_cast<i32>(y)});
    }
}

void Panel::layout_consciousness() {
    // Consciousness-based layout using awareness principles
    Point awareness_center = {
        bounds_.x + static_cast<i32>(bounds_.width / 2),
        bounds_.y + static_cast<i32>(bounds_.height / 2)
    };
    
    for (usize i = 0; i < children_.size(); ++i) {
        if (!children_[i]->is_visible()) continue;
        
        f32 awareness_level = 1.0f / (1.0f + static_cast<f32>(i) * 0.1f);
        f32 consciousness_radius = 100.0f * awareness_level;
        
        f32 angle = static_cast<f32>(i) * 2.0f * 3.14159f / static_cast<f32>(children_.size());
        
        i32 x = awareness_center.x + static_cast<i32>(consciousness_radius * std::cos(angle));
        i32 y = awareness_center.y + static_cast<i32>(consciousness_radius * std::sin(angle));
        
        children_[i]->set_position(Point{x, y});
    }
}

void Panel::layout_ultimate() {
    // Ultimate perfect layout combining all principles
    layout_consciousness();
    
    // Apply quantum corrections
    for (usize i = 0; i < children_.size(); ++i) {
        if (!children_[i]->is_visible()) continue;
        
        Point pos = children_[i]->get_position();
        
        // Golden ratio adjustment
        f32 golden_ratio = 1.618033988749f;
        f32 adjustment = std::sin(static_cast<f32>(i) / golden_ratio) * 10.0f;
        
        pos.x += static_cast<i32>(adjustment);
        pos.y += static_cast<i32>(adjustment * 0.618f);
        
        children_[i]->set_position(pos);
    }
}

// Bar implementation
Bar::Bar()
    : Panel()
    , dock_side_(Alignment::TopCenter)
    , auto_hide_(false)
    , always_on_top_(true)
    , hidden_(false)
    , hide_timer_(0.0f) {
    
    type_ = ComponentType::Bar;
    set_orientation(Orientation::Horizontal);
}

Bar::~Bar() = default;

void Bar::layout() {
    Panel::layout();
}

void Bar::render(RendererPtr renderer) {
    Panel::render(renderer);
}

// Button implementation
Button::Button()
    : Component(ComponentType::Button)
    , text_("Button")
    , icon_("")
    , checkable_(false)
    , checked_(false) {
    
    bounds_.width = 100;
    bounds_.height = 30;
}

Button::~Button() = default;

void Button::set_text(const String& text) {
    if (text_ != text) {
        text_ = text;
        invalidate_render();
    }
}

void Button::set_checked(bool checked) {
    if (checkable_ && checked_ != checked) {
        checked_ = checked;
        toggled.emit(checked);
        invalidate_render();
    }
}

void Button::render(RendererPtr renderer) {
    // Render button background
    render_button_background(renderer);
    
    // Render base component
    Component::render(renderer);
    
    // Render button content
    render_button_text(renderer);
    render_button_icon(renderer);
    
    if (checkable_) {
        render_check_indicator(renderer);
    }
}

bool Button::handle_mouse_down(const Point& position, u32 button) {
    bool handled = Component::handle_mouse_down(position, button);
    
    if (handled && button == 1) {
        if (checkable_) {
            set_checked(!checked_);
        }
        pressed.emit();
    }
    
    return handled;
}

bool Button::handle_mouse_up(const Point& position, u32 button) {
    return Component::handle_mouse_up(position, button);
}

void Button::render_button_background(RendererPtr renderer) {
    Color bg_color = style_.background_color;
    
    if (pressed_) {
        bg_color = style_.pressed_color;
    } else if (hovered_) {
        bg_color = style_.hover_color;
    }
    
    if (checkable_ && checked_) {
        bg_color = style_.selection_color;
    }
    
    if (style_.corner_radius > 0.0f) {
        renderer->fill_rounded_rect(bounds_, style_.corner_radius, bg_color);
    } else {
        renderer->fill_rect(bounds_, bg_color);
    }
}

void Button::render_button_text(RendererPtr renderer) {
    if (!text_.empty()) {
        Rect text_rect = bounds_;
        text_rect.x += static_cast<i32>(style_.padding_left);
        text_rect.y += static_cast<i32>(style_.padding_top);
        text_rect.width -= static_cast<u32>(style_.padding_left + style_.padding_right);
        text_rect.height -= static_cast<u32>(style_.padding_top + style_.padding_bottom);
        
        renderer->draw_text(text_, text_rect, style_.foreground_color, 
                          style_.font_family, style_.font_size, style_.text_alignment);
    }
}

void Button::render_button_icon(RendererPtr renderer) {
    if (!icon_.empty()) {
        // Icon rendering would be implemented here
    }
}

void Button::render_check_indicator(RendererPtr renderer) {
    if (checkable_ && checked_) {
        // Check mark rendering would be implemented here
        Rect check_rect = {
            bounds_.x + static_cast<i32>(bounds_.width - 20),
            bounds_.y + static_cast<i32>((bounds_.height - 16) / 2),
            16,
            16
        };
        
        renderer->fill_rect(check_rect, style_.selection_color);
    }
}

// Label implementation
Label::Label()
    : Component(ComponentType::Label)
    , text_("Label")
    , text_alignment_(Alignment::MiddleLeft)
    , word_wrap_(false) {
    
    style_.focusable = false;
}

Label::~Label() = default;

void Label::set_text(const String& text) {
    if (text_ != text) {
        text_ = text;
        invalidate_render();
    }
}

void Label::render(RendererPtr renderer) {
    Component::render(renderer);
    render_text(renderer);
}

void Label::render_text(RendererPtr renderer) {
    if (!text_.empty()) {
        Rect text_rect = bounds_;
        text_rect.x += static_cast<i32>(style_.padding_left);
        text_rect.y += static_cast<i32>(style_.padding_top);
        text_rect.width -= static_cast<u32>(style_.padding_left + style_.padding_right);
        text_rect.height -= static_cast<u32>(style_.padding_top + style_.padding_bottom);
        
        if (word_wrap_) {
            // Word wrapping would be implemented here
        }
        
        renderer->draw_text(text_, text_rect, style_.foreground_color,
                          style_.font_family, style_.font_size, text_alignment_);
    }
}

// UIManager implementation
UIManager::UIManager()
    : root_component_(nullptr)
    , focused_component_(nullptr)
    , hovered_component_(nullptr)
    , captured_component_(nullptr)
    , current_theme_("default")
    , initialized_(false)
    , layout_dirty_(false)
    , animation_speed_(1.0f)
    , liquid_glass_enabled_(true)
    , holographic_enabled_(false)
    , quantum_enabled_(false)
    , consciousness_enabled_(false)
    , transcendent_enabled_(false)
    , ultimate_effects_enabled_(false) {
    SU1_PROFILE_FUNCTION();
}

UIManager::~UIManager() {
    SU1_PROFILE_FUNCTION();
    shutdown();
}

bool UIManager::initialize() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (initialized_) {
        return true;
    }
    
    SU1_LOG_INFO("Initializing UI manager");
    
    // Register default component factories
    register_default_factories();
    
    // Setup default theme
    setup_default_theme();
    
    // Create root component
    root_component_ = MakeShared<Panel>();
    root_component_->set_name("Root");
    
    initialized_ = true;
    SU1_LOG_INFO("UI manager initialized");
    
    return true;
}

void UIManager::shutdown() {
    SU1_PROFILE_FUNCTION();
    LOCK_GUARD(mutex_);
    
    if (!initialized_) {
        return;
    }
    
    SU1_LOG_INFO("Shutting down UI manager");
    
    root_component_ = nullptr;
    focused_component_ = nullptr;
    hovered_component_ = nullptr;
    captured_component_ = nullptr;
    
    component_factories_.clear();
    themes_.clear();
    
    initialized_ = false;
    SU1_LOG_INFO("UI manager shut down");
}

void UIManager::update(f64 delta_time) {
    SU1_PROFILE_FUNCTION();
    
    if (!initialized_ || !root_component_) {
        return;
    }
    
    update_component_recursive(root_component_, delta_time);
    
    if (layout_dirty_) {
        force_layout();
    }
}

void UIManager::render(RendererPtr renderer) {
    SU1_PROFILE_FUNCTION();
    
    if (!initialized_ || !root_component_ || !renderer) {
        return;
    }
    
    render_component_recursive(root_component_, renderer);
}

void UIManager::layout() {
    if (root_component_) {
        layout_component_recursive(root_component_);
        layout_dirty_ = false;
    }
}

bool UIManager::handle_mouse_move(const Point& position) {
    if (!root_component_) {
        return false;
    }
    
    ComponentPtr component = find_component_at(position);
    set_hovered_component(component);
    
    if (captured_component_) {
        return captured_component_->handle_mouse_move(position);
    }
    
    return root_component_->handle_mouse_move(position);
}

bool UIManager::handle_mouse_down(const Point& position, u32 button) {
    if (!root_component_) {
        return false;
    }
    
    ComponentPtr component = find_component_at(position);
    if (component && component->get_style().focusable) {
        set_focused_component(component);
    }
    
    return root_component_->handle_mouse_down(position, button);
}

void UIManager::set_focused_component(ComponentPtr component) {
    if (focused_component_ != component) {
        if (focused_component_) {
            focused_component_->set_focused(false);
        }
        
        focused_component_ = component;
        
        if (focused_component_) {
            focused_component_->set_focused(true);
        }
    }
}

void UIManager::set_hovered_component(ComponentPtr component) {
    hovered_component_ = component;
}

ComponentPtr UIManager::find_component_at(const Point& position) {
    if (!root_component_) {
        return nullptr;
    }
    
    return find_component_at_recursive(root_component_, position);
}

ComponentPtr UIManager::create_component(ComponentType type) {
    auto it = component_factories_.find(type);
    if (it != component_factories_.end()) {
        return it->second();
    }
    
    return nullptr;
}

void UIManager::register_default_factories() {
    component_factories_[ComponentType::Panel] = []() { return MakeShared<Panel>(); };
    component_factories_[ComponentType::Bar] = []() { return MakeShared<Bar>(); };
    component_factories_[ComponentType::Button] = []() { return MakeShared<Button>(); };
    component_factories_[ComponentType::Label] = []() { return MakeShared<Label>(); };
}

void UIManager::setup_default_theme() {
    global_style_ = ComponentStyle();
    themes_["default"] = global_style_;
    
    // Liquid glass theme
    ComponentStyle liquid_glass_style = global_style_;
    liquid_glass_style.apply_liquid_glass_effect();
    themes_["liquid_glass"] = liquid_glass_style;
    
    // Ultimate theme
    ComponentStyle ultimate_style = global_style_;
    ultimate_style.apply_ultimate_effect();
    themes_["ultimate"] = ultimate_style;
}

void UIManager::update_component_recursive(ComponentPtr component, f64 delta_time) {
    if (component) {
        component->update(delta_time);
    }
}

void UIManager::render_component_recursive(ComponentPtr component, RendererPtr renderer) {
    if (component) {
        component->render(renderer);
    }
}

void UIManager::layout_component_recursive(ComponentPtr component) {
    if (component) {
        component->layout();
    }
}

ComponentPtr UIManager::find_component_at_recursive(ComponentPtr component, const Point& position) {
    if (!component || !component->is_visible()) {
        return nullptr;
    }
    
    const Rect& bounds = component->get_bounds();
    if (position.x >= bounds.x && position.x < bounds.x + static_cast<i32>(bounds.width) &&
        position.y >= bounds.y && position.y < bounds.y + static_cast<i32>(bounds.height)) {
        
        // Check children first (reverse order for proper z-order)
        const auto& children = component->get_children();
        for (auto it = children.rbegin(); it != children.rend(); ++it) {
            ComponentPtr found = find_component_at_recursive(*it, position);
            if (found) {
                return found;
            }
        }
        
        return component;
    }
    
    return nullptr;
}

void UIManager::force_layout() {
    layout();
}

void UIManager::harmonize_ui_consciousness() {
    // Harmonize the consciousness field for optimal UI experience
    if (root_component_ && consciousness_enabled_) {
        root_component_->apply_consciousness_effect();
    }
}

void UIManager::align_transcendent_interface() {
    // Align the interface with transcendent principles
    if (root_component_ && transcendent_enabled_) {
        root_component_->apply_transcendent_effect();
    }
}

void UIManager::perfect_ultimate_user_experience() {
    // Achieve the ultimate perfect user experience
    if (root_component_ && ultimate_effects_enabled_) {
        root_component_->apply_ultimate_effect();
        harmonize_ui_consciousness();
        align_transcendent_interface();
    }
}

} // namespace su1
