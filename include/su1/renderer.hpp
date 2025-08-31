#pragma once

#include "su1/core.hpp"

namespace su1 {

class Renderer {
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual bool initialize() { return true; }
    virtual void shutdown() {}

    virtual void begin_frame() {}
    virtual void end_frame() {}

    virtual void clear(Color color) {}
    virtual void present() {}
};

using RendererPtr = std::shared_ptr<Renderer>;

} // namespace su1
