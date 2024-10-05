#include <stdexcept>
#include "MigrationInfo.h"
#include "Common/ProcessUtil.h"
#include "Common/PathUtil.h"

namespace Business {
    MigrationInfo::MigrationInfo(const std::wstring& mappingDirectory, const std::wstring& storageDirectory, Operation operation): mappingDirectory(mappingDirectory), storageDirectory(storageDirectory), operation(operation) {}

    MigrationInfo::MigrationInfo(const Win32::Profile& profile, const std::wstring& section): MigrationInfo(ResolvePath(profile, section, L"MappingDirectory"), ResolvePath(profile, section, L"StorageDirectory"), ResolveOperation(profile.ReadString(section, L"Operation"))) {}

    void MigrationInfo::WriteOperationToProfile(const Win32::Profile& profile, const std::wstring& section) const {
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

    std::wstring MigrationInfo::ResolvePath(const Win32::Profile& profile, const std::wstring& section, const std::wstring& key) {
        std::wstring path = profile.ReadString(section, key);
        std::wstring resolvedPath;
        size_t position = 0;
        while (true) {
            size_t openBracket = path.find(L'<', position);
            if (openBracket == std::wstring::npos) {
                resolvedPath.append(path.substr(position));
                break;
            }
            resolvedPath.append(path.substr(position, openBracket - position));
            size_t closeBracket = path.find(L'>', openBracket + 1);
            if (closeBracket == std::wstring::npos) {
                throw std::runtime_error("Mismatched brackets in environment variable.");
            }
            std::wstring variableName = path.substr(openBracket + 1, closeBracket - openBracket - 1);
            std::wstring variableValue = Win32::ProcessUtil::GetEnvironment(variableName);
            resolvedPath.append(variableValue);
            position = closeBracket + 1;
        }
        return Win32::PathUtil::Combine(Win32::PathUtil::GetParent(*const_cast<Win32::Profile&>(profile)), resolvedPath);
    }
}
