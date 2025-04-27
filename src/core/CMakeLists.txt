add_library(sourcevaultcore STATIC
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
        model/OptionalField.h
)
target_include_directories(sourcevaultcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sourcevaultcore PUBLIC Qt6::Core Qt6::Network)
