#pragma once
class DeviceContext;

/*
* @class Component
* @brief Clase base abstracta para todos los componentes del juego.
* La clase Component define la interfaz básica que todos los componentes deben implementar,
* permitiendo actualizar y renderizar el componente, asi como obtener su tipo.
*/
class
Component {
public:
  Component() = default;

  /**
  * @brief Constructor with component type.
  * @param type Type of the component.
  */
  Component(const ComponentType type) : m_type(type) {}

  virtual
  ~Component() = default;

  /*
  * @brief Pure virtual method to update the component.
  * @param deltaTime Time elapsed since the last update.
  */
  virtual void
  update(float deltaTime) = 0;

  /*
  * @brief Pure virtual method to render the component.
  * @param deviceContext The device context for graphic operations.
  */
  virtual void
  render(DeviceContext& deviceContext) = 0;

  /*
  * @brief Gets the component type.
  * @return The type of the component.
  */
  ComponentType 
  getType()
    const {
    return m_type;
  }

private:

protected:
  ComponentType m_type; //Tipo de componente
};