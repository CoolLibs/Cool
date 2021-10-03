#pragma once

#include "hide_console_in_release.h"

#if defined(DEBUG)
#define DOCTEST_CONFIG_IMPLEMENT
#include <doctest/doctest.h>
#endif

#if defined(__COOL_APP_VULKAN)
#include <Cool/Gpu/Vulkan/Context.h>
#endif

#include <Cool/File/File.h>
#include <Cool/Icons/Icons.h>
#include <Cool/Log/Log.h>

namespace Cool {

/** 
 * @brief Initializes all the Cool systems. 
 * (NB : this function is somewhat opinionated and you might want to replace this with your own way of initializing Cool) 
 *  
 */
inline void initialize()
{
#if defined(DEBUG)
    // Run the tests !
    doctest::Context{}.run();
#endif

    Cool::Log::initialize();

#if defined(DEBUG)
    // When launching from an IDE (which almost always means we are in Debug mode)
    // the initial current_path will be set to bin/Debug
    // but I prefer to have it at the root of the project, so that the assets we load
    // are the ones at the root of the project, not the ones that have been copied to the bin folders.
    // This means that if for example we do hot reloading of shaders
    // we can modify the source assets (at the root), instead of the ones that have been copied.
    // This is important because only the ones at the root are tracked in Git.
    // Also, if you modify an asset at the root whithout modifying some code, and then restart the program
    // the asset won't be copied (because I couldn't figure out how to configure CMake to do so)
    // So it is better to always work with the assets at the root while in development.
    std::filesystem::current_path(COOL_DEBUG_ONLY__CMAKE_SOURCE_DIR);
#endif
    Cool::File::initialize_root_dir(std::filesystem::current_path().string());
}

/** 
 * @brief Shuts down all the Cool systems 
 *  
 */
inline void shut_down()
{
#if defined(__COOL_APP_VULKAN)
    vkDeviceWaitIdle(Vulkan::context().g_Device);
#endif
    Icons::shut_down();
}

} // namespace Cool
