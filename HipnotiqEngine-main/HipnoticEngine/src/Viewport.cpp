#include "Viewport.h"
#include "Window.h"
#include "DeviceContext.h"

HRESULT 
Viewport::init(const Window& window) {
  HRESULT hr = S_OK;

  if (window.m_hWnd == nullptr) {
    ERROR("Viewport", "init", "Window is nullptr in Viewport init method");
  }

  m_viewport.Width = static_cast<float>(window.m_width);
  m_viewport.Height = static_cast<float>(window.m_height);
  m_viewport.MinDepth = 0.0f;
  m_viewport.MaxDepth = 1.0f;
  m_viewport.TopLeftX = 0;
  m_viewport.TopLeftY = 0;

  return E_NOTIMPL;
}

HRESULT 
Viewport::init(unsigned int width, unsigned int height) {
  if (width == 0) {
    ERROR("Viewport", "init", "Width is zero in init method");
  }

  if (height == 0) {
    ERROR("Viewport", "init", "Height is zero in init method");
  }

  m_viewport.Width = static_cast<float>(width);
  m_viewport.Height = static_cast<float>(height);
  m_viewport.MinDepth = 0.0f;
  m_viewport.MaxDepth = 1.0f;
  m_viewport.TopLeftX = 0;
  m_viewport.TopLeftY = 0;
  
  return E_NOTIMPL;
}

void 
Viewport::update() {
}

void 
Viewport::render(DeviceContext& deviceContext) {
  if (!deviceContext.m_deviceContext) {
    ERROR("Viewport", "render", "DeviceContext is nullptr in Viewport render method");
    return;
  }

  deviceContext.RSSetViewports(1, &m_viewport);
}

void 
Viewport::destroy() {
  //SAFE_RELEASE(m_viewport); // No es una referencia así que no
}
