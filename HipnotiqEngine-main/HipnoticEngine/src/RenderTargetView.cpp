#include "RenderTargetView.h"
#include "Device.h"
#include "DeviceContext.h"
#include "Texture.h"
#include "DepthStencilView.h"

HRESULT
RenderTargetView::init(Device& device, Texture& backBuffer, DXGI_FORMAT format) {
  HRESULT hr = S_OK;

  if (device.m_device == nullptr) {
    ERROR("RenderTargetView", "init", "Device is nullptr in RenderTargetView init method");
  }

  if (!backBuffer.m_texture) {
    ERROR("RenderTargetView", "init", "backBuffer texture is nullptr in RenderTargetView init method");
  }

  D3D11_RENDER_TARGET_VIEW_DESC desc;
  memset(&desc, 0, sizeof(desc));

  desc.Format = format;
  desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DMS;
  // desc.Texture2D.MipSlice = 0;

	hr = device.CreateRenderTargetView(backBuffer.m_texture, &desc, &m_renderTargetView);

	if (FAILED(hr)) {
		ERROR("RenderTargetView", "CreateRenderTargetView", "Failed to create Render Target View.");
    return hr;
	}

  return S_OK;
}

void 
RenderTargetView::update() {
}

void 
RenderTargetView::render(DeviceContext& deviceContext, 
                        DepthStencilView& depthStencilView, 
                        unsigned int numOfViews, 
                        const float clearColor[4]) {
  if (!deviceContext.m_deviceContext) {
    ERROR("RenderTargetView", "render", "DeviceContext is nullptr in RenderTargetView render method");
  }

  if (!depthStencilView.m_depthStencilView) {
    ERROR("RenderTargetView", "render", "DepthStencilView is nullptr in RenderTargetView render method");
  }

  deviceContext.ClearRenderTargetView(m_renderTargetView, clearColor);
  deviceContext.OMSetRenderTargets(numOfViews, &m_renderTargetView, depthStencilView.m_depthStencilView);
}

void 
RenderTargetView::destroy() {
  SAFE_RELEASE(m_renderTargetView);
}
