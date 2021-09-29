#pragma once

#if !defined(DEBUG)
// MSVC version
#if defined(_MSC_VER)
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
#endif