add_library(sourcevaultcore STATIC
        shared/Cloneable.h
        shared/MediumVisitor.h
        shared/MediumTypeVisitor.h
        shared/MediumTypeVisitor.cpp
        
        model/Medium.cpp
        model/Medium.h
        model/Book.cpp
        model/Book.h
        model/Video.cpp
        model/Video.h
        model/Article.cpp
        model/Article.h
        model/Library.cpp
        model/Library.h
        model/ValidatedField.h
        model/ValidatedSet.h
        model/MediumUserData.cpp
        model/MediumUserData.h

        queries/QueryBuilder.cpp
        queries/QueryBuilder.h

        queries/filters/Filter.h
        queries/filters/FavoriteFilter.h
        queries/filters/FavoriteFilter.cpp
        queries/filters/MediumTypeFilter.h
        queries/filters/MediumTypeFilter.cpp

        queries/sortings/Sort.cpp
        queries/sortings/Sort.h
        queries/sortings/TitleSort.cpp
        queries/sortings/TitleSort.h
)
target_include_directories(sourcevaultcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sourcevaultcore PUBLIC Qt6::Core Qt6::Network)
