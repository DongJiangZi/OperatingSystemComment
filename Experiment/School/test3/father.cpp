#include <iostream>
#include <windows.h>

using namespace std;

int main() {
	STARTUPINFO sui;
	ZeroMemory(&sui, sizeof(sui));
	sui.cb = sizeof(STARTUPINFO);
	PROCESS_INFORMATION pi;
	if(!CreateProcess(".\\child.exe", NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &sui, &pi)) {
		cout << "Process creation failed." << endl;
	} else {
		cout << "This is a SubProcess. Wait for event." << endl;
	}

	HANDLE hEvent = CreateEvent (NULL, FALSE, TRUE, "myEvent");
	ResetEvent(hEvent);
	DWORD flag = WaitForSingleObject(hEvent, 10000);
	if(WAIT_FAILED == flag) {
		cout << "Waiting for the event signal failed" << endl;
	} else if(WAIT_OBJECT_0 == flag) {
		cout << "Get the event" << endl;
	} else if(WAIT_TIMEOUT == flag) {
		cout << "Waiting for the event signal timeout" << endl;
	}
	return 0;
}