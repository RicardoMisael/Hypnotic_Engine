#pragma once
#include "Prerequisites.h"

class Window;
class DeviceContext;

class
Viewport {
public:
  Viewport() = default;
  ~Viewport() = default;

  /**
   * @brief Inicializa el viewport utilizando una instancia de Window.
   * @param window Referencia al objeto Window.
   * @return Resultado de la operación (HRESULT).
   */
  HRESULT
  init(const Window& window);

  /**
   * @brief Inicializa el viewport con dimensiones específicas.
   * @param width Ancho del viewport.
   * @param height Alto del viewport.
   * @return Resultado de la operación (HRESULT).
   */
  HRESULT
  init(unsigned int width, unsigned int height);

  /**
   * @brief Actualiza la configuración del viewport cuando se requieren cambios en sus dimensiones.
   */
  void
  update();

  /**
   * @brief Renderiza el viewport utilizando el contexto de dispositivo proporcionado.
   * @param deviceContext Referencia al DeviceContext utilizado para la renderización.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Libera recursos
   */
  void
  destroy();

private:
  D3D11_VIEWPORT m_viewport;
};