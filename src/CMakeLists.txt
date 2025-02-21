add_subdirectory(core)
add_subdirectory(gui)

add_executable(sourcevault main.cpp)
target_link_libraries(sourcevault PRIVATE sourcevaultcore sourcevaultgui)
target_compile_options(sourcevault PRIVATE ${COMMON_COMPILE_OPTIONS})
