#pragma once
#include <string>
#include <functional>

#undef CopyFile
#undef CreateDirectory
#undef RemoveDirectory

namespace Win32 {
    namespace FileUtil {
        void CopyFile(const std::wstring& sourceFileName, const std::wstring& destinationFileName, bool failIfExists = true);
        bool Exists(const std::wstring& path);
        bool IsDirectory(const std::wstring& directoryName);
        void CreateDirectory(const std::wstring& directoryName);
        void CreateDirectoryTree(const std::wstring& directoryName);
        void RemoveDirectory(const std::wstring& directoryName);
        void RemoveDirectoryByShell(const std::wstring& directoryName, bool sendToRecycleBin = false, bool showUi = false);
        void MoveDirectoryByShell(const std::wstring& sourceDirectoryName, const std::wstring& destinationDirectoryName, bool showUi = false);
        void TraverseDirectory(const std::wstring& directoryName, std::function<bool(const std::wstring& fileName)> handler);
        bool DirectoryIsEmpty(const std::wstring& directoryName);
        std::wstring ReadUtf16LeTextFileAll(const std::wstring& fileName);
    }
}
