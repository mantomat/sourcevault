TEMPLATE = app
TARGET = search_tester
CONFIG += c++20 console
CONFIG -= app_bundle

INCLUDEPATH += $$PWD ../../src/core

LIBS += -L"$$OUT_PWD/../../src/core" -lsourcevaultcore

HEADERS += \
    FieldScoreCalculatorRepl.h

SOURCES += \
    main.cpp