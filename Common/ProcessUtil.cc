#include <Windows.h>
#include "ProcessUtil.h"
#include "Exception.h"

namespace Win32 {
    namespace ProcessUtil {
        std::wstring GetEnvironment(const std::wstring& name) {
            DWORD bufferSize = GetEnvironmentVariableW(name.data(), nullptr, 0);
            Exception::Assert(bufferSize != 0);
            std::wstring buffer(bufferSize, 0);
            bufferSize = GetEnvironmentVariableW(name.data(), buffer.data(), static_cast<DWORD>(buffer.size()));
            Exception::Assert(bufferSize != 0);
            buffer.resize(buffer.size() - 1);
            return buffer;
        }

        void SetEnvironment(const std::wstring& name, const std::wstring& value) {
            Exception::Assert(SetEnvironmentVariableW(name.data(), value.data()));
        }

        void LaunchProcess(const std::wstring& path, const std::wstring& args) {
            STARTUPINFO si = { 0 };
            PROCESS_INFORMATION pi = { 0 };
            si.cb = sizeof(si);
            std::wstring command(path.size() + args.size() + 1, 0);
            std::wstring::iterator commandIterator = std::copy(path.begin(), path.end(), command.begin());
            *commandIterator++ = L' ';
            std::copy(args.begin(), args.end(), commandIterator);
            if (CreateProcessW(path.data(), command.data(), nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
                CloseHandle(pi.hThread);
                CloseHandle(pi.hProcess);
            } else {
                Exception::Throw();
            }
        }
    }
}
