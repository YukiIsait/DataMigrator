#pragma once
#include <string>

namespace Win32 {
    namespace ProcessUtil {
        std::wstring GetEnvironment(const std::wstring& name);
        void SetEnvironment(const std::wstring& name, const std::wstring& value);
        void LaunchProcess(const std::wstring& path, const std::wstring& args);
    }
}
