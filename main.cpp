#include <Windows.h>
#include <iostream>
#include "memory.hpp"
#include <string>

using namespace std;

char namedll[32];

Injector inj;

DWORD pid;

bool DoesFileExist(const char* name) {
	if (FILE* file = fopen(name, "r")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

void bypass()
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

int main()
{
	SetConsoleTitle("EZinjector Reborn for CS:GO");
	
	cout << "EZinjector Reborn\n\nVisit ezcheats.ru / ezcheats.com\n Owner: xvorost\n\n" << endl;

	inj.hwndproc = FindWindowA(0, "Counter-Strike: Global Offensive");

	GetWindowThreadProcessId(inj.hwndproc, &pid);
	inj.process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	inj.clientDLL = inj.GetModule(pid, "client.dll");

	cout << "Enter dll name:" << endl;

	scanf_s("%s", namedll);
	

	if (DoesFileExist(namedll)) {
		bypass();

			if (inj.inject(pid, namedll)) {
				cout << "Injection successful! You can close this window.\n\n" << endl;
					Sleep(5000);
					exit(0);
			}
			else
			{
				cout << "Injection failed, try run as Administrator.\n\n" << endl;
					system("pause");
			}

	}
	else
	{
		cout << "Cannot find : " << namedll <<"\n";
		system("pause");
	}

	return 0;
}
