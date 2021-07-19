include_directories(${CMAKE_SOURCE_DIR}/Cool/lib/Vookoo/include)
find_package(Vulkan REQUIRED)

function(setspirvsupport)
  get_filename_component(VULKAN_DIR ${Vulkan_INCLUDE_DIR} DIRECTORY)
  set(SPIR_V_INCLUDE_DIR ${VULKAN_DIR}/spirv-tools/external/spirv-headers/include/spirv/)
  set(SPIR_V_FILE ${SPIR_V_INCLUDE_DIR}unified1/spirv.hpp11)
  if(EXISTS ${SPIR_V_FILE})
	message(STATUS "Found " ${SPIR_V_FILE})
	message(STATUS "Setting VOOKOO_SPIRV_SUPPORT definition")
    include_directories(${SPIR_V_INCLUDE_DIR})
	add_definitions(-DVOOKOO_SPIRV_SUPPORT)
  endif()
endfunction(setspirvsupport)

setspirvsupport()

include_directories(
    ${Vulkan_INCLUDE_DIR}
)

function(example order exname)
  set(shaders "")

  foreach(shader ${ARGN})
    add_custom_command(
      OUTPUT ${shader}.spv
      COMMAND glslangValidator -V ${PROJECT_SOURCE_DIR}/${exname}/${shader} -o ${PROJECT_BINARY_DIR}/${shader}.spv
      MAIN_DEPENDENCY ${exname}/${shader}
    )
    list(APPEND shaders "${exname}/${shader}")
  endforeach(shader)

  #message(STATUS "Included ${shaders} shaders")

  add_executable(${order}-${exname} ${exname}/${exname}.cpp ${shaders} ../include/vku/vku.hpp ../include/vku/vku_framework.hpp)

  # Require C++11
  target_compile_features(${order}-${exname} PRIVATE cxx_range_for)

  target_link_libraries(${order}-${exname} glfw Vulkan::Vulkan)

  if (WIN32)
    target_link_libraries(${order}-${exname})
	add_definitions(-DNOMINMAX)
  endif()

  if(APPLE)
    target_link_libraries(${order}-${exname} dl pthread)
  else(APPLE)
    if (UNIX)
      target_link_libraries(${order}-${exname} dl pthread X11 Xrandr Xinerama Xcursor)
    endif()
  endif(APPLE)
endfunction(example)