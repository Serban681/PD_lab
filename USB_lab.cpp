#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>

#pragma comment(lib, "setupapi.lib")

void ListUSBDevices() {
    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD i;

    hDevInfo = SetupDiGetClassDevs(NULL, L"USB", 0, DIGCF_PRESENT | DIGCF_ALLCLASSES);
    if (hDevInfo == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to get device list.\n";
        return;
    }

    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &DeviceInfoData); i++) {
        char deviceName[256];

        if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &DeviceInfoData, SPDRP_FRIENDLYNAME,
            NULL, (PBYTE)deviceName, sizeof(deviceName), NULL)) {
            std::cout << "Device: " << deviceName << std::endl;
        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
}

int main() {
    ListUSBDevices();
    return 0;
}
