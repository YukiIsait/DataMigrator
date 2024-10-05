#pragma once
#include <string>

#undef MessageBox

namespace Win32 {
    namespace MessageBox {
        enum class ButtonType : uint32_t {
            Ok = 0x00000000L,
            OkCancel = 0x00000001L,
            AbortRetryIgnore = 0x00000002L,
            YesNoCancel = 0x00000003L,
            YesNo = 0x00000004L,
            RetryCancel = 0x00000005L,
            CancelTryContinue = 0x00000006L,
            Help = 0x00004000L
        };

        enum class DefaultButton : uint32_t {
            Button1 = 0x00000000L,
            Button2 = 0x00000100L,
            Button3 = 0x00000200L,
            Button4 = 0x00000300L
        };

        enum class IconType : uint32_t {
            Hand = 0x00000010L,
            Stop = 0x00000010L,
            Error = 0x00000010L,
            Question = 0x00000020L,
            Warning = 0x00000030L,
            Exclamation = 0x00000030L,
            Asterisk = 0x00000040L,
            Information = 0x00000040L
        };

        enum class DialogModality : uint32_t {
            Application = 0x00000000L,
            System = 0x00001000L,
            TaskModal = 0x00002000L
        };

        enum class OtherOption : uint32_t {
            DefaultDesktopOnly = 0x00020000L,
            Right = 0x00080000L,
            RtlReading = 0x00100000L,
            SetForeground = 0x00010000L,
            TopMost = 0x00040000L,
            ServiceNotification = 0x00200000L
        };

        enum class ReturnCode : int {
            Abort = 3,
            Cancel = 2,
            Continue = 11,
            Ignore = 5,
            No = 7,
            Ok = 1,
            Retry = 4,
            TryAgain = 10,
            Yes = 6
        };

        ReturnCode Show(void* wnd, const std::wstring& message, const std::wstring& title, uint32_t type);
        ReturnCode Show(void* wnd, const std::string& message, const std::string& title, uint32_t type);
        ReturnCode Show(const std::wstring& message, const std::wstring& title, uint32_t type);
        ReturnCode Show(const std::string& message, const std::string& title, uint32_t type);
    }
}
