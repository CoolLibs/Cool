#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Icons/Icons.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Serialization/Serialization.h>
#include <Cool/UserSettings/UserSettings.h>
#include <exception>
#include <filesystem>
#include <fstream>
#include "Audio/Audio.hpp"
#include "Cool/File/File.h"
#include "Cool/Path/Path.h"
#include "Cool/TextureSource/TextureLibrary_Image.h"
#include "Cool/TextureSource/TextureLibrary_Webcam.hpp"
#include "Cool/TextureSource/TextureSamplerLibrary.h"
#include "Cool/View/ViewsManager.h"

namespace Cool::internal {

void shut_down()
{
#if defined(COOL_VULKAN)
    vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
    Audio::shut_down();
    TextureLibrary_Webcam::instance().shut_down(); // We must destroy the textures in the WebcamImages before the texture_pool() gets destroyed
}

auto create_autosaver(Cool::AutoSerializer const& auto_serializer) -> std::function<void()>
{
    return [&auto_serializer]() {
        if (!user_settings().autosave_enabled)
            return;

        static auto last_time = std::chrono::steady_clock::now();
        const auto  now       = std::chrono::steady_clock::now();
        if (now - last_time > std::chrono::duration<float>{user_settings().autosave_delay_in_seconds})
        {
            auto_serializer.save();
            last_time = now;
            if (DebugOptions::log_when_autosaving())
            {
                Log::ToUser::info("Autosave", "The application was just saved.");
            }
        }
    };
}

static void erase_imgui_ini_if_old_version(int imgui_ini_version)
{
    try
    {
        auto const version_path = Cool::Path::user_data() / "imgui_ini_version.txt";
        auto const ini_path     = Cool::Path::user_data() / "imgui.ini";

        {
            auto input_file = std::ifstream{version_path};
            if (!input_file.is_open())
            {
                Cool::File::remove_file(ini_path);
            }
            else
            {
                int user_version{-1};
                input_file >> user_version;
                if (user_version < imgui_ini_version)
                    Cool::File::remove_file(ini_path);
            }
        }

        Cool::File::remove_file(version_path);
        auto output_file = std::ofstream{version_path};
        output_file << imgui_ini_version;
    }
    catch (...)
    {
    }
}

void copy_default_user_data_ifn(int imgui_ini_version)
{
    erase_imgui_ini_if_old_version(imgui_ini_version); // When new imgui windows are added, we need to erase the user's imgui.ini so that they will get the new one that is in user_data_default().
    try
    {
        for (auto const& entry : std::filesystem::recursive_directory_iterator(Cool::Path::default_user_data()))
        {
            auto const& default_path = entry.path();
            if (!Cool::File::is_regular_file(default_path))
                continue;

            auto const path = Cool::Path::user_data() / Cool::File::relative(default_path, Cool::Path::default_user_data());
            if (Cool::File::exists(path))
                continue;

            try
            {
                Cool::File::copy_file(default_path, path);
            }
            catch (std::exception const& e)
            {
                Cool::Log::ToUser::warning("Default user data", fmt::format("Failed to copy {} to {}:\n{}", Cool::File::weakly_canonical(default_path), Cool::File::weakly_canonical(path), e.what()));
            }
        }
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::warning("Default user data", fmt::format("Failed to copy default user data:\n{}", e.what()));
    }
}

} // namespace Cool::internal