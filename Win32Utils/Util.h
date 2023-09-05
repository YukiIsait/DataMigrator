#pragma once
#include <functional>
#include <optional>
#include <string>

namespace Win32 {
    class Util {
    public:
        static std::optional<std::wstring> GetEnvironmentVariable(const std::wstring& variableName);
        static std::optional<std::wstring> GetFullPathName(const std::wstring& path);
        static std::optional<std::wstring> PathCombine(const std::wstring& pathIn, const std::wstring& pathMore);
        static bool TraverseDirectory(const std::wstring& directory, const std::function<bool(const std::wstring&)>& handler);
        static bool DirectoryIsEmpty(const std::wstring& path);
        static bool RemoveDirectory(const std::wstring& path);
        static bool MoveDirectory(const std::wstring& sourcePath, const std::wstring& destinationPath);
        static std::optional<std::wstring> ReadUTF16LETextFileAll(const std::wstring& fileName);
        static std::wstring PathGetParent(const std::wstring& path);
        static bool PathFileExists(const std::wstring& path);
        static bool PathIsDirectory(const std::wstring& path);
        static bool CreateDirectory(const std::wstring& path);
    };
}
