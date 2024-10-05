#include <Windows.h>
#include "EntryPoint.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance,
                      _In_ PWSTR pCmdLine,
                      _In_ int nCmdShow) {
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(nCmdShow);

    std::vector<std::wstring> args;
    if (pCmdLine && *pCmdLine) {
        int argc;
        LPWSTR* argv = CommandLineToArgvW(pCmdLine, &argc);
        if (argv) {
            args.reserve(argc);
            for (int i = 0; i < argc; ++i) {
                args.emplace_back(argv[i]);
            }
            LocalFree(argv);
        }
    }
    return Win32::EntryPoint::Main(args);
}
