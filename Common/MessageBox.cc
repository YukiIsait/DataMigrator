#include <Windows.h>
#include "MessageBox.h"
#include "Exception.h"

namespace Win32 {
    namespace MessageBox {
        ReturnCode Show(void* wnd, const std::wstring& message, const std::wstring& title, uint32_t type) {
            int result = MessageBoxW(reinterpret_cast<HWND>(wnd), message.data(), title.data(), static_cast<UINT>(type));
            Exception::Assert(result != 0);
            return static_cast<ReturnCode>(result);
        }

        ReturnCode Show(void* wnd, const std::string& message, const std::string& title, uint32_t type) {
            int result = MessageBoxA(reinterpret_cast<HWND>(wnd), message.data(), title.data(), static_cast<UINT>(type));
            Exception::Assert(result != 0);
            return static_cast<ReturnCode>(result);
        }

        ReturnCode Show(const std::wstring& message, const std::wstring& title, uint32_t type) {
            return Show(nullptr, message, title, type);
        }

        ReturnCode Show(const std::string& message, const std::string& title, uint32_t type) {
            return Show(nullptr, message, title, type);
        }
    }
}
