file(GLOB_RECURSE DemoSources "*.h" "*.hpp" "*.c" "*.cpp" "*.cc")

add_executable(Demo ${DemoSources})
target_link_libraries(Demo HaleonEngine)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Demo)