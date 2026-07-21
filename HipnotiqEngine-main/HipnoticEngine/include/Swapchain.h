#pragma once
#include "Prerequisites.h"

// Forward declarations
class Device;
class DeviceContext;
class Window;
class Texture;

class
SwapChain {
public:
  /**
   * @brief Default constructor  and destructor for the SwapChain class.
   */
  SwapChain() = default;
  ~SwapChain() = default;

  /**
   * @brief Initializes the swap chain with the given device, context, back buffer, and window.
   */
  HRESULT
  init(Device& device,
      DeviceContext& deviceContext,
      Texture& backBuffer,
      Window window);

  /**
   * @brief Updates the swap chain state, if necessary
   */
  void
  update();

  /**
   * @brief Renders the current frame using the swap chain
   */
  void 
  render();

  /**
   * @brief Releases resources associated with the swap chain
   */
  void
  destroy();

  /**
   * @brief Presents the rendered frame to the screen
   */
  void
  present();

public:
  IDXGISwapChain* g_pSwapChain = nullptr; // Pointer to the directX swap chain
  D3D_DRIVER_TYPE m_driverType = D3D_DRIVER_TYPE_NULL; // Type of directX driver

private:
  D3D_FEATURE_LEVEL m_featureLevel = D3D_FEATURE_LEVEL_11_0;

  /*
  * Para evitar un efecto de "serrucho" (aliasing) en los bordes de los modelos
  * en DirectX 11 con C++, se utiliza una técnica llamada anti-aliasing.
  * 
  * Miltisample Anti-Aliasing(MSAA)
  * MSAA es una técnica que suaviza los bordes de los objetos mostrando varios puntos
  * por pixel. Para usar esta técnica se usa lo siguiente:
  */
  unsigned int m_sampleCount; // Number of MSAA samples (4x MSAA)
  unsigned int m_qualityLevels;

  // Punteros a las interfaces DXGI
  IDXGIDevice* m_dxgiDevice = nullptr; 
  IDXGIAdapter* m_dxgiAdapter = nullptr; 
  IDXGIFactory* m_dxgiFactory = nullptr;
};