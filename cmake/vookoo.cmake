
find_package(Vulkan REQUIRED)

include_directories(
    ${Vulkan_INCLUDE_DIR}
)

# Set spir-v support
get_filename_component(VULKAN_DIR ${Vulkan_INCLUDE_DIR} DIRECTORY)
set(SPIR_V_INCLUDE_DIR ${VULKAN_DIR}/spirv-tools/external/spirv-headers/include/spirv/)
set(SPIR_V_FILE ${SPIR_V_INCLUDE_DIR}unified1/spirv.hpp11)
if(EXISTS ${SPIR_V_FILE})
  message(STATUS "Found " ${SPIR_V_FILE})
  message(STATUS "Setting VOOKOO_SPIRV_SUPPORT definition")
  include_directories(${SPIR_V_INCLUDE_DIR})
  add_definitions(-DVOOKOO_SPIRV_SUPPORT)
endif()