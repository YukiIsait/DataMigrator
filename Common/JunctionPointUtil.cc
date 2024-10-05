#include <Windows.h>
#include "JunctionPointUtil.h"
#include "Exception.h"
#include "Handle.hh"
#include "PathUtil.h"
#include "FileUtil.h"

namespace Win32 {
    namespace JunctionPointUtil {
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

        static std::pair<std::unique_ptr<ReparseDataBuffer>, size_t> CreateMountPointReparseDataBuffer(const std::wstring& substituteName, const std::wstring& printName) {
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

        void Mount(const std::wstring& junctionPoint, const std::wstring& targetDir) {
            std::wstring targetDirNtPath = PathUtil::GetFullPath(targetDir);
            targetDirNtPath.insert(0, L"\\??\\", 4);
            Handle reparsePoint = CreateFileW(junctionPoint.data(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
            Exception::Assert(reparsePoint != INVALID_HANDLE_VALUE);
            std::pair<std::unique_ptr<ReparseDataBuffer>, size_t> reparseData = CreateMountPointReparseDataBuffer(targetDirNtPath, L"");
            DWORD bytesReturned;
            BOOL result = DeviceIoControl(reparsePoint, FSCTL_SET_REPARSE_POINT, reparseData.first.get(), static_cast<DWORD>(reparseData.second), nullptr, 0, &bytesReturned, nullptr);
            Exception::Assert(result);
        }

        void Unmount(const std::wstring& junctionPoint) {
            ReparseDataBuffer reparseDataBuffer = { 0 };
            reparseDataBuffer.ReparseTag = IO_REPARSE_TAG_MOUNT_POINT;
            Handle reparsePoint = CreateFileW(junctionPoint.data(), GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OPEN_REPARSE_POINT, nullptr);
            Exception::Assert(reparsePoint != INVALID_HANDLE_VALUE);
            DWORD bytesReturned;
            BOOL result = DeviceIoControl(reparsePoint, FSCTL_DELETE_REPARSE_POINT, &reparseDataBuffer, UFIELD_OFFSET(ReparseDataBuffer, MountPointReparseBuffer), nullptr, 0, &bytesReturned, nullptr);
            Exception::Assert(result);
        }

        void Create(const std::wstring& junctionPoint, const std::wstring& targetDir) {
            FileUtil::CreateDirectory(junctionPoint);
            try {
                Mount(junctionPoint, targetDir);
            } catch (...) {
                FileUtil::RemoveDirectory(junctionPoint);
                throw;
            }
        }

        void Delete(const std::wstring& junctionPoint) {
            Unmount(junctionPoint);
            FileUtil::RemoveDirectory(junctionPoint);
        }

        bool IsJunctionPoint(const std::wstring& path) {
            DWORD attributes = GetFileAttributesW(path.data());
            Exception::Assert(attributes != INVALID_FILE_ATTRIBUTES);
            return !((attributes & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)) != (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT));
        }
    }
}
