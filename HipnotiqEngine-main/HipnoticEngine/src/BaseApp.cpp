#include "BaseApp.h"
HRESULT
BaseApp::init() {
  HRESULT hr = S_OK;

  // Create Swapchain and BackBuffer
  hr = m_swapchain.init(m_device, m_deviceContext, m_backBuffer, m_window);
  if (FAILED(hr)) {
    return hr;
  }

  // Create a Render Target View
  hr = m_renderTargetView.init(m_device,
    m_backBuffer,
    DXGI_FORMAT_R8G8B8A8_UNORM);
  if (FAILED(hr)) {
    return hr;
  }

  // Create a Depth Stencil texture
  hr = m_depthStencil.init(m_device,
    m_window.m_width,
    m_window.m_height,
    DXGI_FORMAT_D24_UNORM_S8_UINT,
    D3D11_BIND_DEPTH_STENCIL, 4, 0);
  if (FAILED(hr)) {
    return hr;
  }

  // Create a Depth Stencil View
  hr = m_depthStencilView.init(m_device,
    m_depthStencil,
    DXGI_FORMAT_D24_UNORM_S8_UINT);
  if (FAILED(hr)) {
    return hr;
  }

  m_viewport.init(m_window);
  std::vector<D3D11_INPUT_ELEMENT_DESC> Layout;

  D3D11_INPUT_ELEMENT_DESC position;
  position.SemanticName = "POSITION";
  position.SemanticIndex = 0;
  position.Format = DXGI_FORMAT_R32G32B32_FLOAT;
  position.InputSlot = 0;
  position.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT /*0*/;
  position.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  position.InstanceDataStepRate = 0;
  Layout.push_back(position);

  D3D11_INPUT_ELEMENT_DESC texcoord;
  texcoord.SemanticName = "TEXCOORD";
  texcoord.SemanticIndex = 0;
  texcoord.Format = DXGI_FORMAT_R32G32_FLOAT;
  texcoord.InputSlot = 0;
  texcoord.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT /*12*/;
  texcoord.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  texcoord.InstanceDataStepRate = 0;
  Layout.push_back(texcoord);

  // Create the Shader Program
  hr = m_shaderProgram.init(m_device, "KerberosEngine.fx", Layout);
  if (FAILED(hr)) {
    return hr;
  }

  // Create the constant buffers
  hr = m_neverChanges.init(m_device, sizeof(CBNeverChanges));
  if (FAILED(hr))
    return hr;

  hr = m_changeOnResize.init(m_device, sizeof(CBChangeOnResize));
  if (FAILED(hr))
    return hr;

  // Initialize the view matrix
  XMVECTOR Eye = XMVectorSet(0.0f, 3.0f, -6.0f, 0.0f);
  XMVECTOR At = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  m_View = XMMatrixLookAtLH(Eye, At, Up);

  m_userInterface.init(m_window.m_hWnd, m_device.m_device, m_deviceContext.m_deviceContext);

  // Carga del modelo fbx
  Texture head;
  head.init(m_device, "Textures/T_F_MED_THFashion_Head_D.tga.png", ExtensionType::PNG);

  Texture hair;
  hair.init(m_device, "Textures/T_F_MED_TreasureHunterFashion_Hair_D.tga.png", ExtensionType::PNG);

  Texture body;
  body.init(m_device, "Textures/T_F_MED_TreasureHunterFashion_Body_D.tga.png", ExtensionType::PNG);

  m_modelTextures.push_back(body);
  m_modelTextures.push_back(head);
  m_modelTextures.push_back(hair);

  m_modelLoaderFbx.LoadFBXModel("model/Aura_Pose.fbx");
  Aaura = EngineUtilities::MakeShared<Actor>(m_device);
  if (!Aaura.isNull()) {
    // Init Transform
    Aaura->getComponent<Transform>()->setTransform(EngineUtilities::Vector3(2.0f, -0.5f, 1.0f),
      EngineUtilities::Vector3(XM_PI / -2.0f, -1.8f, XM_PI / 2.0f),
      EngineUtilities::Vector3(0.04f, 0.04f, 0.04f));
    // Init Actor Mesh
    Aaura->setMesh(m_device, m_modelLoaderFbx.meshes);
    // Init Actor Textures
    Aaura->setTextures(m_modelTextures);

    std::string msg = Aaura->getName() + "Actor accessed successfully.";
    m_actors.push_back(Aaura);
    MESSAGE("Actor", "Actor", msg.c_str());
  }
  else {
    MESSAGE("Actor", "Actor", "Actor resource not found")
  }


  // Carga del modelo obj
  m_modelLoaderObj.LoadOBJ_model("model/FN Obiwan OBJ.obj");

  m_modelTextures2.clear();

  Texture hairo;
  hairo.init(m_device, "textures/T_Noble_Hair_D.png", PNG);

  Texture heado;
  heado.init(m_device, "textures/T_Noble_Head_D.png", PNG);

  Texture bodyo;
  bodyo.init(m_device, "textures/T_Noble_Body_D.png", PNG);

  m_modelTextures2.push_back(bodyo);
  m_modelTextures2.push_back(heado);
  m_modelTextures2.push_back(hairo);

  Aobi = EngineUtilities::MakeShared<Actor>(m_device);
  if (!Aobi.isNull()) {
    // Init Transform
    Aobi->getComponent<Transform>()->setTransform(EngineUtilities::Vector3(-0.7, 0.3f, -1.5f),
      EngineUtilities::Vector3(0.0f, 2.4f, 0.0f),
      EngineUtilities::Vector3(5.3f, 5.3f, 5.3f));
    // Init Actor Mesh
    Aobi->setMesh(m_device, m_modelLoaderObj.meshes);
    // Init Actor Textures
    Aobi->setTextures(m_modelTextures2);

    m_actors.push_back(Aobi);
    MESSAGE("Actor", "Aobi", (Aobi->getName() + " - Actor accessed successfully.").c_str());
  }
  else {
    ERROR("Actor", "Aobi", "Failed to create actor.");
  }

  // Carga del modelo obj 2
  m_modelLoaderObj2.LoadOBJ_model("model/goku.obj");

  m_modelTextures3.clear();

  Texture goku;
  goku.init(m_device, "textures/Tex_0000.png", PNG);

  m_modelTextures3.push_back(goku);

  Agoku = EngineUtilities::MakeShared<Actor>(m_device);
  if (!Agoku.isNull()) {
    // Init Transform
    Agoku->getComponent<Transform>()->setTransform(EngineUtilities::Vector3(-4.0, -1.0f, 1.5f),
      EngineUtilities::Vector3(0.0f, 2.0f, 0.0f),
      EngineUtilities::Vector3(2.0f, 2.0f, 2.0f));
    // Init Actor Mesh
    Agoku->setMesh(m_device, m_modelLoaderObj2.meshes);
    // Init Actor Textures
    Agoku->setTextures(m_modelTextures3);

    m_actors.push_back(Agoku);
    MESSAGE("Actor", "Agoku", (Agoku->getName() + " - Actor accessed successfully.").c_str());
  }
  else {
    ERROR("Actor", "Agoku", "Failed to create actor.");
  }


  return S_OK;
}

