#include "MigrationTool.h"
#include "CommonUtils/StringUtil.hh"
#include "Win32Utils/JunctionPoint.h"
#include "Win32Utils/Util.h"
#include <optional>
#include <string>

namespace MigrationTool {
    std::optional<std::wstring> ResolvePath(const std::wstring& path) {
        size_t start = path.find(L'<');
        if (start == std::wstring::npos) {
            return path;
        }
        size_t end = path.find(L'>', start + 1);
        if (end == std::wstring::npos) {
            return std::nullopt;
        }
        std::wstring variableName = path.substr(start + 1, end - start - 1);
        std::optional<std::wstring> variableValue = Win32::Util::GetEnvironmentVariable(variableName);
        if (!variableValue) {
            return std::nullopt;
        }
        return path.substr(0, start) + *variableValue + path.substr(end + 1);
    }

    std::optional<std::wstring> GetMigrationPath(const std::wstring& currentPath, const std::wstring& migrationInfoLine,
                                                 MigrationInfoEnum migrationInfoEnum) {
        std::optional<std::wstring> path;
        path = StringUtil::SplitAndGet(migrationInfoLine, L'|', static_cast<size_t>(migrationInfoEnum));
        if (!path) {
            return std::nullopt;
        }
        StringUtil::Trim(*path);
        path = ResolvePath(*path);
        if (!path) {
            return std::nullopt;
        }
        return Win32::Util::PathCombine(currentPath, *path);
    }

    std::optional<KeepingRule> GetMigrationKeepingRule(const std::wstring& migrationInfoLine, MigrationInfoEnum migrationInfoEnum) {
        std::optional<std::wstring> keepingRule = StringUtil::SplitAndGet(migrationInfoLine, L'|', static_cast<size_t>(migrationInfoEnum));
        if (!keepingRule) {
            return std::nullopt;
        }
        StringUtil::Trim(*keepingRule);
        if (keepingRule == L"KeepDestination") {
            return KeepingRule::KeepDestination;
        }
        if (keepingRule == L"KeepSource") {
            return KeepingRule::KeepSource;
        }
        if (keepingRule == L"KeepNothing") {
            return KeepingRule::KeepNothing;
        }
        return std::nullopt;
    }

    std::optional<MigrationInfo> GetMigrationInfo(const std::wstring& currentPath, const std::wstring& migrationInfoLine) {
        std::optional<std::wstring> destination = GetMigrationPath(currentPath, migrationInfoLine, MigrationInfoEnum::Destination);
        if (!destination) {
            return std::nullopt;
        }
        std::optional<std::wstring> source = GetMigrationPath(currentPath, migrationInfoLine, MigrationInfoEnum::Source);
        if (!source) {
            return std::nullopt;
        }
        std::optional<KeepingRule> keepingRule = GetMigrationKeepingRule(migrationInfoLine, MigrationInfoEnum::KeepingRule);
        if (!keepingRule) {
            return std::nullopt;
        }
        return MigrationInfo { std::move(*destination), std::move(*source), *keepingRule };
    }

    bool Migrate(const MigrationInfo& migrationInfo) {
        if ((Win32::Util::PathFileExists(migrationInfo.destination) && !Win32::Util::PathIsDirectory(migrationInfo.destination)) ||
            (Win32::Util::PathFileExists(migrationInfo.source) && !Win32::Util::PathIsDirectory(migrationInfo.source))) {
            return false;
        }
        switch (migrationInfo.keepingRule) {
            case KeepingRule::KeepDestination:
                Win32::Util::RemoveDirectory(migrationInfo.source);
                if (!Win32::Util::PathFileExists(migrationInfo.destination)) {
                    Win32::Util::CreateDirectory(migrationInfo.destination);
                }
                break;
            case KeepingRule::KeepSource:
                if (Win32::JunctionPoint::IsJunctionPoint(migrationInfo.source)) {
                    return false;
                }
                Win32::Util::RemoveDirectory(migrationInfo.destination);
                if (Win32::Util::PathFileExists(migrationInfo.source)) {
                    Win32::Util::MoveDirectory(migrationInfo.source, migrationInfo.destination);
                } else {
                    Win32::Util::CreateDirectory(migrationInfo.destination);
                }
                break;
            case KeepingRule::KeepNothing:
                Win32::Util::RemoveDirectory(migrationInfo.destination);
                Win32::Util::RemoveDirectory(migrationInfo.source);
                Win32::Util::CreateDirectory(migrationInfo.destination);
                break;
            default:
                return false;
        }
        return Win32::JunctionPoint::Create(migrationInfo.source, migrationInfo.destination);
    }

    bool MigrationTool::MigrateByListFile(const std::wstring& filePath) {
        std::optional<std::wstring> fileFullPath = Win32::Util::GetFullPathName(filePath);
        if (!fileFullPath) {
            return false;
        }
        std::wstring currentPath = Win32::Util::PathGetParent(*fileFullPath);
        std::optional<std::wstring> content = Win32::Util::ReadUTF16LETextFileAll(*fileFullPath);
        if (!content) {
            return false;
        }
        bool result = true;
        for (size_t index = 0;; index++) {
            std::optional<std::wstring> line = StringUtil::SplitAndGet(*content, L"\r\n", index);
            if (!line) {
                break;
            }
            StringUtil::Trim(*line);
            if (line->empty() || (*line)[0] == L':') {
                continue;
            }
            std::optional<MigrationInfo> migrationInfo = GetMigrationInfo(currentPath, *line);
            if (!migrationInfo) {
                return false;
            }
            if (!Migrate(*migrationInfo)) {
                result = false;
            }
        }
        return result;
    }
}
