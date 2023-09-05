#include "JunctionPoint.h"
#include <Windows.h>
#include <Shlwapi.h>
#include <memory>

namespace Win32 {
    struct ReparseDataBuffer {
        DWORD ReparseTag;
        WORD ReparseDataLength;
        WORD Reserved;
        struct {
            WORD SubstituteNameOffset;
            WORD SubstituteNameLength;
            WORD PrintNameOffset;
            WORD PrintNameLength;
            WCHAR PathBuffer[1];
        } MountPointReparseBuffer;
    };

    std::pair<std::unique_ptr<ReparseDataBuffer>, size_t> CreateMountPointReparseDataBuffer(const std::wstring& substituteName, const std::wstring& printName) {
        size_t substituteNameSize = (substituteName.size() + 1) * sizeof(wchar_t);
        size_t printNameSize = (printName.size() + 1) * sizeof(wchar_t);
        size_t reparseDataBufferSize = UFIELD_OFFSET(ReparseDataBuffer, MountPointReparseBuffer.PathBuffer) + substituteNameSize + printNameSize;
        std::unique_ptr<ReparseDataBuffer> reparseDataBuffer(reinterpret_cast<ReparseDataBuffer*>(new uint8_t[reparseDataBufferSize]));
        reparseDataBuffer->ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
        reparseDataBuffer->ReparseDataLength = static_cast<uint16_t>(reparseDataBufferSize - UFIELD_OFFSET(ReparseDataBuffer, MountPointReparseBuffer));
        reparseDataBuffer->Reserved = 0;
        reparseDataBuffer->MountPointReparseBuffer.SubstituteNameOffset = 0;
        reparseDataBuffer->MountPointReparseBuffer.SubstituteNameLength = static_cast<uint16_t>(substituteNameSize - sizeof(wchar_t));
        reparseDataBuffer->MountPointReparseBuffer.PrintNameOffset = static_cast<uint16_t>(substituteNameSize);
        reparseDataBuffer->MountPointReparseBuffer.PrintNameLength = static_cast<uint16_t>(printNameSize - sizeof(wchar_t));
        std::memcpy(reparseDataBuffer->MountPointReparseBuffer.PathBuffer, substituteName.data(), substituteNameSize);
        std::memcpy(reinterpret_cast<uint8_t*>(reparseDataBuffer->MountPointReparseBuffer.PathBuffer) + substituteNameSize, printName.data(), printNameSize);
        return std::make_pair(std::move(reparseDataBuffer), reparseDataBufferSize);
    }

    bool JunctionPoint::Mount(const std::wstring& junctionPoint, const std::wstring& targetDir) {
        DWORD targetDirFullPathSize = ::GetFullPathNameW(targetDir.data(), 0, nullptr, nullptr);
        if (targetDirFullPathSize == 0) {
            return false;
        }
        std::wstring targetDirNtPath(targetDirFullPathSize + 4, 0);
        std::wmemcpy(targetDirNtPath.data(), L"\\??\\", 4);
        if (!::GetFullPathNameW(targetDir.data(), targetDirFullPathSize, targetDirNtPath.data() + 4, nullptr)) {
            return false;
        }
        targetDirNtPath.resize(targetDirNtPath.size() - 1);
        HANDLE reparsePoint = ::CreateFileW(junctionPoint.data(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
                                            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
        if (reparsePoint == INVALID_HANDLE_VALUE) {
            return false;
        }
        std::pair<std::unique_ptr<ReparseDataBuffer>, size_t> reparseData = CreateMountPointReparseDataBuffer(targetDirNtPath, L"");
        DWORD bytesReturned;
        BOOL ret = ::DeviceIoControl(reparsePoint, FSCTL_SET_REPARSE_POINT, reparseData.first.get(),
                                     static_cast<DWORD>(reparseData.second), nullptr, 0, &bytesReturned, nullptr);
        ::CloseHandle(reparsePoint);
        return ret;
    }

    bool JunctionPoint::Create(const std::wstring& junctionPoint, const std::wstring& targetDir) {
        if (!::CreateDirectoryW(junctionPoint.data(), nullptr)) {
            return false;
        }
        if (!Mount(junctionPoint, targetDir)) {
            ::RemoveDirectoryW(junctionPoint.data());
            return false;
        }
        return true;
    }

    bool JunctionPoint::Unmount(const std::wstring& junctionPoint) {
        ReparseDataBuffer reparseDataBuffer = { .ReparseTag = IO_REPARSE_TAG_MOUNT_POINT };
        HANDLE reparsePoint = ::CreateFileW(junctionPoint.data(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING,
                                            FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
        if (reparsePoint == INVALID_HANDLE_VALUE) {
            return false;
        }
        DWORD bytesReturned;
        BOOL ret = ::DeviceIoControl(reparsePoint, FSCTL_DELETE_REPARSE_POINT, &reparseDataBuffer,
                                     UFIELD_OFFSET(ReparseDataBuffer, MountPointReparseBuffer), nullptr, 0, &bytesReturned, nullptr);
        ::CloseHandle(reparsePoint);
        return ret;
    }

    bool JunctionPoint::Delete(const std::wstring& junctionPoint) {
        if (!Unmount(junctionPoint)) {
            return false;
        }
        return ::RemoveDirectoryW(junctionPoint.data());
    }

    bool JunctionPoint::IsJunctionPoint(uint32_t attributes) {
        if (attributes == INVALID_FILE_ATTRIBUTES ||
            (attributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)) != (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)) {
            return false;
        }
        return true;
    }

    bool JunctionPoint::IsJunctionPoint(const std::wstring& path) {
        return IsJunctionPoint(::GetFileAttributesW(path.data()));
    }
}
