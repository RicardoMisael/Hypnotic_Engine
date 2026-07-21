#include "DepthStencilView.h"
#include "Device.h"
#include "DeviceContext.h"
#include "Texture.h"

HRESULT
DepthStencilView::init(Device& device, Texture& depthStencil, DXGI_FORMAT format) {
  HRESULT hr = S_OK;

  if (device.m_device == nullptr) {
    ERROR("RenderTargetView", "init", "Device is nullptr in DepthStencilView init method");
  }

  if (!depthStencil.m_texture) {
    ERROR("RenderTargetView", "init", "depthStencil texture is nullptr in DepthStencilView init method");
  }

  D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
  memset(&descDSV, 0, sizeof(descDSV));
  descDSV.Format = format;
  descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

  hr = device.CreateDepthStencilView(depthStencil.m_texture, &descDSV, &m_depthStencilView);

  if (FAILED(hr)) {
    ERROR("DepthStencilView", "CreateDepthStencilView", "Failed to create Depth Stencil View.");
    return hr;
  }

  return S_OK;
}

void
DepthStencilView::update() {
}

void
DepthStencilView::render( DeviceContext& deviceContext) {
  if (!deviceContext.m_deviceContext) {
    ERROR("DepthStencilView", "render", "DeviceContext is nullptr in DepthStencilView render method");
  }

  deviceContext.ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void
DepthStencilView::destroy() {
  SAFE_RELEASE(m_depthStencilView);
}
