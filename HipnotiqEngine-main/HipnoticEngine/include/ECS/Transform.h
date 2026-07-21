#pragma once
#include "Prerequisites.h"
#include "ECS/Component.h"
#include "utilities/Vectors/Vector3.h"

class
Transform : public Component {
public:
  /**
   * @brief Inicializa los vectores de posición, rotación y escala a cero, y el matrix vacío.
   */
  Transform() : position(),
                rotation(),
                scale(),
                matrix(),
                Component(ComponentType::TRANSFORM) {}

  /**
   * @brief Inicializa el estado del Transform.
   */
  void
  init();

  /**
   * @brief Lógica de actualización del Transform.
   * @param deltaTime Tiempo (en segundos) desde el último frame.
   */
  void
  update(float deltaTime) override;

  /**
   * @brief En esta clase no realiza ninguna operación visual.
   * @param deviceContext Contexto de renderizado.
   */
  void
  render(DeviceContext& deviceContext) override {};

  /**
   * @brief Libera recursos o referencias que pueda tener el Transform.
   */
  void
  destroy() {}

  /**
   * @brief Acceder directamente a la posición
   * @return Vector de posición.
   */
  const EngineUtilities::Vector3& 
  getPosition() const { return position; }
  
  /**
   * @brief Define una nueva posición para el Transform
   */
  void
  setPosition(const EngineUtilities::Vector3& newPos) { position = newPos; }

  /**
   * @brief Obtiene la rotación actual del Transform.
   * @return Vector de rotación.
   */
  const EngineUtilities::Vector3& 
  getRotation() const { return rotation; }

  /**
   * @brief Establece una nueva rotación para el Transform.
   */
  void
  setRotation(const EngineUtilities::Vector3& newRot) { rotation = newRot; }

  /**
   * @brief Devuelve la escala actual del objeto.
   * @return Vector que representa la escala en cada eje.
   */
  const EngineUtilities::Vector3& 
  getScale() const { return scale; }

  /**
   * @brief Establece una nueva escala en el Transform.
   */
  void
  setScale(const EngineUtilities::Vector3& newScale) { scale = newScale; }

  /*
  * @brief Configura los tres atributos principales del Transform de una vez.
  * @param newPos Nueva posición.
  * @param newRot Nueva rotación.
  * @param newSca Nueva escala.
  */
  void
  setTransform(const EngineUtilities::Vector3& newPos, 
               const EngineUtilities::Vector3& newRot,
               const EngineUtilities::Vector3& newSca);

private:
  EngineUtilities::Vector3 position;
  EngineUtilities::Vector3 rotation;
  EngineUtilities::Vector3 scale;

public:
  XMMATRIX matrix;
};