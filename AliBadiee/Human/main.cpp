#include "Setup.h"
#include <windows.h>

// WndProc
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
switch (msg) {
case WM_DESTROY:
::PostQuitMessage(0);
break;

case WM_KEYDOWN:
if (wParam == VK_ESCAPE)
::DestroyWindow(hwnd);
break;
}

return ::DefWindowProc(hwnd, msg, wParam, lParam);
}



// WinMain
int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) {


if (!d3d::InitD3D(hinstance, Width, Height, true, D3DDEVTYPE_HAL, &Device)) {
::MessageBox(0, (LPCWSTR)"InitD3D() - FAILED", 0, 0);
return 0;
}

if (!Setup()) {
::MessageBox(0, (LPCWSTR)"Setup() - FAILED", 0, 0);
return 0;
}

d3d::EnterMsgLoop(Display);

Cleanup();
Device->Release();

return 0;
}
