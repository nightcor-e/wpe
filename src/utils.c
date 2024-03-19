#include <utils.h>

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  if (FindWindowEx(hwnd, NULL, "SHELLDLL_DefView", NULL)) {
    *(HWND *)lParam = FindWindowEx(NULL, hwnd, "WorkerW", NULL);
    return FALSE;
  }

  return TRUE;
}

HWND GetDesktopW() {
  HWND progman = FindWindow("ProgMan", NULL);
  HWND desktop = NULL;

  SendMessageTimeout(progman, 0x052C, 0, 0, 0x0000, 1000, NULL);
  EnumWindows(EnumWindowsProc, (LPARAM)&desktop);

  return desktop;
}