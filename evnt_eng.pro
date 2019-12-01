TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

CONFIG(release, debug|release) {
    #This is a release build
    DEFINES += NDEBUG
    QMAKE_CXXFLAGS += -s
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
    LIBS += -liconv -lz -lws2_32 -lshell32
    LIBS += -lstdc++fs -static-libgcc -static-libstdc++ -static -lpthread
    LIBS += -lglfw3dll -lglewdll -lopengl32
}
unix:{
    DEFINES += BOOST_LOG_DYN_LINK
    LIBS += -lboost_thread -lboost_system -lboost_filesystem -lboost_log -lboost_locale
    LIBS += -lz -lpthread -lstdc++fs
    LIBS += -lglfw -lGL -lGLEW
}

SOURCES +=  \
    src/app/app.cpp \
    src/app/appstate.cpp \
    src/app/command.cpp \
    src/app/glfwwindow.cpp \
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
    src/input/inputglfw.cpp \
    src/input/keybinding.cpp \
    src/log/log.cpp \
    src/main.cpp \
    src/network/connection.cpp \
    src/network/socketaddress.cpp \
    src/network/udpsocket.cpp \
    src/render/gl/glcontext.cpp \
    src/render/gl/glcontextstate.cpp \
    src/render/gl/typeconversions.cpp \
    src/render/utils/colorconversion.cpp \
    src/render/utils/graphicsaccessories.cpp \
    src/resource/imagedata.cpp

HEADERS += \
    src/app/app.h \
    src/app/appstate.h \
    src/app/command.h \
    src/app/glfwwindow.h \
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
    src/input/inputglfw.h \
    src/input/keybinding.h \
    src/log/debug_messages.h \
    src/log/log.h \
    src/network/connection.h \
    src/network/socketaddress.h \
    src/network/udpsocket.h \
    src/render/blend.h \
    src/render/depthstencilstate.h \
    src/render/devicecaps.h \
    src/render/gl/asyncwritableresource.h \
    src/render/gl/glcontext.h \
    src/render/gl/glcontextstate.h \
    src/render/gl/globjwrapper.h \
    src/render/gl/typeconversions.h \
    src/render/graphics_types.h \
    src/render/ibuffer.h \
    src/render/ibufferview.h \
    src/render/idevicecontext.h \
    src/render/ideviceobject.h \
    src/render/ifence.h \
    src/render/inputlayout.h \
    src/render/ipipelinestate.h \
    src/render/irenderdevice.h \
    src/render/iresourcemapping.h \
    src/render/isampler.h \
    src/render/ishader.h \
    src/render/ishaderresourcebinding.h \
    src/render/ishaderresourcevariable.h \
    src/render/iswapchain.h \
    src/render/itexture.h \
    src/render/itextureview.h \
    src/render/rasterizerstate.h \
    src/render/utils/colorconversion.h \
    src/render/utils/graphicsaccessories.h \
    src/resource/imagedata.h
