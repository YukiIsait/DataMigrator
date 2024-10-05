#pragma once
#include <memory>

namespace Win32 {
    namespace HandleImplementation {
        void Free(void* handle);
    }

    template <typename HandleDeleterType = decltype(&HandleImplementation::Free)>
    class Handle: public std::unique_ptr<void, HandleDeleterType> {
    public:
        inline Handle(void* handle) noexcept: std::unique_ptr<void, HandleDeleterType>(handle, &HandleImplementation::Free) {};
        inline Handle(void* handle, const HandleDeleterType& deleter) noexcept: std::unique_ptr<void, HandleDeleterType>(handle, deleter) {};
        inline Handle(void* handle, HandleDeleterType&& deleter) noexcept: std::unique_ptr<void, HandleDeleterType>(handle, std::move(deleter)) {};

        operator void*() const noexcept {
            return this->get();
        }
    };
}