void
BaseApp::update() {
  m_userInterface.update();

  m_userInterface.inspector(m_actors);

  m_userInterface.Inspector(*Aaura->getComponent<Transform>());

  // Update our time
  static float t = 0.0f;
  if (m_swapchain.m_driverType == D3D_DRIVER_TYPE_REFERENCE)
  {
    t += (float)XM_PI * 0.0125f;
  }
  else {
    static DWORD dwTimeStart = 0;
    DWORD dwTimeCur = GetTickCount();
    if (dwTimeStart == 0)
      dwTimeStart = dwTimeCur;
    t = (dwTimeCur - dwTimeStart) / 1000.0f;
  }

  inputActionMap(t);

  // Initialize the projection matrix
  float FOV = XMConvertToRadians(90.0f);
  m_Projection = XMMatrixPerspectiveFovLH(FOV, m_window.m_width / (float)m_window.m_height, 0.01f, 10000.0f);

  UpdateCamera();

  cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
  m_changeOnResize.update(m_deviceContext, 0, nullptr, &cbChangesOnResize, 0, 0);

  // Actualiza los actores
  Aaura->update(0, m_deviceContext);
  Aobi->update(0, m_deviceContext);
  Agoku->update(0, m_deviceContext);
}

void
BaseApp::render() {
  // Clear the back buffer
  float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha

  // Set Viewport
  m_viewport.render(m_deviceContext);

  // Set Render Target View
  m_renderTargetView.render(m_deviceContext, m_depthStencilView, 1, ClearColor);

  // Set Depth Stencil View
  m_depthStencilView.render(m_deviceContext);

  // Render the cube
  // Set Buffers and Shaders for pipeline
  m_shaderProgram.render(m_deviceContext);
  m_deviceContext.IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // Set Constant Buffers and asign Shaders
  // g_deviceContext.m_deviceContext->VSSetShader(g_pVertexShader, NULL, 0);
  m_neverChanges.render(m_deviceContext, 0, 1);
  m_changeOnResize.render(m_deviceContext, 1, 1);
  m_changesEveryFrame.render(m_deviceContext, 2, 1);

  // Renderiza los actores
  Aaura->render(m_deviceContext);
  Aobi->render(m_deviceContext);
  Agoku->render(m_deviceContext);

  m_changesEveryFrame.render(m_deviceContext, 2, 1, true);

  // Render the UI
  m_userInterface.render();

  // Present our back buffer to our front buffer
  m_swapchain.present();
}

