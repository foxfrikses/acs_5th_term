TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread

SOURCES += \
        arraytask.cpp \
        main.cpp \
        menu.cpp \
        tasks.cpp

HEADERS += \
    arraytask.h \
    menu.h \
    tasks.h
