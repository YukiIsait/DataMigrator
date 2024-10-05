#include <Windows.h>
#include <Shlwapi.h>
#include "PathUtil.h"
#include "Exception.h"

namespace Win32 {
    namespace PathUtil {
        std::wstring GetParent(const std::wstring& path) {
            std::wstring buffer(path);
            Exception::Assert(PathRemoveFileSpecW(buffer.data()), ERROR_PATH_NOT_FOUND);
            buffer.resize(buffer.find((wchar_t) 0));
            return buffer;
        }

        std::wstring GetFullPath(const std::wstring& path) {
            DWORD bufferSize = GetFullPathNameW(path.data(), 0, nullptr, nullptr);
            Exception::Assert(bufferSize != 0);
            std::wstring buffer(bufferSize, 0);
            bufferSize = GetFullPathNameW(path.data(), bufferSize, buffer.data(), nullptr);
            Exception::Assert(bufferSize != 0);
            buffer.resize(buffer.size() - 1);
            return buffer;
        }

        size_t FindExtension(const std::wstring& path) {
            LPWSTR extension = PathFindExtensionW(path.data());
            Exception::Assert(*extension, ERROR_PATH_NOT_FOUND);
            return extension - path.data();
        }

        std::wstring Combine(const std::wstring& pathIn, const std::wstring& pathMore) {
            std::wstring buffer(MAX_PATH, 0);
            Exception::Assert(PathCombineW(buffer.data(), pathIn.data(), pathMore.data()));
            buffer.resize(buffer.find((wchar_t) 0));
            return buffer;
        }

        std::wstring GetProgramFileName() {
            std::wstring buffer(MAX_PATH, 0);
            DWORD retSize = GetModuleFileNameW(nullptr, buffer.data(), MAX_PATH);
            Exception::Assert(retSize != 0 && retSize != MAX_PATH);
            buffer.resize(retSize);
            return buffer;
        }

        std::wstring ReplaceExtension(const std::wstring& path, const std::wstring& ext) {
            std::wstring buffer(MAX_PATH, 0);
            std::wmemcpy(buffer.data(), path.data(), path.size());
            Exception::Assert(PathRenameExtensionW(buffer.data(), ext.data()));
            return buffer;
        }
    }
}
