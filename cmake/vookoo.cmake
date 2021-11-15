add_library(Vookoo INTERFACE)
add_library(Vookoo::Vookoo ALIAS Vookoo)
target_include_directories(Vookoo INTERFACE lib/Vookoo/include)

# Try to set SPIR-V support
function(set_spirv_support)
    message(STATUS "[Vookoo] Found " ${SPIR_V_FILE})
    message(STATUS "[Vookoo] Setting VOOKOO_SPIRV_SUPPORT definition")
    target_include_directories(Vookoo INTERFACE ${SPIR_V_INCLUDE_DIR})
    target_compile_definitions(Vookoo INTERFACE VOOKOO_SPIRV_SUPPORT)
endfunction()

get_filename_component(VULKAN_DIR ${Vulkan_INCLUDE_DIR} DIRECTORY)
# Try nb 1
set(SPIR_V_INCLUDE_DIR ${VULKAN_DIR}/spirv-tools/external/spirv-headers/include/spirv/unified1)
set(SPIR_V_FILE ${SPIR_V_INCLUDE_DIR}/spirv.hpp11)
if(EXISTS ${SPIR_V_FILE})
    set_spirv_support()
else()
# Try nb 2
    set(SPIR_V_INCLUDE_DIR ${VULKAN_DIR}/Include/spirv-headers)
    set(SPIR_V_FILE ${SPIR_V_INCLUDE_DIR}/spirv.hpp11)
    if(EXISTS ${SPIR_V_FILE})
        set_spirv_support()
    else()
        message(STATUS "[Vookoo] Couldn't find SPIR-V")
    endif()
endif()