#include "MessageBox.h"
#include <Windows.h>

namespace Win32 {
#undef MessageBox
    MessageBox::ReturnCode MessageBox::Show(void* wnd, const std::wstring& message, const std::wstring& title, uint32_t type) {
        return static_cast<MessageBox::ReturnCode>(::MessageBoxW(reinterpret_cast<HWND>(wnd), message.data(), title.data(), static_cast<UINT>(type)));
    }

    MessageBox::ReturnCode MessageBox::Show(const std::wstring& message, const std::wstring& title, uint32_t type) {
        return Show(nullptr, message, title, type);
    }
}
