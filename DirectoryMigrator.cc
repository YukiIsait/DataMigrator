#include "MigrationTool.h"
#include "Win32Utils/EntryPoint.h"
#include "Win32Utils/MessageBox.h"
#include "Win32Utils/Util.h"
#include <string>

int Win32::EntryPoint::Main(int argc, wchar_t** argv) {
    std::wstring listFileName = L"Migrations.mil";
    if (argc == 1) {
        if (Win32::Util::PathIsDirectory(argv[0])) {
            std::optional<std::wstring> fileName = Win32::Util::PathCombine(argv[0], listFileName);
            if (!fileName) {
                Win32::MessageBox::Show(L"Invalid path.", L"Error", static_cast<uint32_t>(Win32::MessageBox::IconType::Error));
                return 1;
            }
            listFileName = std::move(*fileName);
        } else {
            listFileName = argv[0];
        }
    } else if (argc > 1) {
        Win32::MessageBox::Show(L"Invalid number of arguments.", L"Error", static_cast<uint32_t>(Win32::MessageBox::IconType::Error));
        return 1;
    }
    if (!Win32::Util::PathFileExists(listFileName)) {
        Win32::MessageBox::Show(L"File not found: " + listFileName, L"Error", static_cast<uint32_t>(Win32::MessageBox::IconType::Error));
        return 1;
    }
    if (!MigrationTool::MigrationTool::MigrateByListFile(listFileName)) {
        Win32::MessageBox::Show(L"Some migration tasks failed.", L"Warning", static_cast<uint32_t>(Win32::MessageBox::IconType::Warning));
        return 1;
    }
    Win32::MessageBox::Show(L"Migration completed.", L"Information", static_cast<uint32_t>(Win32::MessageBox::IconType::Information));
    return 0;
}
