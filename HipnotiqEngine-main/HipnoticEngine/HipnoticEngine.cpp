#include "BaseApp.h"
#include "UserInterface.h"

BaseApp g_app;

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK
WndProc(HWND hWnd, unsigned int message, WPARAM wParam, LPARAM lParam) {
  PAINTSTRUCT ps;
  HDC hdc;

  if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
    return true;

  switch (message) {
  case WM_PAINT:
    hdc = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    break;

  case WM_SIZE:
    g_app.ResizeWindow(hWnd, lParam);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_KEYDOWN:
    g_app.keys[wParam] = true;
    break;

  case WM_KEYUP:
    g_app.keys[wParam] = false;
    break;

  case WM_RBUTTONDOWN:
    g_app.mouseLeftDown = true;
    break;

  case WM_RBUTTONUP:
    g_app.mouseLeftDown = false;
    break;

  case WM_MOUSEMOVE:
    if (g_app.mouseLeftDown) {
      int x = LOWORD(lParam);
      int y = HIWORD(lParam);
      g_app.RotateCamera(x, y);
    }
    break;

  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }

  return 0;
}

int WINAPI
wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
  return g_app.run(hInstance, hPrevInstance, lpCmdLine, nCmdShow, WndProc);
}