#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <iostream>
#include "memory.hpp"
#include <string>

using namespace std;

Injector inj;

DWORD pid;

static bool DoesFileExist(const char* name) {
	if (FILE* file = fopen(name, "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

static void bypass()
{
	// Restore original NtOpenFile from external process
	LPVOID ntOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");
	if (ntOpenFile) {
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		WriteProcessMemory(inj.process, ntOpenFile, originalBytes, 5, NULL);
	} 
	else
	{
		cout << "Unable to bypass.\n";
	}
}

int main(const int argc, char** argv)
{
	SetConsoleTitle("injector by xvorost");

	inj.hwndproc = FindWindowA(0, "Counter-Strike 2");

	GetWindowThreadProcessId(inj.hwndproc, &pid);
	inj.process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	inj.clientDLL = inj.GetModule(pid, "client.dll");

	bypass();

	if (inj.inject(pid, argv[1])) {
		cout << "Injection successful! You can close this window.\n\n" << endl;
		Sleep(5000);
		exit(0);
	}
	else
	{
		cout << "Injection failed, try run as Administrator.\n\n" << endl;
		system("pause");
	}

	return 0;
}
