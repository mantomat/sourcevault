add_executable(sourcevaultgui-qt6

        resources.qrc

        main.cpp

        AppController.cpp
        AppController.h 
        MainWindow.cpp
        MainWindow.h

        components/library-page/LibraryPage.cpp
        components/library-page/LibraryPage.h
        components/library-page/LibraryPageController.cpp
        components/library-page/LibraryPageController.h
        components/library-page/MediumToCardVisitor.cpp
        components/library-page/MediumToCardVisitor.h
        components/library-page/LibraryQueryBuilder.cpp
        components/library-page/LibraryQueryBuilder.h
        components/library-page/MediumCardsViewModelFactory.cpp
        components/library-page/MediumCardsViewModelFactory.h
        components/library-page/topbar/LibraryTopbar.cpp
        components/library-page/topbar/LibraryTopbar.h
        components/library-page/sidebar/LibrarySidebar.cpp
        components/library-page/sidebar/LibrarySidebar.h
        components/library-page/media-list/LibraryMediaList.cpp
        components/library-page/media-list/LibraryMediaList.h
        components/library-page/media-list/LibraryMediumCard.cpp
        components/library-page/media-list/LibraryMediumCard.h

        components/thumbnail-renderer/ThumbnailRenderer.cpp
        components/thumbnail-renderer/ThumbnailRenderer.h

)

target_include_directories(sourcevaultgui-qt6 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sourcevaultgui-qt6 PUBLIC sourcevaultcore Qt6::Core Qt6::Gui Qt6::Widgets)
target_compile_options(sourcevaultgui-qt6 PRIVATE ${COMMON_COMPILE_OPTIONS})