void
BaseApp::destroy() {
  if (m_deviceContext.m_deviceContext) m_deviceContext.m_deviceContext->ClearState();

  Aaura->destroy();
  Aobi->destroy();
  Agoku->destroy();
  
  m_changeOnResize.destroy();
  m_changesEveryFrame.destroy();
  m_neverChanges.destroy();
  m_shaderProgram.destroy();
  m_depthStencil.destroy();
  m_depthStencilView.destroy();
  m_renderTargetView.destroy();
  m_swapchain.destroy();
  m_deviceContext.destroy();
  m_device.destroy();
  m_userInterface.destroy();
}

HRESULT
BaseApp::ResizeWindow(HWND hWnd, LPARAM lParam)
{
  // Validar que Swapchain exista
  if (m_swapchain.g_pSwapChain) {
    // Destruir los recursos anteriores del RTV, DS, DSV
    m_renderTargetView.destroy();
    m_depthStencil.destroy();
    m_depthStencilView.destroy();
    m_backBuffer.destroy();

    // Redimensionar los datos del ancho y alto de la ventana
    m_window.m_width = LOWORD(lParam);
    m_window.m_height = HIWORD(lParam);

    // Redimensionar el buffer del swapchain
    HRESULT hr = m_swapchain.g_pSwapChain->ResizeBuffers(0,
      m_window.m_width,
      m_window.m_height,
      DXGI_FORMAT_R8G8B8A8_UNORM,
      0);
    // Validación del HRESULT
    if (FAILED(hr)) {
      ERROR("ResizeWindow", "SwapChain", "Failed to resize buffers");
      return hr;
    }

    // Recrear el backBuffer
    hr = m_swapchain.g_pSwapChain->GetBuffer(0,
      __uuidof(ID3D11Texture2D),
      reinterpret_cast<void**>(&m_backBuffer.m_texture));
    if (FAILED(hr)) {
      ERROR("ResizeWindow", "ResizeWindow", "Failed to recreate back buffer");
      return hr;
    }

    // Recrear el render target view
    hr = m_renderTargetView.init(m_device,
      m_backBuffer,
      DXGI_FORMAT_R8G8B8A8_UNORM);
    if (FAILED(hr)) {
      ERROR("ResizeWindow", "Render Target View", "Failed to create Render Target View");
      return hr;
    }

    // Recrear el depth stencil
    hr = m_depthStencil.init(m_device,
      m_window.m_width,
      m_window.m_height,
      DXGI_FORMAT_D24_UNORM_S8_UINT,
      D3D11_BIND_DEPTH_STENCIL,
      4,
      0);
    if (FAILED(hr)) {
      ERROR("ResizeWindow", "Depth Stencil", "Failed to create Depth Stencil");
      return hr;
    }

    // Recrear el depth stencil view
    hr = m_depthStencilView.init(m_device,
      m_depthStencil,
      DXGI_FORMAT_D24_UNORM_S8_UINT);
    if (FAILED(hr)) {
      ERROR("ResizeWindow", "Depth Stencil View", "Failed to create new Depth Stencil View");
      return hr;
    }

    // Actualizar el viewport
    m_viewport.init(m_window);

    // Actualizar la proyección
    m_Projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, m_window.m_width / (float)m_window.m_height, 0.01f, 100.0f);
    cbChangesOnResize.mProjection = XMMatrixTranspose(m_Projection);
    m_changeOnResize.update(m_deviceContext, 0, nullptr, &cbChangesOnResize, 0, 0);
  }
}

