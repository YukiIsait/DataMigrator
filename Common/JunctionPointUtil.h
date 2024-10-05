#pragma once
#include <string>

namespace Win32 {
    namespace JunctionPointUtil {
        void Mount(const std::wstring& junctionPoint, const std::wstring& targetDir);
        void Unmount(const std::wstring& junctionPoint);
        void Create(const std::wstring& junctionPoint, const std::wstring& targetDir);
        void Delete(const std::wstring& junctionPoint);
        bool IsJunctionPoint(const std::wstring& path);
    }
}
