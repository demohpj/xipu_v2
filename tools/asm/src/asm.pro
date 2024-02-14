QT -= gui
QT += core

CONFIG += c++14 console
CONFIG -= app_bundle
CONFIG -= debug_and_release debug_and_release_target

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    asm.cpp \
    instruction.cpp \
    codeitem.cpp \
    codesection.cpp \
    main.cpp \
    ramitem.cpp \
    flashitem.cpp \
    data.cpp \
    parser.cpp \
    compiler.cpp \
    linker.cpp \
    datalist.cpp

HEADERS += \
    asm.h \
    instruction.h \
    codeitem.h \
    codesection.h \
    ramitem.h \
    flashitem.h \
    data.h \
    parser.h \
    compiler.h \
    linker.h \
    datalist.h
