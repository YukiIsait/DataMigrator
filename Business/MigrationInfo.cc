#include <stdexcept>
#include "MigrationInfo.h"

namespace Business {
    MigrationInfo::MigrationInfo(const std::wstring& mappingDirectory, const std::wstring& storageDirectory, Operation operation): mappingDirectory(mappingDirectory), storageDirectory(storageDirectory), operation(operation) {}

    MigrationInfo::MigrationInfo(const Win32::Profile& profile, const std::wstring& section): MigrationInfo(profile.ReadString(section, L"MappingDirectory"), profile.ReadString(section, L"StorageDirectory"), ResolveOperation(profile.ReadString(section, L"Operation"))) {}

    void MigrationInfo::WriteToProfile(const Win32::Profile& profile, const std::wstring& section) const {
        profile.WriteString(section, L"MappingDirectory", mappingDirectory);
        profile.WriteString(section, L"StorageDirectory", storageDirectory);
        profile.WriteString(section, L"Operation", ResolveOperation(operation));
    }

    MigrationInfo::Operation MigrationInfo::ResolveOperation(const std::wstring& operation) {
        if (operation == L"Move") {
            return Operation::Move;
        } else if (operation == L"Map") {
            return Operation::Map;
        } else if (operation == L"Erase") {
            return Operation::Erase;
        } else {
            throw std::runtime_error("Invalid migration operation.");
        }
    }

    std::wstring MigrationInfo::ResolveOperation(Operation operation) {
        switch (operation) {
            case MigrationInfo::Operation::Move:
                return L"Move";
            case MigrationInfo::Operation::Map:
                return L"Map";
            case MigrationInfo::Operation::Erase:
                return L"Erase";
            default:
                throw std::runtime_error("Invalid migration operation.");
        }
    }
}
