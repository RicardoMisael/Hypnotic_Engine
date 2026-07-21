#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

class
SamplerState {
public:
  SamplerState() = default;
  ~SamplerState() = default;

  /**
   * @brief Inicializa el SamplerState, definiendo cómo se muestrean las texturas.
 * @param device Referencia al dispositivo gráfico.
 * @return HRESULT indicando éxito o error en la inicialización.
   */
  HRESULT
  init(Device& device);

  /**
   * @brief Actualiza la configuración del SamplerState.
   */
  void
  update();

  /**
   * @brief Vincula el SamplerState al contexto del dispositivo para el muestreo de texturas.
   * @param deviceContext Contexto del dispositivo.
   * @param StartSlot Primer slot donde se vinculará el SamplerState.
   * @param numSamplers Número de estados de muestreo a establecer.
   */
  void
  render(DeviceContext& deviceContext, 
           unsigned int StartSlot, 
           unsigned int numSamplers);

  /**
   * @brief Libera los recursos
   */
  void
  destroy();

private:
  ID3D11SamplerState* m_samplerState = nullptr;
};