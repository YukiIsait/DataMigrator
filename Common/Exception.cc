#include <Windows.h>
#include <memory>
#include "Exception.h"

namespace Win32 {
    static std::unique_ptr<char, decltype(&LocalFree)> FormatErrorMessage(uint32_t errorCode) noexcept {
        LPSTR buffer = nullptr;
        uint32_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                       nullptr, errorCode, MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), (LPSTR) &buffer, 0, nullptr);
        if (size) {
            uint16_t* lineBreak = reinterpret_cast<uint16_t*>(buffer + size - 2);
            if (*lineBreak == 0x0A0D) {
                *lineBreak = 0;
            }
        } else {
            buffer = reinterpret_cast<LPSTR>(LocalAlloc(LMEM_FIXED, 26));
            if (buffer) {
                wsprintfA(buffer, "Unknown error 0x%08X.", errorCode);
            }
        }
        return { buffer, &LocalFree };
    }

    Exception::Exception(uint32_t errorCode) noexcept: std::runtime_error(FormatErrorMessage(errorCode).get()), errorCode(errorCode) {}

    uint32_t Exception::GetErrorCode() const noexcept {
        return errorCode;
    }

    void Exception::Throw() {
        throw Exception(GetLastError());
    }

    void Exception::Throw(uint32_t errorCode) {
        SetLastError(errorCode);
        throw Exception(errorCode);
    }

    void Exception::Assert(bool expression) {
        if (!expression) {
            Throw();
        }
    }

    void Exception::Assert(bool expression, uint32_t errorCodeToThrow) {
        if (!expression) {
            Throw(errorCodeToThrow);
        }
    }

    void Exception::AssertNoError(uint32_t errorCode) {
        uint32_t lastErrorCode = GetLastError();
        if (lastErrorCode == errorCode) {
            throw Exception(lastErrorCode);
        }
    }

    void Exception::AssertNoError(uint32_t errorCode, uint32_t errorCodeToThrow) {
        uint32_t lastErrorCode = GetLastError();
        if (lastErrorCode == errorCode) {
            Throw(errorCodeToThrow);
        }
    }
}
