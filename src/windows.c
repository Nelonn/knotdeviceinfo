/*
 * This file is part of knotdeviceinfo.
 *
 * For license and copyright information please follow this link:
 * https://github.com/noseam-env/knotdeviceinfo/blob/master/LEGAL
 */

#include "target.h"

#if defined(OS_WIN)

#include "knot/deviceinfo.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <Wbemidl.h>

#pragma comment(lib, "wbemuuid.lib")

// https://stackoverflow.com/questions/36543301/detecting-windows-10-version
//typedef LONG NTSTATUS;
//typedef NTSTATUS *PNTSTATUS;
#define STATUS_SUCCESS (0x00000000)

typedef NTSTATUS(WINAPI *RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

RTL_OSVERSIONINFOW GetRealOSVersion() {
    HMODULE hMod = GetModuleHandle("ntdll.dll");
    if (hMod) {
        RtlGetVersionPtr fxPtr = (RtlGetVersionPtr) GetProcAddress(hMod, "RtlGetVersion");
        if (fxPtr) {
            RTL_OSVERSIONINFOW osvi = {0};
            osvi.dwOSVersionInfoSize = sizeof(osvi);
            if (STATUS_SUCCESS == fxPtr(&osvi)) {
                return osvi;
            }
        }
    }
    RTL_OSVERSIONINFOW osvi = {0};
    return osvi;
}

char* GetWindowsVersion() {
    // Using strdup for allow memory free
    RTL_OSVERSIONINFOW osvi = GetRealOSVersion();
    if (osvi.dwMajorVersion == 10 && osvi.dwMinorVersion == 0) {
        if (osvi.dwBuildNumber >= 22000 && osvi.dwBuildNumber < 23000) {
            return _strdup("11");
        }
        return _strdup("10");
    } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 3) {
        return _strdup("8.1");
    } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 2) {
        return _strdup("8");
    } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 1) {
        return _strdup("7");
    } else if (osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0) {
        return _strdup("Vista");
    } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2) {
        return _strdup("Server 2003 or XP x64 Edition");
    } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1) {
        return _strdup("XP");
    } else if (osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0) {
        return _strdup("2000");
    }
    return NULL;
}

char* GetComputerModel() {
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        return NULL;
    }

    hres = CoInitializeSecurity(
            NULL,
            -1,
            NULL,
            NULL,
            RPC_C_AUTHN_LEVEL_DEFAULT,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE,
            NULL);
    if (FAILED(hres)) {
        CoUninitialize();
        return NULL;
    }

    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance(
            &CLSID_WbemLocator,
            NULL,
            CLSCTX_INPROC_SERVER,
            &IID_IWbemLocator,
            (LPVOID*) &pLoc
    );
    if (FAILED(hres)) {
        CoUninitialize();
        return NULL;
    }

    IWbemServices *pSvc = NULL;

    hres = pLoc->lpVtbl->ConnectServer(
            pLoc,
            SysAllocString(L"ROOT\\CIMV2"), // namespace
            NULL,
            NULL,
            NULL,
            0,
            NULL,
            NULL,
            &pSvc
    );
    if (FAILED(hres)) {
        pLoc->lpVtbl->Release(pLoc);
        CoUninitialize();
        return NULL;
    }

    hres = CoSetProxyBlanket(
            (IUnknown*) pSvc,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            NULL,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            NULL,
            EOAC_NONE);
    if (FAILED(hres)) {
        pSvc->lpVtbl->Release(pSvc);
        pLoc->lpVtbl->Release(pLoc);
        CoUninitialize();
        return NULL;
    }

    char* computerModel = NULL;

    IEnumWbemClassObject *pEnumerator = NULL;
    hres = pSvc->lpVtbl->ExecQuery(
            pSvc,
            SysAllocString(L"WQL"),
            SysAllocString(L"SELECT * FROM Win32_ComputerSystem"),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            NULL,
            &pEnumerator);
    if (SUCCEEDED(hres)) {
        IWbemClassObject *pclsObj = NULL;
        ULONG uReturn = 0;

        hres = pEnumerator->lpVtbl->Next(pEnumerator, WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 1) {
            VARIANT vtProp;

            hres = pclsObj->lpVtbl->Get(pclsObj, L"Model", 0, &vtProp, 0, 0);
            if (SUCCEEDED(hres)) {
                // https://learn.microsoft.com/en-us/windows/win32/api/stringapiset/nf-stringapiset-widechartomultibyte
                //UINT len = SysStringLen(vtProp.bstrVal);
                int utf8_len = WideCharToMultiByte(CP_UTF8, 0, vtProp.bstrVal, -1, NULL, 0, NULL, NULL);
                char* utf8 = (char*) calloc(utf8_len, sizeof(char));
                WideCharToMultiByte(CP_UTF8, 0, vtProp.bstrVal, -1, utf8, utf8_len, NULL, NULL);
                computerModel = utf8;

                VariantClear(&vtProp);
            }
        }

        pclsObj->lpVtbl->Release(pclsObj);
    }

    pEnumerator->lpVtbl->Release(pEnumerator);
    pSvc->lpVtbl->Release(pSvc);
    pLoc->lpVtbl->Release(pLoc);
    CoUninitialize();

    return computerModel;
}

void KNDeviceInfoFetch(KNDeviceInfo* info) {
    memset(info, 0, sizeof(KNDeviceInfo));

    info->platform = _strdup("Windows");
    info->system_name = _strdup("Windows");
    info->system_version = GetWindowsVersion();

    // uuid
    HKEY hKey;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        char uuid[256];
        DWORD uuidSize = sizeof(uuid);
        if (RegQueryValueExA(hKey, "MachineGuid", NULL, NULL,
                             (LPBYTE) uuid, &uuidSize) == ERROR_SUCCESS) {
            info->uuid = _strdup(uuid);
        }
        RegCloseKey(hKey);
    }

    // name
    char computerName[256];
    DWORD computerNameSize = sizeof(computerName);
    if (GetComputerNameExA(ComputerNameDnsHostname, computerName, &computerNameSize)) {
        info->name = _strdup(computerName);
    }

    // model
    info->model_raw = GetComputerModel();
}

#endif
