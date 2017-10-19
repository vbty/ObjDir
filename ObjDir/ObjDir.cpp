// ObjDir.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "ObjectManager.h"

using namespace std;

#define STATUS_OBJECT_TYPE_MISMATCH      ((NTSTATUS)0xC0000024L)

int wmain(int argc, const wchar_t* argv[]) {
    CString root(L"\\");
    if (argc > 1) {
        root = argv[1];
    }

    ObjectManager mgr;
    NTSTATUS status;
    auto folders = mgr.GetObjects(root, status);
    if (status < 0) {
        if (status == STATUS_OBJECT_TYPE_MISMATCH) {
            cout << "Error: not a directory" << endl;
        }
        else {
            cout << "Error: " << hex << status << endl;
        }
        return 1;
    }

    sort(folders.begin(), folders.end(), [](const auto& pair1, const auto& pair2) {
        return pair1.first < pair2.first;
    });

    wcout << L"Directory: " << (PCWSTR)root << endl;

    for (const auto& [name, type] : folders) {
        wcout << setiosflags(ios_base::left) << setw(50) << setiosflags(ios_base::left) << 
            (PCWSTR)name << L" " << setw(20) << (PCWSTR)type;
        if (type == "SymbolicLink") {
            auto link = mgr.GetSymbolicLinkFromName(root, name);
            wcout << L" " << (PCWSTR)link;
        }
        wcout << endl;
    }

    return 0;
}

