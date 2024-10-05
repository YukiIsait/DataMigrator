#include "Profile.h"

namespace Win32 {
    Profile::Profile(const std::wstring& fileName) noexcept: fileName(fileName) {}

    Profile::Profile(std::wstring&& fileName) noexcept: fileName(std::move(fileName)) {}

    Profile::Profile(Profile&& other) noexcept: fileName(std::move(other.fileName)) {}

    Profile& Profile::operator=(const Profile& other) noexcept {
        if (this == &other) {
            return *this;
        }
        fileName = other.fileName;
        return *this;
    }

    Profile& Profile::operator=(Profile&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        fileName = std::move(other.fileName);
        return *this;
    }

    std::wstring& Profile::operator*() noexcept {
        return fileName;
    }

    std::vector<std::wstring> Profile::ReadSections() const {
        return Profile::ReadSections(fileName);
    }

    std::vector<std::wstring> Profile::ReadKeys(const std::wstring& appName) const {
        return Profile::ReadKeys(fileName, appName);
    }

    std::wstring Profile::ReadString(const std::wstring& appName, const std::wstring& keyName) const {
        return Profile::ReadString(fileName, appName, keyName);
    }

    void Profile::WriteString(const std::wstring& appName, const std::wstring& keyName, const std::wstring& value) const {
        Profile::WriteString(fileName, appName, keyName, value);
    }

    void Profile::RemoveSection(const std::wstring& appName) const {
        Profile::RemoveSection(fileName, appName);
    }

    void Profile::RemoveKey(const std::wstring& appName, const std::wstring& keyName) const {
        Profile::RemoveKey(fileName, appName, keyName);
    }
}
