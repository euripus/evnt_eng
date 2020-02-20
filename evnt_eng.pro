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
    LIBS += -lglfw3dll -lglew32dll -lopengl32
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
    src/assets/assetmanager.cpp \
    src/assets/imagedata.cpp \
    src/core/cmpmsgs.cpp \
    src/core/component.cpp \
    src/core/core.cpp \
    src/core/exception.cpp \
    src/core/gameobject.cpp \
    src/core/object.cpp \
    src/core/objectmanager.cpp \
    src/fs/file.cpp \
    src/fs/file_system.cpp \
    src/fs/memory_stream.cpp \
    src/input/input.cpp \
    src/input/inputglfw.cpp \
    src/input/keybinding.cpp \
    src/log/log.cpp \
    src/main.cpp \
    src/network/connection.cpp \
    src/network/socketaddress.cpp \
    src/network/udpsocket.cpp \
    src/render/common/texturebase.cpp \
    src/render/gl/devicecontextglimpl.cpp \
    src/render/gl/glcontext.cpp \
    src/render/gl/glcontextstate.cpp \
    src/render/gl/renderdeviceglimpl.cpp \
    src/render/gl/texturebasegl.cpp \
    src/render/gl/textureviewglimpl.cpp \
    src/render/gl/typeconversions.cpp \
    src/render/render.cpp \
    src/render/utils/colorconversion.cpp \
    src/render/utils/graphicsaccessories.cpp 

HEADERS += \
    src/app/app.h \
    src/app/appstate.h \
    src/app/command.h \
    src/app/glfwwindow.h \
    src/app/mousecursor.h \
    src/app/window.h \
    src/assets/assetmanager.h \
    src/assets/imagedata.h \
    src/core/classids.h \
    src/core/cmpmsgs.h \
    src/core/component.h \
    src/core/core.h \
    src/core/event.h \
    src/core/exception.h \
    src/core/gameobject.h \
    src/core/module.h \
    src/core/object.h \
    src/core/objectmanager.h \
    src/core/objhandle.h \
    src/core/threadpool.h \
    src/fs/file.h \
    src/fs/file_system.h \
    src/fs/memory_stream.h \
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
    src/render/common/blendstate.h \
    src/render/common/depthstencilstate.h \
    src/render/common/devicecaps.h \
    src/render/common/devicecontextbase.h \
    src/render/common/deviceobjectbase.h \
    src/render/common/graphics_types.h \
    src/render/common/interface/ibuffer.h \
    src/render/common/interface/ibufferview.h \
    src/render/common/interface/idevicecontext.h \
    src/render/common/interface/ideviceobject.h \
    src/render/common/interface/ifence.h \
    src/render/common/interface/inputlayout.h \
    src/render/common/interface/ipipelinestate.h \
    src/render/common/interface/irenderdevice.h \
    src/render/common/interface/iresourcemapping.h \
    src/render/common/interface/isampler.h \
    src/render/common/interface/ishader.h \
    src/render/common/interface/ishaderresourcebinding.h \
    src/render/common/interface/ishaderresourcevariable.h \
    src/render/common/interface/iswapchain.h \
    src/render/common/interface/itexture.h \
    src/render/common/interface/itextureview.h \
    src/render/common/objectbase.h \
    src/render/common/rasterizerstate.h \
    src/render/common/renderdevicebase.h \
    src/render/common/texturebase.h \
    src/render/common/textureviewbase.h \
    src/render/gl/asyncwritableresource.h \
    src/render/gl/devicecontextglimpl.h \
    src/render/gl/glcontext.h \
    src/render/gl/glcontextstate.h \
    src/render/gl/gldebug.h \
    src/render/gl/globjwrapper.h \
    src/render/gl/interface/idevicecontextgl.h \
    src/render/gl/interface/irenderdevicegl.h \
    src/render/gl/interface/itexturegl.h \
    src/render/gl/interface/itextureviewgl.h \
    src/render/gl/renderdeviceglimpl.h \
    src/render/gl/texturebasegl.h \
    src/render/gl/textureviewglimpl.h \
    src/render/gl/typeconversions.h \
    src/render/render.h \
    src/render/utils/colorconversion.h \
    src/render/utils/graphicsaccessories.h 
