TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += thread

SOURCES += \
        arraytask.cpp \
        dynamicqueue.cpp \
        fixedatomicqueue.cpp \
        fixedmutexqueue.cpp \
        main.cpp \
        menu.cpp \
        queuetask.cpp \
        tasks.cpp

HEADERS += \
    arraytask.h \
    dynamicqueue.h \
    fixedatomicqueue.h \
    fixedmutexqueue.h \
    iqueue.h \
    menu.h \
    queuetask.h \
    tasks.h
