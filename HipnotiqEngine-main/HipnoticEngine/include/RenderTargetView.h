#pragma once
#include "Prerequisites.h"

// Forward Declarations
class Device;
class DeviceContext;
class Texture;
class DepthStencilView;

class 
RenderTargetView {
public:
  /**
   * @brief Default constructor and destructor.
   */
  RenderTargetView() = default;
  ~RenderTargetView() = default;

  /**
   * @brief Initializes the render target view using a device and a back buffer texture.
   */
  HRESULT
  init(Device& device, Texture& backBuffer, DXGI_FORMAT format);

  /**
   * @brief Updates the render target view state.
   */
  void
  update();

  /**
   * @brief Renders using the render target view.
   */
  void
  render(DeviceContext& deviceContext, 
        DepthStencilView& depthStencilView, 
        unsigned int numOfViews, 
        const float clearColor[4]);

  /**
   * @brief Releases the resources associated with the render target view.
   */
  void
  destroy();

public:
  ID3D11RenderTargetView* m_renderTargetView = nullptr;
};