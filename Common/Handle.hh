#pragma once
#include <memory>

namespace Win32 {
    namespace HandleImplementation {
        void Free(void* handle);
    }

    template <typename HandleDeleterType = decltype(&HandleImplementation::Free)>
    class Handle {
    protected:
        std::unique_ptr<void, HandleDeleterType> handle;

    public:
        Handle(void* handle) noexcept: handle(handle, &HandleImplementation::Free) {};
        Handle(void* handle, const HandleDeleterType& deleter) noexcept: handle(handle, deleter) {};
        Handle(void* handle, HandleDeleterType&& deleter) noexcept: handle(handle, std::move(deleter)) {};

        operator void*() const noexcept {
            return handle.get();
        }
    };
}
