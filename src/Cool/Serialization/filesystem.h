#pragma once
#include <filesystem>

namespace CoolInternal {

template<typename T>
concept filesystem_path = std::is_same_v<T, std::filesystem::path>; // If we don't use that concept, std::string implicitly converts to std::filesystem::path
                                                                    // and finds these serialization functions.
                                                                    // ser20 then complains that std::string has two different serialization functions.
                                                                    // This concept prevents implicit conversions.
}

namespace ser20 {

template<class Archive, CoolInternal::filesystem_path Path>
inline std::string save_minimal(
    Archive const&,
    Path const& path
)
{
    return path.string();
}

template<class Archive, CoolInternal::filesystem_path Path>
inline void load_minimal(
    Archive const&,
    Path&              path,
    std::string const& path_as_str
)
{
    path = std::filesystem::path{path_as_str};
}

} // namespace ser20