#pragma once
#include "Prerequisites.h"

class 
Device {
public:
  /**
   * @brief Default constructor and destructor
   */
  Device() = default;
  ~Device() = default;

  /**
   * @brief Initializes the device setting up the resources.
   */
  void
  init();

  /**
   * @brief Updates the device state.
   */
  void
  update();

  /**
   * @brief Renders the current frame.
   */
  void
  render();

  /**
   * @brief Cleans up and releases allocated resources.
   */
  void
  destroy();

  /**
   * @brief Creates a render target view from a resource.
   */
  HRESULT
  CreateRenderTargetView(ID3D11Resource* pResource,
                        const D3D11_RENDER_TARGET_VIEW_DESC* pDesc,
                        ID3D11RenderTargetView** ppRTView);

  /**
   * @brief Creates a 2D texture.
   */
  HRESULT
  CreateTexture2D(const D3D11_TEXTURE2D_DESC* pDesc,
                  const D3D11_SUBRESOURCE_DATA* pInitialData,
                  ID3D11Texture2D** ppTexture2D);

  /**
   * @brief Creates a depth stencil view from a resource.
   */
  HRESULT
  CreateDepthStencilView(ID3D11Resource* pResource,
                        const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc,
                        ID3D11DepthStencilView** ppDepthStencilView);

  /**
   * @brief Creates a vertex shader.
   */
  HRESULT
  CreateVertexShader(const void* pShaderBytecode,
                    unsigned int BytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage,
                    ID3D11VertexShader** ppVertexShader);

  /**
   * @brief Creates an input layout for vertex data.
   */
  HRESULT
  CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* pInputElementDescs,
                    unsigned int NumElements,
                    const void* pShaderBytecodeWithInputSignature,
                    unsigned int BytecodeLength,
                    ID3D11InputLayout** ppInputLayout);

  /**
   * @brief Creates a pixel shader.
   */
  HRESULT
  CreatePixelShader(const void* pShaderBytecode,
                    unsigned int BytecodeLength,
                    ID3D11ClassLinkage* pClassLinkage,
                    ID3D11PixelShader** ppPixelShader);

  /**
   * @brief Creates a buffer resource.
   */
  HRESULT
  CreateBuffer(const D3D11_BUFFER_DESC* pDesc,
              const D3D11_SUBRESOURCE_DATA* pInitialData,
              ID3D11Buffer** ppBuffer);

  /**
   * @brief Creates a sampler state object.
   */
  HRESULT
  CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc,
                    ID3D11SamplerState** ppSamplerState);


  ID3D11Device* m_device = nullptr;

private:
};