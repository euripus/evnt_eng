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

SOURCES += 

HEADERS +=
