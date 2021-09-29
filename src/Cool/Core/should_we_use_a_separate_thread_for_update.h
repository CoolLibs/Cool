#pragma once

#if defined(__COOL_APP_VULKAN) // We disable this for OpenGL because it would require creating the OpenGL context on this update thread, and I can't be bothered to do it atm
#define COOL_UPDATE_APP_ON_SEPARATE_THREAD
#endif