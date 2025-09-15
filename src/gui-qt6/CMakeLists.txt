add_executable(sourcevaultgui-qt6

        resources.qrc

        main.cpp

        AppController.cpp
        AppController.h 
        MainWindow.cpp
        MainWindow.h
        DialogsController.cpp
        DialogsController.h
        DetailPageFactory.cpp
        DetailPageFactory.h
        MediumToDetailPageVisitor.cpp
        MediumToDetailPageVisitor.h

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

        components/menubar/Menubar.cpp
        components/menubar/Menubar.h
        components/menubar/MenubarController.cpp
        components/menubar/MenubarController.h

        components/medium-detail/CreateController.h
        components/medium-detail/DetailPage.cpp
        components/medium-detail/DetailPage.h
        components/medium-detail/DetailPageController.h
        components/medium-detail/VideoCreateController.cpp
        components/medium-detail/VideoCreateController.h
        components/medium-detail/VideoDetailPage.cpp
        components/medium-detail/VideoDetailPage.h
        components/medium-detail/VideoDetailPageController.cpp
        components/medium-detail/VideoDetailPageController.h
        components/medium-detail/ArticleCreateController.cpp
        components/medium-detail/ArticleCreateController.h
        components/medium-detail/ArticleDetailPage.cpp
        components/medium-detail/ArticleDetailPage.h
        components/medium-detail/ArticleDetailPageController.cpp
        components/medium-detail/ArticleDetailPageController.h
        components/medium-detail/BookCreateController.cpp
        components/medium-detail/BookCreateController.h
        components/medium-detail/BookDetailPage.cpp
        components/medium-detail/BookDetailPage.h
        components/medium-detail/BookDetailPageController.cpp
        components/medium-detail/BookDetailPageController.h

        components/medium-detail/detail-sections/MediumDetailSection.cpp
        components/medium-detail/detail-sections/MediumDetailSection.h
        components/medium-detail/detail-sections/MediumSectionController.cpp
        components/medium-detail/detail-sections/MediumSectionController.h
        components/medium-detail/detail-sections/UserDataDetailSection.cpp
        components/medium-detail/detail-sections/UserDataDetailSection.h
        components/medium-detail/detail-sections/UserDataSectionController.cpp
        components/medium-detail/detail-sections/UserDataSectionController.h
        components/medium-detail/detail-sections/VideoDetailSection.cpp
        components/medium-detail/detail-sections/VideoDetailSection.h
        components/medium-detail/detail-sections/VideoSectionController.cpp
        components/medium-detail/detail-sections/VideoSectionController.h
        components/medium-detail/detail-sections/ArticleDetailSection.cpp
        components/medium-detail/detail-sections/ArticleDetailSection.h
        components/medium-detail/detail-sections/ArticleSectionController.cpp
        components/medium-detail/detail-sections/ArticleSectionController.h
        components/medium-detail/detail-sections/BookDetailSection.cpp
        components/medium-detail/detail-sections/BookDetailSection.h
        components/medium-detail/detail-sections/BookSectionController.cpp
        components/medium-detail/detail-sections/BookSectionController.h

        components/medium-detail/field-editors/BooleanFieldEditor.cpp
        components/medium-detail/field-editors/BooleanFieldEditor.h
        components/medium-detail/field-editors/DateFieldEditor.cpp
        components/medium-detail/field-editors/DateFieldEditor.h
        components/medium-detail/field-editors/FieldEditor.cpp
        components/medium-detail/field-editors/FieldEditor.h
        components/medium-detail/field-editors/NumberFieldEditor.cpp
        components/medium-detail/field-editors/NumberFieldEditor.h
        components/medium-detail/field-editors/PriorityFieldEditor.cpp
        components/medium-detail/field-editors/PriorityFieldEditor.h
        components/medium-detail/field-editors/SetFieldEditor.cpp
        components/medium-detail/field-editors/SetFieldEditor.h
        components/medium-detail/field-editors/StringFieldEditor.cpp
        components/medium-detail/field-editors/StringFieldEditor.h
        components/medium-detail/field-editors/TextFieldEditor.cpp
        components/medium-detail/field-editors/TextFieldEditor.h
        components/medium-detail/field-editors/UrlFieldEditor.cpp
        components/medium-detail/field-editors/UrlFieldEditor.h
)

target_include_directories(sourcevaultgui-qt6 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sourcevaultgui-qt6 PUBLIC sourcevaultcore Qt6::Core Qt6::Gui Qt6::Widgets)
target_compile_options(sourcevaultgui-qt6 PRIVATE ${COMMON_COMPILE_OPTIONS})