void
BaseApp::inputActionMap(float deltaTime) {
  float m_speed = 0.002f;
  float moveSpeedCamera = 0.01f;

  XMVECTOR pos = XMLoadFloat3(&m_camera.position);
  XMVECTOR forward = XMLoadFloat3(&m_camera.forward);
  XMVECTOR right = XMLoadFloat3(&m_camera.right);
  XMStoreFloat3(&m_camera.position, pos);
}

void
BaseApp::UpdateCamera() {
  // Convertir la dirección a vectores normalizados
  XMVECTOR pos = XMLoadFloat3(&m_camera.position);
  XMVECTOR dir = XMLoadFloat3(&m_camera.forward);
  XMVECTOR up = XMLoadFloat3(&m_camera.up);

  // Calcular la nueva vista
  m_View = XMMatrixLookAtLH(pos, pos + dir, up);

  // Transponer y actualizar el buffer de la vista
  cbNeverChanges.mView = XMMatrixTranspose(m_View);
  m_neverChanges.update(m_deviceContext, 0, nullptr, &cbNeverChanges, 0, 0);
}

void 
BaseApp::RotateCamera(int mouseX, int mouseY) {
  float offsetX = (mouseX - lastX) * sensitivity;
  float offsetY = (mouseY - lastY) * sensitivity;
  lastX = mouseX;
  lastY = mouseY;

  m_camera.yaw += offsetX;
  m_camera.pitch += offsetY;

  // Limitar la inclinación de la cámara
  if (m_camera.pitch > 1.5f) m_camera.pitch = 1.5f;
  if (m_camera.pitch < -1.5f) m_camera.pitch = -1.5f;

  // Recalcular la dirección hacia adelante
  XMVECTOR forward = XMVectorSet(
    cosf(m_camera.yaw) * cosf(m_camera.pitch),
    sinf(m_camera.pitch),
    sinf(m_camera.yaw) * cosf(m_camera.pitch),
    0.0f
  );

  XMVECTOR right = XMVector3Cross(forward, XMLoadFloat3(&m_camera.up));

  XMStoreFloat3(&m_camera.forward, XMVector3Normalize(forward));
  XMStoreFloat3(&m_camera.right, XMVector3Normalize(right));
}

int
BaseApp::run(HINSTANCE hInstance,
  HINSTANCE hPrevInstance,
  LPWSTR lpCmdLine,
  int nCmdShow,
  WNDPROC wndproc) {

  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  if (FAILED(m_window.init(hInstance, nCmdShow, wndproc)))
    return 0;

  if (FAILED(init())) {
    destroy();
    return 0;
  }

  // Main message loop
  MSG msg = { 0 };
  while (WM_QUIT != msg.message)
  {
    if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
    else {
      update();
      render();
    }
  }

  destroy();

  return (int)msg.wParam;
}