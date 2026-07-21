#include "SamplerState.h"
#include "Device.h"
#include "DeviceContext.h"

HRESULT
SamplerState::init(Device& device) {
  if (!device.m_device) {
    ERROR("SamplerState", "init", "Device is nullptr in SamplerState init method");
  }

  HRESULT hr = S_OK;

  D3D11_SAMPLER_DESC sampDesc;
  ZeroMemory(&sampDesc, sizeof(sampDesc));
  sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
  sampDesc.MinLOD = 0;
  sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
  hr = device.CreateSamplerState(&sampDesc, &m_samplerState);
  if (FAILED(hr)) {
    ERROR("SamplerState", "init", "Failed to create Sampler State.");
    return hr;
  }

  return S_OK;
}

void
SamplerState::update() {
}

void
SamplerState::render(DeviceContext& deviceContext,
                     unsigned int StartSlot,
                     unsigned int numSamplers) {
  if (!deviceContext.m_deviceContext) {
    ERROR("SamplerState", "render", "DeviceContext is nullptr in SamplerState render method");
  }

  deviceContext.PSSetSamplers(StartSlot, numSamplers, &m_samplerState);
}

void
SamplerState::destroy() {
  SAFE_RELEASE(m_samplerState);
}
