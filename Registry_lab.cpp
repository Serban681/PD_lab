// Registry_lab.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <iostream>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void QueryKey(HKEY hKey)
{
    TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
    DWORD    cbName = 0;                   // size of name string 
    TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // size of class string 
    DWORD    cSubKeys = 0;               // number of subkeys 
    DWORD    cbMaxSubKey = 0;              // longest subkey size 
    DWORD    cchMaxClass = 0;              // longest class string 
    DWORD    cValues = 0;              // number of values for key 
    DWORD    cchMaxValue = 0;          // longest value name
    DWORD    cbMaxValueData = 0;       // longest value data 
    DWORD    cbSecurityDescriptor = 0; // size of security descriptor 
    FILETIME ftLastWriteTime;      // last write time 

    DWORD i = 0, j = 0, retCode = 0;

    TCHAR  achValue[MAX_VALUE_NAME] = { '\0' };
    DWORD cchValue = MAX_VALUE_NAME;

    // Get the class name and the value count. 
    retCode = ::RegQueryInfoKey(
        hKey,                    // key handle 
        achClass,                // buffer for class name 
        &cchClassName,           // size of class string 
        NULL,                    // reserved 
        &cSubKeys,               // number of subkeys 
        &cbMaxSubKey,            // longest subkey size 
        &cchMaxClass,            // longest class string 
        &cValues,                // number of values for this key 
        &cchMaxValue,            // longest value name 
        &cbMaxValueData,         // longest value data 
        &cbSecurityDescriptor,   // security descriptor 
        &ftLastWriteTime);       // last write time 



    // Enumerate the subkeys, until RegEnumKeyEx fails.
    if (cSubKeys)
    {
        printf("\nNumber of subkeys: %d\n", cSubKeys);

        for (i = 0; i < cSubKeys; i++)
        {
            cbName = MAX_KEY_LENGTH;
            retCode = ::RegEnumKeyEx(hKey, i,
                achKey,
                &cbName,
                NULL,
                NULL,
                NULL,
                &ftLastWriteTime);

            if (retCode == ERROR_SUCCESS)
            {
                HKEY hSubKey;

                if (::RegOpenKeyEx(
                    hKey,
                    achKey,
                    0,
                    KEY_READ,
                    &hSubKey
                ) == ERROR_SUCCESS)
                {
                    DWORD cValuesSubDir = 0;
                    DWORD index = 0;

                    DWORD retCodeSubDir = ::RegQueryInfoKey(
                        hSubKey,
                        NULL,           // buffer for class name 
                        NULL,           // size of class string 
                        NULL,           // reserved 
                        NULL,           // number of subkeys 
                        NULL,           // longest subkey size 
                        NULL,           // longest class string 
                        &cValuesSubDir, // number of values for this key 
                        NULL,           // longest value name 
                        NULL,           // longest value data 
                        NULL,           // security descriptor 
                        NULL
                    );

                    //_tprintf(TEXT("(%d) Opened subkey for %s and has %d values \n"), i + 1, achKey, cValuesSubDir);
                    
                    if (cValuesSubDir)
                    {
                        DWORD cchValueSubDir = MAX_VALUE_NAME;
                        TCHAR achValueSubDir[MAX_VALUE_NAME] = { '\0' };
                        BYTE lpDataSubDir[1024];
                        DWORD cbDataSubDir = sizeof(lpDataSubDir);
                        DWORD lpTypeSubDir;

                        for (index = 0; index < cValuesSubDir; index++) {
                            achValueSubDir[0] = '\0';

                            retCode = ::RegEnumValue(hSubKey, index,
                                achValueSubDir,
                                &cchValueSubDir,
                                NULL,
                                &lpTypeSubDir,
                                lpDataSubDir,
                                &cbDataSubDir);

                            if (retCode == ERROR_SUCCESS) {
                                if (lstrcmp(achValueSubDir, _T("ImagePath")) == 0)
                                    _tprintf(TEXT("(%d) %s %s \n"), i + 1, achValueSubDir, lpDataSubDir);
                            }
                        }
                    }
                }
            }
        }
    }
}

int main()
{
    HKEY hTestKey = 0;

    if (::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
        TEXT("SYSTEM\\CurrentControlSet\\Services"),
        0,
        KEY_READ,
        &hTestKey) == ERROR_SUCCESS
        )
    {
        QueryKey(hTestKey);
    }

    ::RegCloseKey(hTestKey);

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
