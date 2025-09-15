TEMPLATE = subdirs

SUBDIRS += \
    src/core \
    src/gui-qt6 \
    tests/core \
    tools/search-tester

CONFIG += ordered

QMAKE_CXXFLAGS += -Wall -Wextra -Wpedantic -Werror -Wshadow
