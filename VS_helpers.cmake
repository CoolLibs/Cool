# Hide "ALL_BUILD" and "ZERO_CHECK" in a folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Recreate the folder architecture inside the Visual Studio solution (might work for other IDEs as well)
function(VS_RegisterFiles _FILES)
    foreach(_file IN ITEMS ${_FILES})
        get_filename_component(_filepath "${_file}" PATH)
        string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_filepath}")
        string(REPLACE "/" "\\" _group_path "${_group_path}")
        source_group("${_group_path}" FILES "${_file}")
    endforeach()
endfunction()