add_library(sourcevaultgui STATIC
        MainWindow.h MainWindow.cpp
)
target_include_directories(sourcevaultgui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sourcevaultgui PUBLIC sourcevaultcore Qt6::Core Qt6::Gui Qt6::Widgets)
target_compile_options(sourcevaultgui PRIVATE ${COMMON_COMPILE_OPTIONS})
