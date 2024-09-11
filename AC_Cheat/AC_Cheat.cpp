#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>


// 특정 프로세스의 이름으로 PID를 찾는 함수
DWORD GetProcessID(const wchar_t* processName) {
    DWORD processID = 0;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  // 전체 프로세스의 스냅샷 생성

    if (hSnapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 processEntry;
        processEntry.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnapshot, &processEntry)) {  // 첫 번째 프로세스 찾기
            do {
                // 찾고자 하는 프로세스 이름과 현재 프로세스 이름이 일치하는지 확인
                if (!_wcsicmp(processEntry.szExeFile, processName)) {
                    processID = processEntry.th32ProcessID;  // 일치하는 프로세스의 PID 저장
                    break;
                }
            } while (Process32Next(hSnapshot, &processEntry));  // 다음 프로세스 열거
        }
    }

    CloseHandle(hSnapshot); 
    return processID;
}


int main() {
    const wchar_t* processName = L"ac_client.exe";
    DWORD processID = GetProcessID(processName);
    if (processID == 0) {
        std::cout << "프로세스를 찾을 수 없습니다." << std::endl;
        return 1;
    }
    else {
        std::cout << "Process ID: " << processID << std::endl;
    }

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION, FALSE, processID);
    if (hProcess == NULL) {
        std::cout << "OpenProcess 실패: " << std::dec << GetLastError() << std::endl;
        return 1;
    }

    uintptr_t pointerAddress = 0x57E254;
    uintptr_t OffsetHP = 0xEC;
    uintptr_t offsetAmmo = 0x140;

   // uintptr_t Offset_NOCLIP = 0x58; // float
    uintptr_t Offset_RIFLE_DELAY = 0x178; // int32
    uintptr_t offset_SMG_DELAY = 0x170;
    uintptr_t offset_VIEW_X = 0x34;
    uintptr_t offset_VIEW_Y = 0x38;
    uintptr_t offset_NAME = 0x205;

    /* 다른 사람이 찾은 오프셋(1.3.0.2 버전 기준)
    Position X                [0x2C]
    Position Y                [0x30]
    Position Z                [0x28]
    Head Position X           [0x4]
    Head Position Y           [0xC]
    Head Position Z           [0x8]
    Fast fire Assault Rifle   [0x164]
    Fast fire Sniper          [0x160]
    Fast fire Shotgun         [0x158]
    recoil1                   ac_client.exe+17E0A8 [364] [C] [5E] 
    recoil2                   ac_client.exe+17E0A8 [364] [C] [60]
    */



    DWORD baseAddress = 0;
    ReadProcessMemory(hProcess, (LPCVOID)pointerAddress, &baseAddress, sizeof(baseAddress), NULL);
    // uintptr_t baseAddress = 0x00681550;
    std::cout << "baseAddress: " << std::hex << baseAddress << std::endl;

    BOOL wpmReturn;
    int choice = -1; 

    while (choice != 0) {
        std::cout << "\n메뉴를 선택하세요.\n[1] 체력\n[2] 탄창\n[0] 종료\n>> ";
        std::cin >> choice;

        if (choice == 1) {
            uintptr_t healthAddress = baseAddress + OffsetHP;
            int newHealthValue = 0;
            std::cout << "체력 주소: " << std::hex << healthAddress << std::endl;
            std::cout << "체력값 입력: ";
            std::cin >> newHealthValue;
            wpmReturn = WriteProcessMemory(hProcess, (LPVOID)healthAddress, &newHealthValue, sizeof(newHealthValue), NULL);
            if (wpmReturn == NULL) {
                std::cout << "WPM 실패: " << std::dec << GetLastError() << std::endl;
            }
            else {
                std::cout << "체력값이 " << std::dec << newHealthValue << "로 변경되었습니다." << std::endl;
            }
        }
        else if (choice == 2) {
            uintptr_t ammoAddress = baseAddress + offsetAmmo;
            int newAmmoValue = 0;
            std::cout << "탄창값 입력: ";
            std::cin >> newAmmoValue;
            wpmReturn = WriteProcessMemory(hProcess, (LPVOID)ammoAddress, &newAmmoValue, sizeof(newAmmoValue), NULL);
            if (wpmReturn == NULL) {
                std::cout << "WPM 실패: " << std::dec << GetLastError() << std::endl;

            }
            else {
                std::cout << "탄창이 " << std::dec << newAmmoValue << "발로 변경되었습니다." << std::endl;
            }
        }/*
        else if (choice == 3) {
            uintptr_t noclipAddress = baseAddress + Offset_NOCLIP;
            float newNoclipValue = 0.0f;
            wpmReturn = WriteProcessMemory(hProcess, (LPVOID)noclipAddress, &newNoclipValue, sizeof(newNoclipValue), NULL);
            if (wpmReturn == NULL) {
                std::cout << "WPM 실패: " << std::dec << GetLastError() << std::endl;

            }
            else {
                std::cout << "NoClip 활성화 완료." << std::endl;
            }
            
        }
        */
        else if (choice == 0) {
            std::cout << "프로그램을 종료합니다." << std::endl;
        }
        else {
            std::cout << "존재하지 않는 메뉴입니다." << std::endl;
        }
    }

    CloseHandle(hProcess);
    return 0;
}
