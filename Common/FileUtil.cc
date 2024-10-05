#include <Windows.h>
#include <Shlwapi.h>
#include <algorithm>
#include "FileUtil.h"
#include "PathUtil.h"
#include "Exception.h"
#include "Handle.hh"

namespace Win32 {
    namespace FileUtil {
        void CopyFile(const std::wstring& sourceFileName, const std::wstring& destinationFileName, bool failIfExists) {
            Exception::Assert(CopyFileW(sourceFileName.data(), destinationFileName.data(), failIfExists));
        }

        bool Exists(const std::wstring& path) {
            return PathFileExistsW(path.data());
        }

        bool IsDirectory(const std::wstring& directoryName) {
            return PathIsDirectoryW(directoryName.data());
        }

        void CreateDirectory(const std::wstring& directoryName) {
            Exception::Assert(CreateDirectoryW(directoryName.data(), nullptr));
        }

        void CreateDirectoryTree(const std::wstring& directoryName) {
            size_t position = 0;
            size_t start = 0;
            while ((position = directoryName.find(L'\\', start)) != std::wstring::npos) {
                std::wstring currentPath = directoryName.substr(0, position);
                if (!Exists(currentPath)) {
                    CreateDirectory(directoryName.substr(0, position));
                }
                start = position + 1;
            }
            if (!Exists(directoryName)) {
                CreateDirectory(directoryName);
            }
        }

        void RemoveDirectory(const std::wstring& directoryName) {
            Exception::Assert(RemoveDirectoryW(directoryName.data()));
        }

        void RemoveDirectoryByShell(const std::wstring& directoryName, bool sendToRecycleBin, bool showUi) {
            std::wstring fullPath = PathUtil::GetFullPath(directoryName);
            fullPath.append(1, 0);
            SHFILEOPSTRUCTW fileOp = { 0 };
            fileOp.wFunc = FO_DELETE;
            fileOp.pFrom = fullPath.data();
            fileOp.fFlags = (sendToRecycleBin ? FOF_ALLOWUNDO : 0) | (showUi ? 0 : FOF_NO_UI);
            Exception::Assert(SHFileOperationW(&fileOp) == 0 && !fileOp.fAnyOperationsAborted, ERROR_CURRENT_DIRECTORY);
        }

        void MoveDirectoryByShell(const std::wstring& sourceDirectoryName, const std::wstring& destinationDirectoryName, bool showUi) {
            std::wstring sourceFullPath = PathUtil::GetFullPath(sourceDirectoryName);
            sourceFullPath.append(1, 0);
            std::wstring destinationFullPath = PathUtil::GetFullPath(destinationDirectoryName);
            destinationFullPath.append(1, 0);
            SHFILEOPSTRUCTW fileOp = { 0 };
            fileOp.wFunc = FO_MOVE;
            fileOp.pFrom = sourceFullPath.data();
            fileOp.pTo = destinationFullPath.data();
            fileOp.fFlags = FOF_NOCONFIRMMKDIR | (showUi ? 0 : FOF_NO_UI);
            Exception::Assert(SHFileOperationW(&fileOp) == 0 && !fileOp.fAnyOperationsAborted, ERROR_CANNOT_MAKE);
        }

        void TraverseDirectory(const std::wstring& directoryName, std::function<bool(const std::wstring& fileName)> handler) {
            std::wstring findPath = PathUtil::Combine(directoryName, L".\\*");
            WIN32_FIND_DATAW findData = { 0 };
            Handle<decltype(&FindClose)> findHandle(FindFirstFileW(findPath.data(), &findData), &FindClose);
            Exception::Assert(findHandle != INVALID_HANDLE_VALUE);
            do {
                if (std::wcscmp(findData.cFileName, L".") && std::wcscmp(findData.cFileName, L"..")) {
                    std::wstring fileName = PathUtil::Combine(directoryName, findData.cFileName);
                    if (!handler(fileName)) {
                        break;
                    }
                }
            } while (FindNextFileW(findHandle, &findData));
        }

        bool DirectoryIsEmpty(const std::wstring& directoryName) {
            bool isEmpty = true;
            TraverseDirectory(directoryName, [&isEmpty](const std::wstring&) {
                isEmpty = false;
                return false;
            });
            return isEmpty;
        }

        std::wstring ReadUtf16LeTextFileAll(const std::wstring& fileName) {
            Handle file = CreateFileW(fileName.data(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            Exception::Assert(file != INVALID_HANDLE_VALUE);
            DWORD readSize = 0;
            DWORD magic = 0;
            Exception::Assert(ReadFile(file, &magic, 2, &readSize, nullptr) && readSize == 2 && magic == 0xFEFF, ERROR_TOO_MANY_OPEN_FILES);
            DWORD fileSize = GetFileSize(file, nullptr);
            Exception::Assert(fileSize != INVALID_FILE_SIZE);
            fileSize -= 2;
            std::wstring buffer(fileSize / sizeof(wchar_t), 0);
            Exception::Assert(ReadFile(file, buffer.data(), fileSize, &readSize, nullptr) && fileSize == readSize);
            return buffer;
        }
    }
}
