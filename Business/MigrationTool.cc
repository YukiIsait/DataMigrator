#include <stdexcept>
#include "MigrationTool.h"
#include "MigrationInfo.h"
#include "Common/ProcessUtil.h"
#include "Common/FileUtil.h"
#include "Common/JunctionPointUtil.h"
#include "Common/Profile.h"

namespace Business {
    static std::wstring ResolvePath(const std::wstring& path) {
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
        return resolvedPath;
    }

    static void Migrate(MigrationInfo& migrationInfo) {
        // 保证映射路径不是一个文件
        if ((Win32::FileUtil::Exists(migrationInfo.mappingDirectory) && !Win32::FileUtil::IsDirectory(migrationInfo.mappingDirectory))) {
            throw std::runtime_error("Mapping path exists but is not a directory.");
        }
        // 保证存储路径不是一个文件
        if (Win32::FileUtil::Exists(migrationInfo.storageDirectory) && !Win32::FileUtil::IsDirectory(migrationInfo.storageDirectory)) {
            throw std::runtime_error("Storage path exists but is not a directory.");
        }
        switch (migrationInfo.operation) {
            case MigrationInfo::Operation::Move: // 迁移后映射
                // 已经有过迁移则直接改为映射操作
                if (Win32::JunctionPointUtil::IsJunctionPoint(migrationInfo.mappingDirectory)) {
                    migrationInfo.operation = MigrationInfo::Operation::Map;
                    break;
                }
                // 保证存储目录不存在
                if (Win32::FileUtil::Exists(migrationInfo.storageDirectory)) {
                    Win32::FileUtil::RemoveDirectoryByShell(migrationInfo.storageDirectory, false, true);
                }
                // 映射目录存在则将其迁移到存储目录，不存在则创建一个存储目录
                if (Win32::FileUtil::Exists(migrationInfo.mappingDirectory)) {
                    Win32::FileUtil::MoveDirectoryByShell(migrationInfo.mappingDirectory, migrationInfo.storageDirectory, true);
                } else {
                    Win32::FileUtil::CreateDirectoryTree(migrationInfo.storageDirectory);
                }
                // 迁移完成，改为映射
                migrationInfo.operation = MigrationInfo::Operation::Map;
                break;
            case MigrationInfo::Operation::Map: // 仅清空映射目录
                // 保证映射目录不存在
                if (Win32::FileUtil::Exists(migrationInfo.mappingDirectory)) {
                    Win32::FileUtil::RemoveDirectoryByShell(migrationInfo.mappingDirectory, false, true);
                }
                // 保证存储目录存在
                if (!Win32::FileUtil::Exists(migrationInfo.storageDirectory)) {
                    Win32::FileUtil::CreateDirectoryTree(migrationInfo.storageDirectory);
                }
                break;
            case MigrationInfo::Operation::Erase: // 全部清空
                // 保证映射目录不存在
                if (Win32::FileUtil::Exists(migrationInfo.mappingDirectory)) {
                    Win32::FileUtil::RemoveDirectoryByShell(migrationInfo.mappingDirectory, false, true);
                }
                // 保证存储目录存在并为空
                if (Win32::FileUtil::Exists(migrationInfo.storageDirectory)) {
                    Win32::FileUtil::RemoveDirectoryByShell(migrationInfo.storageDirectory, false, true);
                }
                Win32::FileUtil::CreateDirectoryTree(migrationInfo.storageDirectory);
                break;
            default:
                throw std::runtime_error("Invalid migration operation.");
        }
        // 创建映射目录并进行映射
        Win32::JunctionPointUtil::Create(migrationInfo.mappingDirectory, migrationInfo.storageDirectory);
    }

    void MigrationTool::Migrate(const Win32::Profile& profile) {
        std::vector<std::wstring> sections = profile.ReadSections();
        for (const std::wstring& section : sections) {
            MigrationInfo migrationInfo(profile, section);
            Migrate(migrationInfo);
            migrationInfo.WriteToProfile(profile, section);
        }
    }
}
