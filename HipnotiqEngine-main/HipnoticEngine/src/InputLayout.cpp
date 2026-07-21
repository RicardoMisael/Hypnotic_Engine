#include "InputLayout.h"
#include "Device.h"
#include "DeviceContext.h"

HRESULT 
InputLayout::init(Device& device, 
                  std::vector<D3D11_INPUT_ELEMENT_DESC>& Layout, 
                  ID3DBlob* VertexShaderData) {
  if (Layout.empty()) {
    ERROR("InputLayout", "init", "Layout vector is empty");
    return E_INVALIDARG;
  }
  if (!VertexShaderData) {
    ERROR("InputLayout", "init", "VertexShaderData is nullptr");
    return E_POINTER;
  }

  HRESULT hr = S_OK;

  hr = device.CreateInputLayout(Layout.data(),
    static_cast<unsigned int>(Layout.size()),
    VertexShaderData->GetBufferPointer(),
    VertexShaderData->GetBufferSize(),
    &m_inputLayout);

  if (FAILED(hr)) {
    ERROR("InputLayout", "init", "Failed to create InputLayout");
    return hr;
  }

  return S_OK;
}

void 
InputLayout::update() {
}

void 
InputLayout::render(DeviceContext& deviceContext) {
  if (!deviceContext.m_deviceContext) {
    ERROR("InputLayout", "render", "DeviceContext is nullptr in InputLayout render method");
    return;
  }

  deviceContext.IASetInputLayout(m_inputLayout);
}

void 
InputLayout::destroy() {
  SAFE_RELEASE(m_inputLayout);
}
