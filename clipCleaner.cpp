#include <windows.h>
#include <shellapi.h>
#include "resource.h"

#define TRAY_ICON_ID 1
#define WM_TRAYICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001

void ClearClipboard() {
    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        CloseClipboard();
    }
}

void ShowContextMenu(HWND hwnd) {
    POINT pt;
    GetCursorPos(&pt);
    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, ID_TRAY_EXIT, "Exit");
    SetForegroundWindow(hwnd);
    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
    DestroyMenu(hMenu);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_TRAYICON:
            if (lParam == WM_LBUTTONUP) {
                ClearClipboard();
            } else if (lParam == WM_RBUTTONUP) {
                ShowContextMenu(hwnd);
            }
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_TRAY_EXIT) {
                PostMessage(hwnd, WM_CLOSE, 0, 0);
            }
            break;
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, (NOTIFYICONDATA*)GetWindowLongPtr(hwnd, GWLP_USERDATA));
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = {0};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "ClipboardCleanerClass";
    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, "ClipboardCleanerClass", NULL, 0, 0, 0, 0, 0, NULL, NULL, hInstance, NULL);
    
    NOTIFYICONDATA nid = {0};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = TRAY_ICON_ID;
    nid.uFlags = NIF_ICON | NIF_MESSAGE;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MYICON), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);
    Shell_NotifyIcon(NIM_ADD, &nid);
    
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&nid);
    
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)nid.hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)nid.hIcon);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}
