#pragma once
#include "Prerequisites.h"
#include "ECS/Entity.h"
#include "Buffer.h"
#include "Texture.h"
#include "SamplerState.h"
#include "Transform.h"
using namespace std;

class Device;
class Component;

class Actor : Entity {
public:
  /*
  * @brief Destructor
  */
  Actor() = default;

  /*
  * @brief Constructor con device
  */
  Actor(Device& device);

  /*
  * @brief Destructor Virtual 
  */
  virtual ~Actor() = default;

  /*
  * @brief Se encarga de actualizar la lógica del Actor
  * @param deltaTime Tiempo en segundos desde la última actualización.
  * @param deviceContext Contexto de renderizado con el que se aplican los cambios.
  */
  void
  update(float deltaTime, DeviceContext& deviceContext) override;

  /*
  * @brief Se encarga de aplicar shaders, enviar buffers y texturas al pipeline gráfico.
 * @param deviceContext Contexto del dispositivo donde se realiza el render.
  */
  void
  render(DeviceContext& deviceContext) override;

  /**
   * @brief Libera todos los buffers y texturas
   */
  void
  destroy();

  /**
   * @brief Este método genera los buffers de vértices e índices
   * @param  device Dispositivo usado para crear los recursos.
   * @param meshes Conjunto de componentes de malla.
   */
  void 
  setMesh(Device& device, std::vector<MeshComponent> meshes);

  /**
  * @brief Las texturas serán utilizadas en el proceso de renderizado.
  * @param textures Vector de objetos Texture.
  */
  void 
  setTextures(vector<Texture> textures) {
    m_textures = textures;
  }

  /**
   * @brief Obtiene el nombre del Actor
   */
  string
  getName() {
    return m_name;
  }

  /*
  * @brief Busca dentro de la lista del Actor y devuelve el que coincida con el tipo solicitado.
  */
  template<typename T>
  EngineUtilities::TSharedPointer<T>
  getComponent();

private:
  vector<MeshComponent> m_meshes;
  vector<Texture> m_textures;
  vector<Buffer> m_vertexBuffers;
  vector<Buffer> m_indexBuffers;
  vector<Buffer> m_buffers;

  Buffer m_modelBuffer;
  SamplerState m_sampler;
  CBChangesEveryFrame m_changeEveryFrame;
  string m_name = "Actor";
};

/*
  * The purpose of this function is to search for and return a specific component of an actor using the specific component type as template arguments.
  * actor using the type of the specific component as template arguments.
  * If the component is not found, the function returns nullptr.
  */
template<typename T>
inline EngineUtilities::TSharedPointer<T>
Actor::getComponent() {
  for (auto& component : m_components) {
    EngineUtilities::TSharedPointer<T> specificComponent = component.template dynamic_pointer_cast<T>();
    if (specificComponent) {
      return specificComponent;
    }
  }
  // Returns an empty TSharedPointer if the component is not found.
  return EngineUtilities::TSharedPointer<T>();
}