// #pragma once
// #include <map>
// #include <opencv2/videoio.hpp>
// #include "Cool/FileWatcher/FileWatcher.h"
// #include "Texture.h"

// namespace Cool {

// class TextureLibrary_VideoFile {
// public:
//     [[nodiscard]] auto get(std::filesystem::path const&) -> Texture const*;

//     // void clear() { _textures.clear(); }
//     /// Returns true iff at least one of the textures in the library has changed.
//     auto update() -> bool;
//     auto error_from(std::filesystem::path const&) -> std::optional<std::string>;

//     void imgui_debug_view() const;

// private:
//     void reload_file(std::filesystem::path const& path);

// private:
//     using clock = std::chrono::steady_clock;
//     struct Data {
//         std::optional<Texture>          texture{};
//         FileWatcher                     file_watcher{};
//         clock::time_point               date_of_last_request{};
//         std::optional<std::string>      error_message{};
//         std::optional<cv::VideoCapture> capture{};
//     };
//     std::map<std::filesystem::path, Data> _textures;
// };

// [[nodiscard]] inline auto texture_library_video_file() -> TextureLibrary_VideoFile&
// {
//     static auto instance = TextureLibrary_VideoFile{};
//     return instance;
// }

// } // namespace Cool