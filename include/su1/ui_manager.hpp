#pragma once

#include "su1/core.hpp"
#include "su1/manager_base.hpp"
#include "su1/component.hpp"

namespace su1 {

class Component;
using ComponentPtr = std::shared_ptr<Component>;

class UIManager : public Manager {
public:
    UIManager();
    ~UIManager() override;

    bool initialize() override;
    void shutdown() override;

    ComponentPtr create_component(const String& type);
    ComponentPtr get_root_component() const;

    void set_root_component(ComponentPtr root);
    std::vector<ComponentPtr> get_components() const;

private:
    ComponentPtr root_component_;
    std::vector<ComponentPtr> components_;
};

} // namespace su1
