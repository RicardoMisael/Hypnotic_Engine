#pragma once
#include "Prerequisites.h"

class 
DeviceContext {
public:
  /**
   * @brief Default constructor and destructor
   */
  DeviceContext() = default;
  ~DeviceContext() = default;

  /**
   * @brief Initializes the device context.
   */
  void
  init();

  /**
   * @brief Updates the device context.
   */
  void
  update();

  /**
   * @brief Renders using the device context.
   */
  void
  render();

  /**
   * @brief Releases resources and destroys the device context.
   */
  void
  destroy();

  /**
   * @brief Sets the render targets for the output merger stage.
   */
  void
  OMSetRenderTargets(unsigned int NumViews,
                    ID3D11RenderTargetView* const* ppRenderTargetViews,
                    ID3D11DepthStencilView* pDepthStencilView);

  /**
   * @brief Sets the input layout for the input assembler stage.
   */
  void
  RSSetViewports(unsigned int NumViewports,
                const D3D11_VIEWPORT* pViewports);

  /**
   * @brief Sets the input layout for the input assembler stage.
   */
  void
  IASetInputLayout(ID3D11InputLayout* pInputLayout);

  /**
   * @brief Sets the vertex buffers for the input assembler stage.
   */
  void
  IASetVertexBuffers(unsigned int StartSlot,
                    unsigned int NumBuffers,
                    ID3D11Buffer* const* ppVertexBuffers,
                    const unsigned int* pStrides,
                    const unsigned int* pOffsets);

  /**
   * @brief Sets the index buffer for the input assembler stage.
   */
  void
  IASetIndexBuffer(ID3D11Buffer* pIndexBuffer,
                  DXGI_FORMAT Format,
                  unsigned int Offset);

  /**
   * @brief Sets the primitive topology for the input assembler stage.
   */
  void
  IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY Topology);

  /**
   * @brief Updates a subresource with new data.
   */
  void
  UpdateSubresource(ID3D11Resource* pDstResource,
                    unsigned int DstSubresource,
                    const D3D11_BOX* pDstBox,
                    const void* pSrcData,
                    unsigned int SrcRowPitch,
                    unsigned int SrcDepthPitch);

  /**
   * @brief Clears a render target view.
   */
  void
  ClearRenderTargetView(ID3D11RenderTargetView* pRenderTargetView,
                        const FLOAT ColorRGBA[4]);

  /**
   * @brief Clears a depth stencil view.
   */
  void
  ClearDepthStencilView(ID3D11DepthStencilView* pDepthStencilView,
                        unsigned int ClearFlags,
                        FLOAT Depth,
                        UINT8 Stencil);

  /**
   * @brief Sets the vertex shader.
   */
  void
  VSSetShader(ID3D11VertexShader* pVertexShader,
              ID3D11ClassInstance* const* ppClassInstances,
              unsigned int NumClassInstances);

  /**
   * @brief Sets the constant buffers for the vertex shader stage.
   */
  void 
  VSSetConstantBuffers(unsigned int StartSlot,
                      unsigned int NumBuffers,
                      ID3D11Buffer* const* ppConstantBuffers);

  /**
   * @brief Sets the pixel shader.
   */
  void
  PSSetShader(ID3D11PixelShader* pPixelShader,
              ID3D11ClassInstance* const* ppClassInstances,
              unsigned int NumClassInstances);

  /**
   * @brief Sets the constant buffers for the pixel shader stage.
   */
  void
  PSSetConstantBuffers(unsigned int StartSlot,
                        unsigned int NumBuffers,
                        ID3D11Buffer* const* ppConstantBuffers);

  /**
   * @brief Sets the shader resources for the pixel shader stage.
   */
  void
  PSSetShaderResources(unsigned int StartSlot,
                      unsigned int NumViews,
                      ID3D11ShaderResourceView* const* ppShaderResourceViews);

  /**
   * @brief Sets the samplers for the pixel shader stage.
   */
  void
  PSSetSamplers(unsigned int StartSlot,
                unsigned int NumSamplers,
                ID3D11SamplerState* const* ppSamplers);

  /**
   * @brief Draws indexed primitives.
   */
  void 
  DrawIndexed(unsigned int IndexCount,
              unsigned int StartIndexLocation,
              INT BaseVertexLocation);

public :
  ID3D11DeviceContext* m_deviceContext = nullptr;

private:

};