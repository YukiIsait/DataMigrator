#pragma once
#include <optional>
#include <string>
#include <memory>

namespace Win32 {
    class JunctionPoint {
    public:
        static bool Mount(const std::wstring& junctionPoint, const std::wstring& targetDir);
        static bool Create(const std::wstring& junctionPoint, const std::wstring& targetDir);
        static bool Unmount(const std::wstring& junctionPoint);
        static bool Delete(const std::wstring& junctionPoint);
        static bool IsJunctionPoint(uint32_t attributes);
        static bool IsJunctionPoint(const std::wstring& path);
    };
}
