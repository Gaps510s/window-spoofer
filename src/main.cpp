#include <Windows.h>
#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")

std::string generateRandomString(int length) {
    const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()[]/.,;'=-_+";
    std::string randomString;
    for (int i = 0; i < length; ++i) {
        randomString += charset[rand() % charset.length()];
    }
    return randomString;
}

void updateWindowTitle(HWND hwnd) {
    std::string randomString = generateRandomString(10);
    SetWindowTextA(hwnd, randomString.c_str());
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
    DWORD processId;
    GetWindowThreadProcessId(hwnd, &processId);

    HANDLE processHandle = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processId);

    if (processHandle != NULL) {
        char szProcessName[MAX_PATH];
        if (GetModuleBaseNameA(processHandle, NULL, szProcessName, sizeof(szProcessName))) {
            std::string processName = szProcessName;
            std::string targetProcessName = reinterpret_cast<const char*>(lParam);

            if (processName == targetProcessName) {
                updateWindowTitle(hwnd);
            }
        }

        CloseHandle(processHandle);
    }

    return TRUE;
}

int main() {
    std::cout << "proc name:";
    std::string processName;
    std::cin >> processName;

    while (true) {
        EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(processName.c_str()));
        Sleep(50);
    }

    return 0;
}
