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

auto create_autosaver(std::function<void()> const& save) -> std::function<void()>
{
    // This function will be run every frame
    return [save]() {
        if (!user_settings().autosave_enabled)
            return;

        static auto last_time = std::chrono::steady_clock::now();
        const auto  now       = std::chrono::steady_clock::now();
        if (now - last_time > std::chrono::duration<float>{user_settings().autosave_delay_in_seconds})
        {
            save();
            last_time = now;
            if (DebugOptions::log_when_autosaving())
                Log::ToUser::info("Autosave", "The application was just saved.");
        }
    };
}

void copy_default_user_data_ifn()
{
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