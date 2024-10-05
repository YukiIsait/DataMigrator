#include "Handle.hh"
#include <Windows.h>

namespace Win32 {
    namespace HandleImplementation {
        void Free(void* handle) {
            CloseHandle(handle);
        }
    }
}
