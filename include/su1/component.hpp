#pragma once

#include "su1/core.hpp"

namespace su1 {

class Component {
public:
    Component();
    virtual ~Component();

    virtual void update(f32 delta_time);
    virtual void render();

    // Basic properties
    String get_id() const { return id_; }
    void set_id(const String& id) { id_ = id; }

    bool is_visible() const { return visible_; }
    void set_visible(bool visible) { visible_ = visible; }

    bool is_enabled() const { return enabled_; }
    void set_enabled(bool enabled) { enabled_ = enabled; }

protected:
    String id_;
    bool visible_;
    bool enabled_;
};

using ComponentPtr = std::shared_ptr<Component>;

} // namespace su1
