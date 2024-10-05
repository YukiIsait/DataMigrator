#pragma once
#include <string>

namespace Win32 {
    namespace PathUtil {
        std::wstring GetParent(const std::wstring& path);
        std::wstring GetFullPath(const std::wstring& path);
        size_t FindExtension(const std::wstring& path);
        std::wstring Combine(const std::wstring& pathIn, const std::wstring& pathMore);
        std::wstring GetProgramFileName();
        std::wstring ReplaceExtension(const std::wstring& path, const std::wstring& ext);
    }
}
