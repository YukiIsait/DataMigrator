#include <stdexcept>
#include <optional>
#include "Business/MigrationTool.h"
#include "Common/EntryPoint.h"
#include "Common/MessageBox.h"
#include "Common/FileUtil.h"
#include "Common/PathUtil.h"
#include "Common/Profile.h"

int Win32::EntryPoint::Main(const std::vector<std::wstring>& args) {
    try {
        if (Win32::MessageBox::Show(L"Do you want to proceed with the migration?", L"Data Migrator",
                                    static_cast<uint32_t>(Win32::MessageBox::ButtonType::YesNo) |
                                        static_cast<uint32_t>(Win32::MessageBox::IconType::Information)) == Win32::MessageBox::ReturnCode::No) {
            return 0;
        }
        std::optional<std::wstring> profileFileName;
        switch (args.size()) {
            case 0:
                profileFileName = L"Migrations.mil";
                break;
            case 1:
                if (Win32::FileUtil::IsDirectory(args[0])) {
                    profileFileName = Win32::PathUtil::Combine(args[0], L"Migrations.mil");
                } else {
                    profileFileName = args[0];
                }
                break;
            default:
                throw std::runtime_error("Invalid number of arguments provided.");
        }
        Business::MigrationTool::Migrate(*profileFileName);
        Win32::MessageBox::Show(L"All directories have been successfully migrated.", L"Migration completed", static_cast<uint32_t>(Win32::MessageBox::IconType::Information));
        return 0;
    } catch (const std::exception& exception) {
        Win32::MessageBox::Show(exception.what(), "Migration failed", static_cast<uint32_t>(Win32::MessageBox::IconType::Error));
        return 1;
    }
}
