#pragma once
#include "Prerequisites.h"

class
Window {
public:
  /**
   * @brief Default constructor and destructor.
   */
  Window() = default;
  ~Window() = default;

  /**
   * @brief Initializes the window with the given parameters.
   */
  HRESULT
  init(HINSTANCE hInstance, int nCmdShow, WNDPROC wndproc);

  /**
   * @brief Updates the window state and handles any necessary processing.
   */
  void
  update();

  /**
   * @brief Renders the window content.
   */
  void
  render();

  /**
   * @brief Cleans up resources and destroys the window.
   */
  void
  destroy();

public:
  HWND m_hWnd = nullptr;
  unsigned int m_width;
  unsigned int m_height;

private:
  HINSTANCE  m_hInst = nullptr;
  RECT m_rect;
  std::string m_windowName = "Hipnotic Engine";
};