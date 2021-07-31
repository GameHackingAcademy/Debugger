#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>

int main(int argc, char** argv) {
	HANDLE process_snapshot = NULL;
	HANDLE thread_handle = NULL;
	HANDLE process_handle = NULL;

	PROCESSENTRY32 pe32 = { 0 };

	DWORD pid;
	DWORD continueStatus = DBG_CONTINUE;
	DWORD bytes_written = 0;

	BYTE instruction_break = 0xcc;
	BYTE instruction_normal = 0x8b;

	DEBUG_EVENT debugEvent = { 0 };

	CONTEXT context = { 0 };

	bool first_break_has_occurred = false;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	process_snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	Process32First(process_snapshot, &pe32);

	do {
		if (wcscmp(pe32.szExeFile, L"ac_client.exe") == 0) {
			pid = pe32.th32ProcessID;

			//new code
			process_handle = OpenProcess(PROCESS_ALL_ACCESS, true, pe32.th32ProcessID);
			WriteProcessMemory(process_handle, (void*)0x0046366C, &instruction_break, 1, &bytes_written);
			//

		}
	} while (Process32Next(process_snapshot, &pe32));

	DebugActiveProcess(pid);

	for (;;) {
		continueStatus = DBG_CONTINUE;

		if (!WaitForDebugEvent(&debugEvent, INFINITE))
			return 0;

		switch (debugEvent.dwDebugEventCode) {
		case EXCEPTION_DEBUG_EVENT:
			switch (debugEvent.u.Exception.ExceptionRecord.ExceptionCode)
			{
			case EXCEPTION_BREAKPOINT:
				printf("Breakpoint hit");

				//new 2
				if (first_break_has_occurred) {
					thread_handle = OpenThread(THREAD_ALL_ACCESS, true, debugEvent.dwThreadId);
					if (thread_handle != NULL) {
						context.ContextFlags = CONTEXT_ALL;
						GetThreadContext(thread_handle, &context);

						context.Eip--;

						SetThreadContext(thread_handle, &context);
						CloseHandle(thread_handle);

						WriteProcessMemory(process_handle, (void*)0x0046366C, &instruction_normal, 1, &bytes_written);
					}
				}

				first_break_has_occurred = true;
				continueStatus = DBG_CONTINUE;
				//
				break;
			default:
				continueStatus = DBG_EXCEPTION_NOT_HANDLED;
				break;
			}
			break;
		default:
			continueStatus = DBG_EXCEPTION_NOT_HANDLED;
			break;
		}

		ContinueDebugEvent(debugEvent.dwProcessId, debugEvent.dwThreadId, continueStatus);
	}

	CloseHandle(process_handle);

	return 0;
}
