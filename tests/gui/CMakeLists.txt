add_executable(testsgui test_main.cpp)
target_link_libraries(testsgui sourcevaultcore sourcevaultgui Qt6::Test)
target_compile_options(testsgui PRIVATE ${COMMON_COMPILE_OPTIONS})
add_test(NAME TestsGui COMMAND testsgui)
