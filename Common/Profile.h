#pragma once
#include <vector>
#include <string>
#include <optional>

namespace Win32 {
    class Profile {
    private:
        static std::wstring InternalReadString(const std::wstring& fileName, std::optional<std::reference_wrapper<const std::wstring>> appName = std::nullopt, std::optional<std::reference_wrapper<const std::wstring>> keyName = std::nullopt);
        static void InternalWriteString(const std::wstring& fileName, const std::wstring& appName, std::optional<std::reference_wrapper<const std::wstring>> keyName = std::nullopt, std::optional<std::reference_wrapper<const std::wstring>> value = std::nullopt);
        static std::vector<std::wstring> InternalReadNames(const std::wstring& fileName, std::optional<std::reference_wrapper<const std::wstring>> appName = std::nullopt);

    public:
        static std::vector<std::wstring> ReadSections(const std::wstring& fileName);
        static std::vector<std::wstring> ReadKeys(const std::wstring& fileName, const std::wstring& appName);
        static std::wstring ReadString(const std::wstring& fileName, const std::wstring& appName, const std::wstring& keyName);
        static void WriteString(const std::wstring& fileName, const std::wstring& appName, const std::wstring& keyName, const std::wstring& value);
        static void RemoveSection(const std::wstring& fileName, const std::wstring& appName);
        static void RemoveKey(const std::wstring& fileName, const std::wstring& appName, const std::wstring& keyName);

    private:
        std::wstring fileName;

    public:
        Profile() noexcept = default;
        Profile(const Profile& other) noexcept = default;
        Profile(const std::wstring& fileName) noexcept;
        Profile(std::wstring&& fileName) noexcept;
        Profile(Profile&& other) noexcept;

        Profile& operator=(const Profile& other) noexcept;
        Profile& operator=(Profile&& other) noexcept;

        std::wstring& operator*() noexcept;

        std::vector<std::wstring> ReadSections() const;
        std::vector<std::wstring> ReadKeys(const std::wstring& appName) const;
        std::wstring ReadString(const std::wstring& appName, const std::wstring& keyName) const;
        void WriteString(const std::wstring& appName, const std::wstring& keyName, const std::wstring& value) const;
        void RemoveSection(const std::wstring& appName) const;
        void RemoveKey(const std::wstring& appName, const std::wstring& keyName) const;
    };
}
