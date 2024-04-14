// #include "Cool/Gpu/TextureLibrary_VideoFile.h"
// #include <chrono>
// #include <filesystem>
// #include <opencv2/core/bindings_utils.hpp>
// #include <opencv2/core/utility.hpp>
// #include <opencv2/core/utils/logger.hpp>
// #include <opencv2/opencv.hpp>
// #include <opencv2/videoio.hpp>
// #include <optional>
// #include "Cool/DebugOptions/DebugOptions.h"
// #include "Cool/FileWatcher/FileWatcher.h"
// #include "Cool/Gpu/Texture.h"
// #include "Cool/ImGui/ImGuiExtras.h"
// #include "Cool/TextureSource/set_texture_from_opencv_image.h"
// #include "TextureLibrary_VideoFile.h"
// #include "fmt/chrono.h"
// #include "imgui.h"

// namespace Cool {

// static constexpr auto time_to_live = 5min;

// static auto time_to_live_has_expired(std::chrono::steady_clock::time_point date_of_last_request) -> bool
// {
//     return std::chrono::steady_clock::now() - date_of_last_request > time_to_live;
// }

// auto TextureLibrary_VideoFile::get(std::filesystem::path const& path) -> Texture const*
// {
//     auto const it = _textures.find(path);
//     // If this path is not known to the library, add it and try again.
//     if (it == _textures.end())
//     {
//         _textures[path] = {
//             .file_watcher         = FileWatcher{path, FileWatcher_NoCallbacks()},
//             .date_of_last_request = clock::now(),
//         };
//         reload_file(path);
//         return get(path);
//     }

//     // We have a new request
//     auto& data                = it->second;
//     data.date_of_last_request = clock::now();

//     auto const& maybe_capture = data.capture;
//     if (!maybe_capture && !data.error_message) // Capture is nullopt simply because it hasn't been used in a while. Reload the file.
//         reload_file(path);

//     if (!maybe_capture)
//         return nullptr;

//     cv::Mat wip_image{};
//     // if (get_clock() && std::abs(capture.get(cv::CAP_PROP_POS_MSEC) - get_clock()->time_in_seconds() * 1000.) > 1.)
//     //    capture.set(cv::CAP_PROP_POS_MSEC, get_clock()->time_in_seconds() * 1000.);
//     *data.capture >> wip_image;
//     set_texture_from_opencv_image(data.texture, wip_image);
//     if (DebugOptions::log_when_creating_textures())
//         Log::ToUser::info("TextureLibrary_FromFile", fmt::format("Generated texture from {}", path));
//     return &*data.texture;
// }

// void TextureLibrary_VideoFile::reload_file(std::filesystem::path const& path)
// {
//     auto& data         = _textures[path];
//     data.error_message = std::nullopt;
//     data.texture       = std::nullopt;
//     try
//     {
//         cv::redirectError([](int status, const char* func_name,
//                              const char* err_msg, const char* file_name,
//                              int line, void* userdata) {
//             Cool::Log::ToUser::warning("OpenCV", err_msg);
//             return 1;
//         });
//         cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_VERBOSE);
//         // cv::utils::logging::internal::writeLogMessage(LogLevel logLevel, const char *message)
//         data.capture = cv::VideoCapture{path.string()};
//         if (data.capture->isOpened())
//         {
//             data.capture->setExceptionMode(true);
//             // capture.set(cv::CAP_PROP_FRAME_HEIGHT, resolution.height);
//         }
//     }
//     catch (std::exception const& e)
//     {
//         data.error_message = e.what();
//     }
// }

// auto TextureLibrary_VideoFile::update() -> bool
// {
//     bool       has_changed = false;
//     auto const reload_tex  = [&](std::filesystem::path const& path) {
//         reload_file(path);
//         has_changed = true;
//     };
//     for (auto& kv : _textures)
//     {
//         kv.second.file_watcher.update({.on_file_changed = reload_tex, .on_path_invalid = reload_tex});
//         if (time_to_live_has_expired(kv.second.date_of_last_request))
//             kv.second.texture = std::nullopt;
//     }

//     return has_changed;
// }

// auto TextureLibrary_VideoFile::error_from(std::filesystem::path const& path) -> std::optional<std::string>
// {
//     return _textures[path].error_message;
// }

// void TextureLibrary_VideoFile::imgui_debug_view() const
// {
//     static constexpr ImGuiTableFlags flags = ImGuiTableFlags_SizingStretchSame
//                                              | ImGuiTableFlags_Resizable
//                                              | ImGuiTableFlags_BordersOuter
//                                              | ImGuiTableFlags_BordersV
//                                              | ImGuiTableFlags_BordersH
//                                              | ImGuiTableFlags_ContextMenuInBody;

//     if (ImGui::BeginTable("tex_library", 3, flags))
//     {
//         for (auto const& kv : _textures)
//         {
//             ImGui::TableNextRow();
//             {
//                 ImGui::TableSetColumnIndex(0);
//                 if (kv.second.texture)
//                     ImGuiExtras::image_framed(kv.second.texture->imgui_texture_id(), {100.f * kv.second.texture->aspect_ratio(), 100.f}, 2.f);
//                 else
//                     ImGui::TextUnformatted("INVALID");
//             }
//             {
//                 ImGui::TableSetColumnIndex(1);
//                 ImGui::TextUnformatted(kv.first.string().c_str());
//             }
//             {
//                 ImGui::TableSetColumnIndex(2);
//                 auto const time_since_last_use = std::chrono::steady_clock::now() - kv.second.date_of_last_request;
//                 if (time_since_last_use < time_to_live)
//                 {
//                     auto const duration = std::chrono::duration<float>{time_to_live - time_since_last_use};
//                     ImGuiExtras::time_formated_hms(duration.count());
//                 }
//                 else
//                 {
//                     ImGui::TextUnformatted(fmt::format("Expired (Not used for {})", time_to_live).c_str());
//                 }
//             }
//         }
//         ImGui::EndTable();
//     }
// }

// } // namespace Cool