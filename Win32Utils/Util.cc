#include "Util.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <shellapi.h>

namespace Win32 {
#undef GetEnvironmentVariable
    std::optional<std::wstring> Util::GetEnvironmentVariable(const std::wstring& variableName) {
        DWORD bufferSize = ::GetEnvironmentVariableW(variableName.data(), nullptr, 0);
        if (bufferSize == 0) {
            return std::nullopt;
        }
        std::wstring buffer(bufferSize, 0);
        DWORD resultSize = ::GetEnvironmentVariableW(variableName.data(), buffer.data(), bufferSize);
        if (resultSize == 0 || resultSize > bufferSize) {
            return std::nullopt;
        }
        buffer.resize(resultSize);
        return buffer;
    }

#undef GetFullPathName
    std::optional<std::wstring> Util::GetFullPathName(const std::wstring& path) {
        DWORD bufferSize = ::GetFullPathNameW(path.data(), 0, nullptr, nullptr);
        if (bufferSize == 0) {
            return std::nullopt;
        }
        std::wstring buffer(bufferSize, 0);
        if (!::GetFullPathNameW(path.data(), bufferSize, buffer.data(), nullptr)) {
            return std::nullopt;
        }
        buffer.resize(buffer.size() - 1);
        return buffer;
    }

#undef PathCombine
    std::optional<std::wstring> Util::PathCombine(const std::wstring& pathIn, const std::wstring& pathMore) {
        wchar_t buffer[MAX_PATH];
        if (!::PathCombineW(buffer, pathIn.data(), pathMore.data())) {
            return std::nullopt;
        }
        return buffer;
    }

    bool Util::TraverseDirectory(const std::wstring& directory, const std::function<bool(const std::wstring&)>& handler) {
        std::optional<std::wstring> directoryFullPath = GetFullPathName(directory);
        if (!directoryFullPath) {
            return false;
        }
        std::optional<std::wstring> findPath = PathCombine(*directoryFullPath, L".\\*");
        if (!findPath) {
            return false;
        }
        WIN32_FIND_DATAW findData;
        HANDLE findHandle = ::FindFirstFileW(findPath->data(), &findData);
        if (findHandle == INVALID_HANDLE_VALUE) {
            return false;
        }
        bool result = false;
        do {
            if (std::wcscmp(findData.cFileName, L".") && std::wcscmp(findData.cFileName, L"..")) {
                std::optional<std::wstring> fileName = PathCombine(*directoryFullPath, findData.cFileName);
                if (!fileName) {
                    continue;
                }
                if (handler(*fileName)) {
                    result = true;
                    break;
                }
            }
        } while (::FindNextFileW(findHandle, &findData));
        ::FindClose(findHandle);
        return result;
    }

    bool Util::DirectoryIsEmpty(const std::wstring& path) {
        bool isEmpty = true;
        TraverseDirectory(path, [&isEmpty](const std::wstring&) {
            isEmpty = false;
            return true;
        });
        return isEmpty;
    }

#undef RemoveDirectory
    bool Util::RemoveDirectory(const std::wstring& path) {
        std::optional<std::wstring> fullPath = GetFullPathName(path);
        if (!fullPath) {
            return false;
        }
        fullPath->resize(fullPath->size() + 1);
        SHFILEOPSTRUCTW fileOp = {
            .wFunc = FO_DELETE,
            .pFrom = fullPath->data(),
            .fFlags = FOF_ALLOWUNDO
        };
        return !::SHFileOperationW(&fileOp);
    }

    bool Util::MoveDirectory(const std::wstring& sourcePath, const std::wstring& destinationPath) {
        std::optional<std::wstring> sourceFullPath = GetFullPathName(sourcePath);
        if (!sourceFullPath) {
            return false;
        }
        std::optional<std::wstring> destinationFullPath = GetFullPathName(destinationPath);
        if (!destinationFullPath) {
            return false;
        }
        sourceFullPath->resize(sourceFullPath->size() + 1);
        destinationFullPath->resize(destinationFullPath->size() + 1);
        SHFILEOPSTRUCTW fileOp = {
            .wFunc = FO_MOVE,
            .pFrom = sourceFullPath->data(),
            .pTo = destinationFullPath->data(),
            .fFlags = FOF_NOCONFIRMMKDIR
        };
        return !::SHFileOperationW(&fileOp);
    }

    std::optional<std::wstring> Util::ReadUTF16LETextFileAll(const std::wstring& fileName) {
        HANDLE hFile = ::CreateFileW(fileName.data(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (hFile == INVALID_HANDLE_VALUE) {
            return std::nullopt;
        }
        DWORD readSize = 0;
        DWORD magic = 0;
        std::optional<std::wstring> result = std::nullopt;
        if (::ReadFile(hFile, &magic, 2, &readSize, nullptr) && readSize == 2 && magic == 0xFEFF) {
            DWORD fileSize = ::GetFileSize(hFile, nullptr);
            if (fileSize != INVALID_FILE_SIZE) {
                fileSize -= 2;
                std::wstring buffer(fileSize / sizeof(wchar_t), 0);
                if (::ReadFile(hFile, buffer.data(), fileSize, &readSize, nullptr) && fileSize == readSize) {
                    result = buffer;
                }
            }
        }
        ::CloseHandle(hFile);
        return result;
    }

#undef PathFileExists
    bool Util::PathFileExists(const std::wstring& path) {
        return ::PathFileExistsW(path.data());
    }

    std::wstring Util::PathGetParent(const std::wstring& path) {
        std::wstring buffer(path);
        ::PathRemoveFileSpecW(buffer.data());
        buffer.resize(buffer.find((wchar_t) 0));
        return buffer;
    }

#undef PathIsDirectory
    bool Util::PathIsDirectory(const std::wstring& path) {
        return ::PathIsDirectoryW(path.data());
    }

#undef CreateDirectory
    bool Util::CreateDirectory(const std::wstring& path) {
        return ::CreateDirectoryW(path.data(), nullptr);
    }
}
