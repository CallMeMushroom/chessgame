#include "clipboard.h"

void toClipboard(const std::string& str) {
    OpenClipboard(0);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, str.size() + 1);
    if (!hg) {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), str.c_str(), str.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
}

char* GetClipboard() {
    HANDLE clip;
    if (OpenClipboard(NULL)) {
        clip = GetClipboardData(CF_TEXT);
        CloseClipboard;
        return (char*)clip;
    }
}