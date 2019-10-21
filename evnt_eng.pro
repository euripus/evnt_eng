TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG(release, debug|release) {
    #This is a release build
    DEFINES += NDEBUG
} else {
    #This is a debug build
    DEFINES += DEBUG
    TARGET = $$join(TARGET,,,_d)
}

DESTDIR = $$PWD/bin

QMAKE_CXXFLAGS += -std=c++17 -Wno-unused-parameter

INCLUDEPATH += $$PWD/include

LIBS += -L$$PWD/lib

win32:{
    INCLUDEPATH += d:/build/boost_1_69_0
    LIBS += -Ld:/build/boost_1_69_0/stage/lib 
    LIBS += -lboost_log-mgw73-mt-x32-1_69 -lboost_system-mgw73-mt-x32-1_69
    LIBS += -lboost_locale-mgw73-mt-x32-1_69 -lboost_thread-mgw73-mt-x32-1_69
    LIBS += -lboost_filesystem-mgw73-mt-x32-1_69
    LIBS += -liconv -lz -lws2_32
    LIBS += -lstdc++fs -static-libgcc -static-libstdc++ -static -lpthread
}
unix:{
    DEFINES += BOOST_LOG_DYN_LINK
    LIBS += -lboost_thread -lboost_system -lboost_filesystem -lboost_log -lboost_locale
    LIBS += -lz -lpthread -lstdc++fs
}

SOURCES +=  \
    src/app/app.cpp \
    src/app/appstate.cpp \
    src/app/mousecursor.cpp \
    src/app/window.cpp \
    src/core/cmpmsgs.cpp \
    src/core/component.cpp \
    src/core/core.cpp \
    src/core/exception.cpp \
    src/core/gameobject.cpp \
    src/core/memory_stream.cpp \
    src/core/object.cpp \
    src/core/objectmanager.cpp \
    src/fs/file.cpp \
    src/fs/file_system.cpp \
    src/input/input.cpp \
    src/log/log.cpp \
    src/main.cpp \
    src/network/connection.cpp \
    src/network/socketaddress.cpp \
    src/network/udpsocket.cpp \
    src/resource/imagedata.cpp

HEADERS += \
    src/app/app.h \
    src/app/appstate.h \
    src/app/mousecursor.h \
    src/app/window.h \
    src/core/classids.h \
    src/core/cmpmsgs.h \
    src/core/component.h \
    src/core/core.h \
    src/core/event.h \
    src/core/exception.h \
    src/core/gameobject.h \
    src/core/memory_stream.h \
    src/core/module.h \
    src/core/object.h \
    src/core/objectmanager.h \
    src/core/objhandle.h \
    src/core/threadpool.h \
    src/fs/file.h \
    src/fs/file_system.h \
    src/fs/zip.h \
    src/input/devices.h \
    src/input/input.h \
    src/log/log.h \
    src/network/connection.h \
    src/network/socketaddress.h \
    src/network/udpsocket.h \
    src/resource/imagedata.h
