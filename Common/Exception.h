#pragma once
#include <stdexcept>
#include <cstdint>

namespace Win32 {
    class Exception: public std::runtime_error {
    private:
        uint32_t errorCode;

    public:
        explicit Exception(uint32_t errorCode) noexcept;
        uint32_t GetErrorCode() const noexcept;

        static void Throw();
        static void Throw(uint32_t errorCode);
        static void Assert(bool expression);
        static void Assert(bool expression, uint32_t errorCodeToThrow);
        static void AssertNoError(uint32_t errorCode);
        static void AssertNoError(uint32_t errorCode, uint32_t errorCodeToThrow);
    };
}
