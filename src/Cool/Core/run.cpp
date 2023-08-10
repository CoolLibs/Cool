#include <Cool/DebugOptions/DebugOptions.h>
#include <Cool/Gpu/FullscreenPipeline.h>
#include <Cool/Icons/Icons.h>
#include <Cool/Serialization/AutoSerializer.h>
#include <Cool/Serialization/Serialization.h>
#include <Cool/UserSettings/UserSettings.h>
#include <exception>
#include <filesystem>
#include <fstream>
#include "Cool/File/File.h"
#include "Cool/Gpu/TextureLibrary_FromFile.h"
#include "Cool/Gpu/TextureSamplerLibrary.h"
#include "Cool/Path/Path.h"
#include "Cool/View/ViewsManager.h"

namespace Cool::internal {

void shut_down()
{
#if defined(COOL_VULKAN)
    vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
    Icons::shut_down();
    TextureLibrary_FromFile::instance().clear();
    TextureSamplerLibrary::instance().clear();
    FullscreenPipeline::shut_down();
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
                std::filesystem::remove(ini_path);
            }
            else
            {
                int user_version{-1};
                input_file >> user_version;
                if (user_version < imgui_ini_version)
                    std::filesystem::remove(ini_path);
            }
        }

        std::filesystem::remove(version_path);
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
            if (!std::filesystem::is_regular_file(default_path))
                continue;

            auto const path = Cool::Path::user_data() / std::filesystem::relative(default_path, Cool::Path::default_user_data());
            if (std::filesystem::exists(path))
                continue;

            try
            {
                Cool::File::copy_file(default_path, path);
            }
            catch (std::exception const& e)
            {
                Cool::Log::ToUser::warning("Default user data", fmt::format("Failed to copy {} to {}:\n{}", std::filesystem::weakly_canonical(default_path), std::filesystem::weakly_canonical(path), e.what()));
            }
        }
    }
    catch (std::exception const& e)
    {
        Cool::Log::ToUser::warning("Default user data", fmt::format("Failed to copy default user data:\n{}", e.what()));
    }
}

} // namespace Cool::internal