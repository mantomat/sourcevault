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

        queries/FilteringQuery.cpp
        queries/FilteringQuery.h
        queries/SearchQuery.cpp
        queries/SearchQuery.h
        queries/SortingQuery.cpp
        queries/SortingQuery.h

        queries/search/SearchEngine.cpp
        queries/search/SearchEngine.h
        queries/search/SearchScoreVisitor.cpp
        queries/search/SearchScoreVisitor.h
        queries/search/FieldScoreCalculator.cpp
        queries/search/FieldScoreCalculator.h

        queries/filters/Filter.h
        queries/filters/FavoriteFilter.h
        queries/filters/FavoriteFilter.cpp
        queries/filters/MediumTypeFilter.h
        queries/filters/MediumTypeFilter.cpp

        queries/sortings/Sort.cpp
        queries/sortings/Sort.h
        queries/sortings/TitleSort.cpp
        queries/sortings/TitleSort.h
        queries/sortings/DateAddedSort.cpp
        queries/sortings/DateAddedSort.h
)
target_include_directories(sourcevaultcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sourcevaultcore PUBLIC Qt6::Core Qt6::Network)
