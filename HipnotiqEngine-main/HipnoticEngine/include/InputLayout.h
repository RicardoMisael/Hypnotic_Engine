#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

class 
InputLayout {
public:
  InputLayout() = default;
  ~InputLayout() = default;

  /**
   * @brief Configura el diseño de entrada utilizando el dispositivo, 
   * la descripción del diseño de entrada y los datos del sombreador de vértices.
   * @param device Referencia al dispositivo utilizado para crear el diseño de entrada.
   * @param Layout Vector de estructuras D3D11_INPUT_ELEMENT_DESC que describen el diseño de entrada.
   * @param VertexShaderData Puntero a los datos del sombreador de vértices.
   * @return Resultado de la operación (HRESULT).
   */
  HRESULT
  init(Device& device,
      std::vector<D3D11_INPUT_ELEMENT_DESC>& Layout,
      ID3DBlob* VertexShaderData);

  /**
   * @brief Actualiza el estado del diseño de entrada para reflejar modificaciones en tiempo de ejecución.
   */
  void
  update();

  /**
   * @brief Vincula el diseño de entrada al pipeline de renderizado.
   * @param deviceContext Referencia al contexto del dispositivo.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Libera recrusos
   */
  void
  destroy();

public:
  ID3D11InputLayout* m_inputLayout = nullptr;
};