#include <engine.h>
#include <player.h>

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <unistd.h>

#define WIDTH 240
#define HEIGHT 160

Engine engine;
Player player = {
  .formatContext = NULL,
  .videoCodec = NULL,
  .codecParameters = NULL,
};
SDL_Event event;
uint32_t tick;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
  switch (message) {
  case WM_COMMAND: {
    HWND textbox = GetDlgItem(hwnd, 101);

    if (LOWORD(wParam) == 102) {
      OPENFILENAME ofn;
      WCHAR szFile[MAX_PATH] = {0};

      ZeroMemory(&ofn, sizeof(ofn));
      ofn.lStructSize = sizeof(ofn);
      ofn.hwndOwner = hwnd;
      ofn.lpstrFilter = TEXT("MP4 Files (*.mp4)\0*.mp4\0");
      ofn.lpstrFile = (LPSTR)szFile;
      ofn.nMaxFile = MAX_PATH;
      ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
      ofn.lpstrDefExt = TEXT("");

      if (GetOpenFileName(&ofn)) {
        SetWindowText(textbox, ofn.lpstrFile);
      }
    } else if (LOWORD(wParam) == 103) {
      char buffer[MAX_PATH];

      GetWindowTextA(textbox, buffer, MAX_PATH);

      if (!access(buffer, 0) == 0) {
        MessageBox(NULL, "Path error!", "Error!", MB_ICONEXCLAMATION | MB_OK);
        break;
      }

      Init(&engine);
      CreateTexture(&player, &engine, buffer);
      DestroyWindow(hwnd);

      while (1) {
        SDL_PollEvent(&event);

        if (event.type == SDL_QUIT) {
          Destroy(&engine);
          RemoveTexture(&player);
          return 0;
        }

        UpdateTexture(&player, &engine);

        tick = SDL_GetTicks();

        if (DELAY > SDL_GetTicks() - tick) {
          SDL_Delay(DELAY - (SDL_GetTicks() - tick));
        }
      }
    }
    break;
  }

  case WM_GETMINMAXINFO: {
    MINMAXINFO* mmi = (MINMAXINFO*)lParam;
    mmi->ptMinTrackSize.x = WIDTH;
    mmi->ptMinTrackSize.y = HEIGHT;
    mmi->ptMaxTrackSize.x = WIDTH;
    mmi->ptMaxTrackSize.y = HEIGHT;
    break;
  }

  case WM_CLOSE: {
    DestroyWindow(hwnd);
    break;
  }

  case WM_DESTROY: {
    PostQuitMessage(0);
    break;
  }

  default:
    return DefWindowProc(hwnd, message, wParam, lParam);
  }

  return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nCmdShow) {
  WNDCLASSEX wc = {
    .cbSize = sizeof(WNDCLASSEX),
    .style = 0,
    .lpfnWndProc = WndProc,
    .cbClsExtra = 0,
    .cbWndExtra = 0,
    .hInstance = hInstance,
    .hIcon = LoadIcon(NULL, IDI_APPLICATION),
    .hCursor = LoadCursor(NULL, IDC_ARROW),
    .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
    .lpszMenuName = NULL,
    .lpszClassName = "Wallpaper Engine",
    .hIconSm = LoadIcon(NULL, IDI_APPLICATION),
  };

  if (!RegisterClassEx(&wc)) {
    MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  HWND hwnd = CreateWindowEx(
    WS_EX_CLIENTEDGE,
    "Wallpaper Engine",
    "Wallpaper Engine GUI",
    WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX,
    CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT,
    NULL, NULL, hInstance, NULL);

  if (hwnd == NULL) {
    MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  int textboxW = 200;
  int textboxH = 20;
  int textboxY = HEIGHT / 2 - textboxH * 3;

  CreateWindowEx(0, WC_EDIT, "", WS_CHILD | WS_VISIBLE | WS_BORDER,
    WIDTH / 2 - textboxW / 2 - 10, textboxY, 200, 20, hwnd, (HMENU)101, hInstance, NULL);

  int BTNW = 120;
  int BTNH = 20;
  int BTNX = WIDTH / 2 - BTNW / 2 - 10;
  int BTNMARGIN = BTNH * 1.5;
  int BTNY = textboxY + BTNMARGIN;
  
  CreateWindowEx(0, WC_BUTTON, "Find Files", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
    BTNX, BTNY, BTNW, BTNH, hwnd, (HMENU)102, hInstance, NULL);

  CreateWindowEx(0, WC_BUTTON, "Apply", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
    BTNX, BTNY + BTNMARGIN, BTNW, BTNH, hwnd, (HMENU)103, hInstance, NULL);

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  MSG message;

  while (GetMessage(&message, NULL, 0, 0) > 0) {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }

  return message.wParam;
}