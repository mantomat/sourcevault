add_executable(sourcevaultgui-qt6
        main.cpp

        AppController.cpp
        AppController.h 
        MainWindow.cpp
        MainWindow.h

)

target_include_directories(sourcevaultgui-qt6 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sourcevaultgui-qt6 PUBLIC sourcevaultcore Qt6::Core Qt6::Gui Qt6::Widgets)
target_compile_options(sourcevaultgui-qt6 PRIVATE ${COMMON_COMPILE_OPTIONS})
