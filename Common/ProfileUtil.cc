#include <Windows.h>
#include "Profile.h"
#include "PathUtil.h"
#include "Exception.h"

namespace Win32 {
    std::wstring Profile::InternalReadString(const std::wstring& fileName, std::optional<std::reference_wrapper<const std::wstring>> appName, std::optional<std::reference_wrapper<const std::wstring>> keyName) {
        std::wstring fullFileName = PathUtil::GetFullPath(fileName);
        std::wstring buffer(0x1F, 0);
        for (uint32_t size = 0x20; size < 0x10000; size += 0x20) {
            buffer.resize(static_cast<size_t>(size) - 1);
            uint32_t result = GetPrivateProfileStringW(appName ? appName->get().data() : nullptr, keyName ? keyName->get().data() : nullptr, nullptr, buffer.data(), size, fullFileName.data());
            Exception::AssertNoError(ERROR_FILE_NOT_FOUND);
            Exception::Assert(!buffer.empty(), ERROR_BAD_ARGUMENTS);
            if (appName && keyName) {
                if (result == size - 1) {
                    continue;
                }
            } else {
                if (result == size - 2) {
                    continue;
                }
            }
            buffer.resize(result);
            break;
        }
        return buffer;
    }

    void Profile::InternalWriteString(const std::wstring& fileName, const std::wstring& appName, std::optional<std::reference_wrapper<const std::wstring>> keyName, std::optional<std::reference_wrapper<const std::wstring>> value) {
        Exception::Assert(WritePrivateProfileStringW(appName.data(), keyName ? keyName->get().data() : nullptr, value ? value->get().data() : nullptr, fileName.data()));
    }

    std::vector<std::wstring> Profile::InternalReadNames(const std::wstring& fileName, std::optional<std::reference_wrapper<const std::wstring>> appName) {
        std::wstring sections = InternalReadString(fileName, appName);
        std::vector<std::wstring> result;
        size_t size = 0;
        while (size < sections.size()) {
            wchar_t* data = sections.data() + size;
            size_t length = std::wcslen(data);
            size += length + 1;
            result.emplace_back(std::wstring(data, length));
        }
        return result;
    }

    std::vector<std::wstring> Profile::ReadSections(const std::wstring& fileName) {
        return InternalReadNames(fileName);
    }

    std::vector<std::wstring> Profile::ReadKeys(const std::wstring& fileName, const std::wstring& appName) {
        Exception::Assert(!appName.empty(), ERROR_BAD_ARGUMENTS);
        return InternalReadNames(fileName, appName);
    }

    std::wstring Profile::ReadString(const std::wstring& fileName, const std::wstring& appName, const std::wstring& keyName) {
        Exception::Assert(!(appName.empty() || keyName.empty() || fileName.empty()), ERROR_BAD_ARGUMENTS);
        return InternalReadString(fileName, appName, keyName);
    }

    void Profile::WriteString(const std::wstring& fileName, const std::wstring& appName, const std::wstring& keyName, const std::wstring& value) {
        InternalWriteString(fileName, appName, keyName, value);
    }

    void Profile::RemoveSection(const std::wstring& fileName, const std::wstring& appName) {
        InternalWriteString(fileName, appName);
    }

    void Profile::RemoveKey(const std::wstring& fileName, const std::wstring& appName, const std::wstring& keyName) {
        InternalWriteString(fileName, appName, keyName);
    }
}
