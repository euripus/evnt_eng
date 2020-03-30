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
    src/render/gl45/glbuffer.cpp \
    src/render/gl45/glframebuffer.cpp \
    src/render/gl45/glpipeline.cpp \
    src/render/gl45/glrenderdevice.cpp \
    src/render/gl45/glsampler.cpp \
    src/render/gl45/glshader.cpp \
    src/render/gl45/glshaderconstants.cpp \
    src/render/gl45/glshaderdescriptor.cpp \
    src/render/gl45/gltexture.cpp \
    src/render/gl45/glvertexarray.cpp \
    src/render/gl45/typeconversions.cpp \
    src/render/pipeline.cpp \
    src/render/quad2d.cpp \
    src/render/render.cpp  \
    src/render/renderdevice.cpp \
    src/render/sampler.cpp \
    src/render/shaderconstants.cpp \
    src/render/shaderdescriptor.cpp \
    src/render/texture.cpp

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
    src/render/buffer.h \
    src/render/framebuffer.h \
    src/render/gl45/glbuffer.h \
    src/render/gl45/glframebuffer.h \
    src/render/gl45/glpipeline.h \
    src/render/gl45/glrenderdevice.h \
    src/render/gl45/glsampler.h \
    src/render/gl45/glshader.h \
    src/render/gl45/glshaderconstants.h \
    src/render/gl45/glshaderdescriptor.h \
    src/render/gl45/gltexture.h \
    src/render/gl45/glvertexarray.h \
    src/render/gl45/typeconversions.h \
    src/render/graphics_types.h \
    src/render/pipeline.h \
    src/render/quad2d.h \
    src/render/render.h  \
    src/render/renderdevice.h \
    src/render/sampler.h \
    src/render/shader.h \
    src/render/shaderconstants.h \
    src/render/shaderdescriptor.h \
    src/render/texture.h \
    src/render/vertexarray.h
