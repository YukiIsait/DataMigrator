#pragma once
#include <string>
#include "Common/Profile.h"

namespace Business {
    struct MigrationInfo {
        enum class Operation {
            Move,
            Map,
            Erase
        };

        std::wstring mappingDirectory;
        std::wstring storageDirectory;
        Operation operation;

        MigrationInfo(const std::wstring& mappingDirectory, const std::wstring& storageDirectory, Operation operation);
        MigrationInfo(const Win32::Profile& profile, const std::wstring& section);
        void WriteOperationToProfile(const Win32::Profile& profile, const std::wstring& section) const;

    private:
        static Operation ResolveOperation(const std::wstring& operation);
        static std::wstring ResolveOperation(Operation operation);
        static std::wstring ResolvePath(const std::wstring& path);
    };
}
