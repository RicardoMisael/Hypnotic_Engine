#pragma once
#include "Prerequisites.h"

class Device;
class DeviceContext;

class 
Texture {
public:
  /**
   * @brief Default constructor and destructor.
   */
  Texture() = default;
  ~Texture() = default;

  /**
   * @brief Initializes a texture from a file.
   */
  HRESULT
  init(Device device,
      const std::string& textureName,
      ExtensionType extensionType);

  /**
   * @brief Initializes a texture with specified parameters.
   */
  HRESULT
  init(Device device,
      unsigned int width,
      unsigned int height,
      DXGI_FORMAT Format,
      unsigned int BindFlags,
      unsigned int sampleCount = 1,
      unsigned int qualityLevels = 0);

  /**
   * @brief Updates the texture data.
   */
  void
  update();

  /**
   * @brief Renders the texture using the specified device context.
   */
  void
  render(DeviceContext& deviceContet, 
        unsigned int StartSlot, 
        unsigned int NumViews);

  /**
  * @brief Releases texture resources and cleans up memory.
  */
  void
  destroy();

public:
  // This variable is in charge of handle a texture resource as data
  ID3D11Texture2D* m_texture = nullptr;
  // This variable is in charge of handle a texture resource as image data
  ID3D11ShaderResourceView* m_textureFromImg;
};