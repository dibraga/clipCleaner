#include <windows.h>

int main() {
    if (OpenClipboard(NULL)) {
        EmptyClipboard();
        CloseClipboard();
    }
    return 0;
}
