#pragma once
#include <string>

namespace MigrationTool {
    enum class MigrationInfoEnum {
        Destination,
        Source,
        KeepingRule
    };

    enum class KeepingRule {
        KeepDestination,
        KeepSource,
        KeepNothing
    };

    struct MigrationInfo {
        std::wstring destination;
        std::wstring source;
        KeepingRule keepingRule;
    };

    class MigrationTool {
    public:
        static bool MigrateByListFile(const std::wstring& filePath);
    };
}
