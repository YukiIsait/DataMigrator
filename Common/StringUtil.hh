#pragma once
#include <optional>
#include <string>

namespace Common {
    namespace String {
        template <typename T>
        static void Trim(T& str) {
            if (str.empty()) {
                return;
            }
            str.erase(0, str.find_first_not_of(L"\x20\x0D\x0A"));
            str.erase(str.find_last_not_of(L"\x20\x0D\x0A") + 1);
        }

        template <typename T>
        static T Trim(const T& str) {
            T copy(str);
            Trim(copy);
            return copy;
        }

        template <typename ST, typename DT>
        static std::optional<ST> SplitAndGet(const ST& str, const DT& delimiter, size_t index) {
            size_t start = 0;
            size_t end = str.find(delimiter);
            for (size_t i = 0; i < index; i++) {
                if (end == std::wstring::npos) {
                    return std::nullopt;
                }
                start = end + 1;
                end = str.find(delimiter, start);
            }
            return str.substr(start, end - start);
        }
    }
}
