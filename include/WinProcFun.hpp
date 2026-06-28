#ifndef WINPROCFUN_HPP
#define WINPROCFUN_HPP

#define outfile_path ".\\log.txt"

extern std::ofstream outfile;
extern time_t stop_time;
extern HMENU popupMenu_handle;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);

#endif
