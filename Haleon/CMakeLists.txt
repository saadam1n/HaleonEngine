unset(HaleonEngine CACHE)

set(HaleonSupportedGraphicsAPIs "OpenGL3" "Direct3D9")
set(HaleonSelectedGraphicsAPI "OpenGL3" CACHE STRING "The graphics API used for rendering")
set_property(CACHE HaleonSelectedGraphicsAPI PROPERTY STRINGS ${HaleonSupportedGraphicsAPIs})

function(HaleonAddFolder FileList FileListData path)
    file(GLOB_RECURSE FolderSources CONFIGURE_DEPENDS "${path}/*.h" "${path}/*.hpp" "${path}/*.c" "${path}/*.cpp" "${path}/*.cc" )
    set(${FileList} ${FileListData} ${FolderSources} PARENT_SCOPE)
endfunction()

set(HaleonSources "")
HaleonAddFolder(HaleonSources "${HaleonSources}" "IO")
HaleonAddFolder(HaleonSources "${HaleonSources}" "Kernel")
HaleonAddFolder(HaleonSources "${HaleonSources}" "Utility")
HaleonAddFolder(HaleonSources "${HaleonSources}" "Window")
HaleonAddFolder(HaleonSources "${HaleonSources}" "Graphics")

set(HaleonLibraries "SDL2-static" "libconsole_c" "glm")
set(HaleonDefines "HALEON_ENGINE")

list(FIND HaleonSupportedGraphicsAPIs ${HaleonSelectedGraphicsAPI} HaleonGraphicsAPIIndex)

if(HaleonGraphicsAPIIndex EQUAL 0)
    HaleonAddFolder(HaleonSources "${HaleonSources}" "OpenGL3")
    list(APPEND HaleonLibraries "libglew_static")
    list(APPEND HaleonDefines "HALEON_GRAPHICS_API_OPENGL3")
elseif(HaleonGraphicsAPIIndex EQUAL 1)   
    HaleonAddFolder(HaleonSources "${HaleonSources}" "Direct3D9")
    list(APPEND HaleonLibraries "d3d9")
    list(APPEND HaleonDefines "HALEON_GRAPHICS_API_DIRECT3D9")
else()
    message(FATAL "Please select a valid graphics API. ${HaleonSelectedGraphicsAPI} is not a valid graphics API.")
endif()

list(REMOVE_ITEM HaleonSources "HaleonSources")

add_library(HaleonEngine STATIC ${HaleonSources})
set_target_properties(HaleonEngine PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# TODO: change the library linking to private and set library include directories to public
target_link_libraries(HaleonEngine PUBLIC ${HaleonLibraries})
target_compile_definitions(HaleonEngine PUBLIC ${HaleonDefines})
target_include_directories(HaleonEngine INTERFACE ${HaleonEngine_SOURCE_DIR})