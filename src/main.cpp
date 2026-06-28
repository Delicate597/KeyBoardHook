#include <windows.h>
#include <commctrl.h>
#include <fstream>
#include "WinProcFun.hpp"
#include "resourse.hpp"

#pragma comment(lib, "user32.lib")

#define taskbar_name "Window System"
#define TaskbarIco_Path ".\\icons\\taskbar.ico"

HWND hwnd_handle;
UINT_PTR timer_handle;
HHOOK hook_handle;

void Create_Wind(HINSTANCE);

int WINAPI WinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance,_In_ LPSTR lpCmdLine,_In_ int nCmdShow)
{
  outfile.open(outfile_path, std::ios::out | std::ios::trunc);
  try
  {
    if (!outfile.is_open())
      throw "error:when open file";
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    return 1;
  }

  HWND hwnd = GetConsoleWindow();
  try
  {
      if (hwnd == NULL)
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }
  ShowWindow(hwnd, SW_HIDE);

  hook_handle = SetWindowsHookExA(WH_KEYBOARD_LL, (HOOKPROC)&LowLevelKeyboardProc, NULL, 0);
  try
  {
    if (hook_handle == NULL)
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }

  timer_handle = SetTimer(NULL, 0, 60000, TimerProc); // 1min
  try
  {
    if (timer_handle == '\0')
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }

  Create_Wind(hPrevInstance);

  MSG msg = {};
  int flag = 1;
  while (flag != 0)
  {
    flag = GetMessage(&msg, NULL, 0, 0);
    try
    {
      if (flag > 0)
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
      else if (flag < 0)
        throw GetLastError();
    }
    catch (const char *msg)
    {
      outfile << msg;
    }
  };

  if (!outfile.is_open())
    outfile.close();
  if (hook_handle != NULL)
    UnhookWindowsHookEx(hook_handle);
  if (timer_handle != '\0')
    KillTimer(NULL, timer_handle);
  return msg.wParam;
}

void Create_Wind(HINSTANCE hInstance)
{
  const char CLASS_NAME[] = "Window System";
  WNDCLASSEXA wc = {};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style = CS_BYTEALIGNCLIENT;
  wc.lpfnWndProc = WindowProc;
  wc.hIcon = NULL;
  wc.hInstance = hInstance;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = CLASS_NAME;
  try
  {
    if (RegisterClassExA(&wc) == 0)
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }

  hwnd_handle = CreateWindowA(CLASS_NAME, "Window System", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
  try
  {
    if (hwnd_handle == NULL)
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }
  ShowWindow(hwnd_handle, SW_HIDE);

  NOTIFYICONDATAA taskbar;
  taskbar.cbSize = sizeof(NOTIFYICONDATA);
  taskbar.hWnd = hwnd_handle;
  taskbar.uID = 1;
  taskbar.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
  taskbar.uCallbackMessage = ID_CLICK;
  taskbar.hIcon = (HICON)LoadImageA(hInstance, TaskbarIco_Path, IMAGE_ICON, NULL, NULL, LR_LOADFROMFILE);
  strcpy_s(taskbar.szTip, sizeof(taskbar_name), taskbar_name);
  taskbar.dwState = NIS_SHAREDICON;
  Shell_NotifyIconA(NIM_ADD, &taskbar);

  popupMenu_handle = CreatePopupMenu();
  try
  {
    if (popupMenu_handle == NULL)
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }

  try
  {
    if (AppendMenuA(popupMenu_handle, MF_STRING, ID_ALLWAYS, "Allways") == 0)
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }
  try
  {
    if (AppendMenuA(popupMenu_handle, MF_STRING, ID_5M, "5min") == 0)
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }
  try
  {
    if (AppendMenuA(popupMenu_handle, MF_STRING, ID_10M, "10min") == 0)
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }
  try
  {
    if (AppendMenuA(popupMenu_handle, MF_STRING, ID_EXIT, "Exit") == 0)
      throw GetLastError();
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }
}
