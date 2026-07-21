#pragma once
#include "Prerequisites.h"
#include "Component.h"

// Forward Declarations
class DeviceContext;

class
Entity {
public:
  /**
   * @brief Virtual destructor.
   */
  virtual
    ~Entity() = default;

  /**
   * @brief Pure virtual method to update the entity.
   * @param deltaTime Time elapsed since the last update.
   * @param deviceContext The device context for graphical operations.
   */
  virtual void
    update(float deltaTime, DeviceContext& deviceContext) = 0;

  /**
   * @brief Pure virtual method to render the entity.
   * @param deviceContext The device context for graphic operations.
   */
  virtual void
    render(DeviceContext& deviceContext) = 0;

  /**
   * @brief Adds a component to the entity.
   * @tparam T The component type, must derive from Component.
   * @param component Shared pointer to the component to add.
   */
  template <typename T>
  void addComponent(EngineUtilities::TSharedPointer<T> component) {
    static_assert(std::is_base_of<Component, T>::value, "T must be derived from Component");
    m_components.push_back(component.template dynamic_pointer_cast<Component>());
  }

  /**
   * @brief Retrieves a component from the entity.
   * @tparam T The type of component to retrieve.
   * @return Shared pointer to the component, or nullptr if not found.
   */
  template<typename T>
  EngineUtilities::TSharedPointer<T>
    getComponent() {
    for (auto& component : m_components) {
      EngineUtilities::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
      if (specificComponent) {
        return specificComponent;
      }
    }
    return EngineUtilities::TSharedPointer<T>();
  }

protected:
  bool m_isActive;
  int m_id;

  std::vector<EngineUtilities::TSharedPointer<Component>> m_components;
};