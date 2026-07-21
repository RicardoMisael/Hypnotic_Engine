#pragma once
#include "Prerequisites.h"

// Forward Declarations
class Device;
class DeviceContext;
class Texture;

class
DepthStencilView {
public:
  /**
   * @brief Default constructor and destructor
   */
  DepthStencilView() = default;
  ~DepthStencilView() = default;

  /**
   * @brief Initializes the depth stencil view.
   * @param device Reference to the Device class.
   * @param depthStencil Reference to the depth stencil used as a texture.
   */
  HRESULT
  init(Device& device, Texture& depthStencil, DXGI_FORMAT format);

  /**
   * @brief Updates the depth stencil view.
   */
  void
  update();

  /**
   * @brief Renders using the depth stencil view.
   * @param deviceContext Reference to the device context.
   */
  void
  render(DeviceContext& deviceContext);

  /**
   * @brief Destroys the depth stencil view and releases resources.
   */
  void
  destroy();

public:
  ID3D11DepthStencilView* m_depthStencilView = nullptr;
};
