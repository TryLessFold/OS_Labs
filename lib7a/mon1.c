#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

// To ensure correct resolution of symbols, add Psapi.lib to TARGETLIBS
// and compile with -DPSAPI_VERSION=1

void PrintProcessNameAndID(DWORD processID)
{
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	TCHAR szProcessPath[MAX_PATH] = TEXT("<unknown>");
	// Get a handle to the process.
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID);

	// Get the process name.
	if (NULL != hProcess)
	{
		HMODULE hMod[16];
		DWORD cbNeeded;

		/*if (EnumProcessModules(hProcess, hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
		}*/
		if (EnumProcessModules(hProcess, hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod[0], szProcessName,
				sizeof(szProcessName) / sizeof(TCHAR));
			_tprintf(TEXT("\n%-20s(PID: %5u)\n"), szProcessName, processID);
			for (int i = 1; i < ((sizeof(hMod)>cbNeeded)?cbNeeded:sizeof(hMod)) / sizeof(hMod[0]); i++)
			{
				GetModuleBaseName(hProcess, hMod[i], szProcessName,
					sizeof(szProcessName) / sizeof(TCHAR));
				GetModuleFileNameExA(hProcess, hMod[i], szProcessPath,
					sizeof(szProcessPath) / sizeof(TCHAR));
				_tprintf(TEXT("%20s(PID: %5u) %s\n"), szProcessName, processID, szProcessPath);
			}
		}
	}

	// Print the process name and identifier.
	//_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

	// Release the handle to the process.
	CloseHandle(hProcess);
}

int main(void)
{
	// Get the list of process identifiers.
	DWORD aProcesses[1024], cbNeeded, cProcesses;
	unsigned int i;

	if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
	{
		return 1;
	}

	// Calculate how many process identifiers were returned.
	cProcesses = cbNeeded / sizeof(DWORD);

	// Print the name and process identifier for each process.
	for (i = 0; i < cProcesses; i++)
	{
		if (aProcesses[i] != 0)
		{
			PrintProcessNameAndID(aProcesses[i]);
		}
	}

	return 0;
}