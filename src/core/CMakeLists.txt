add_library(sourcevaultcore STATIC
        model/Library.h model/Library.cpp
)
target_include_directories(sourcevaultcore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sourcevaultcore PUBLIC Qt6::Core Qt6::Network)
