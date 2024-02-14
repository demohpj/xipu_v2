QT += core gui widgets multimedia

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS
CONFIG -= debug_and_release debug_and_release_target

SOURCES += \
    cpu.cpp \
    fs.cpp \
    io.cpp \
    keyboard.cpp \
    lcd.cpp \
    lcdview.cpp \
    led.cpp \
    main.cpp \
    emu.cpp \
    rs232.cpp \
    rtc.cpp \
    speaker.cpp

HEADERS += \
    cpu.h \
    emu.h \
    font.h \
    fs.h \
    io.h \
    keyboard.h \
    lcd.h \
    lcdview.h \
    led.h \
    rs232.h \
    rtc.h \
    speaker.h

FORMS += \
    emu.ui
