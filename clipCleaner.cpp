#include <windows.h>
#include <shellapi.h>

#define TRAY_ICON_ID 1
#define WM_TRAYICON (WM_USER + 1)

void ClearClipboard() {
    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        CloseClipboard();
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_TRAYICON:
            if (lParam == WM_LBUTTONUP) {
                ClearClipboard();
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
    nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    Shell_NotifyIcon(NIM_ADD, &nid);
    
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&nid);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return 0;
}
