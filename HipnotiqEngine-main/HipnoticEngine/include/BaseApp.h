#include "Prerequisites.h"
#include "Window.h"
#include "Device.h"
#include "DeviceContext.h"
#include "Swapchain.h"
#include "Texture.h"
#include "RenderTargetView.h"
#include "DepthStencilView.h"
#include "Viewport.h"
#include "ShaderProgram.h"
#include "MeshComponent.h"
#include "Buffer.h"
#include "SamplerState.h"
#include "UserInterface.h"
#include "ModelLoader.h"
#include "ECS/Actor.h"

class
  BaseApp {
public:
  /**
   * @brief Constructor y destructor
   */
  BaseApp() = default;
  ~BaseApp() = default;

  /**
   * @brief Inicializa la aplicación con los recursos encesarios
   */
  HRESULT
    init();

  /**
   * @brief Actualiza la alicación en cada frame
   */
  void
    update();

  /**
   * @brief Renderiza el elemento
   */
  void
    render();

  /**
   * @brief Libera recursos y elementos antes de salir.
   */
  void
    destroy();

  /**
   * @brief Le hace resize a la ventana para que no se defigure al cambiar el tamaño
   */
  HRESULT
    ResizeWindow(HWND hWnd, LPARAM lParam);

  /**
  * @brief Maneja los inputs de la aplicaicón.
  */
  void
    inputActionMap(float deltaTime);

  /**
  * @brief Actualiza la matriz vista de la cámara
  */
  void
    UpdateCamera();

  /**
  * @brief Corre la aplicación
  */
  void
    RotateCamera(int mouseX, int mouseY);

  /**
   * @brief Ejecuta la aplicación
   */
  int
    run(HINSTANCE hInstance,
      HINSTANCE hPrevInstance,
      LPWSTR lpCmdLine,
      int nCmdShow,
      WNDPROC wndproc);

private:
  Window                              m_window;
  Device                              m_device;
  DeviceContext                       m_deviceContext;
  SwapChain                           m_swapchain;
  Texture                             m_backBuffer;
  Texture                             m_depthStencil;
  RenderTargetView                    m_renderTargetView;
  DepthStencilView                    m_depthStencilView;
  Viewport                            m_viewport;
  ShaderProgram                       m_shaderProgram;
  Buffer                              m_neverChanges;
  Buffer                              m_changeOnResize;
  Buffer                              m_changesEveryFrame;
  UserInterface                       m_userInterface;

  XMMATRIX                            m_View;
  XMMATRIX                            m_Projection;

  Camera                              m_camera;
  CBNeverChanges                      cbNeverChanges;
  CBChangeOnResize                    cbChangesOnResize;

  std::vector<EngineUtilities::TSharedPointer<Actor>> m_actors;

  ModelLoader                         m_modelLoaderFbx;
  EngineUtilities::TSharedPointer<Actor> Aaura;
  std::vector<Texture>                m_modelTextures;

  ModelLoader                         m_modelLoaderObj;
  EngineUtilities::TSharedPointer<Actor> Aobi;
  std::vector<Texture>                m_modelTextures2;

  ModelLoader                         m_modelLoaderObj2;
  EngineUtilities::TSharedPointer<Actor> Agoku;
  std::vector<Texture>                m_modelTextures3;

public:

  XMFLOAT4                            m_vMeshColor{ 0.7f, 0.7f, 0.7f, 1.0 };
  bool keys[256] = { false };
  int lastX;
  int lastY;
  float sensitivity = 0.01f;
  bool mouseLeftDown = false;
};