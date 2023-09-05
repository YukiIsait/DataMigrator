#include "EntryPoint.h"
#include <Windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance,
                    _In_opt_ HINSTANCE hPrevInstance,
                    _In_ PWSTR pCmdLine,
                    _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);
    if (!pCmdLine || !*pCmdLine) {
        return Win32::EntryPoint::Main(0, nullptr);
    }
    int argc;
    LPWSTR* argv = ::CommandLineToArgvW(pCmdLine, &argc);
    if (!argv) {
        return Win32::EntryPoint::Main(0, nullptr);
    }
    int result = Win32::EntryPoint::Main(argc, argv);
    ::LocalFree(argv);
    return result;
}
