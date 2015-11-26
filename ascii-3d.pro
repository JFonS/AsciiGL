TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lncurses \
        -lsfml-graphics \
        -lsfml-system \
        -lsfml-window



SOURCES += main.cpp \
    Framebuffer.cpp \
    Pipeline.cpp \
    VAO.cpp \
    ShaderProgram.cpp \
    GenericMap.cpp

HEADERS += \
    Framebuffer.h \
    Pipeline.h \
    VAO.h \
    ShaderProgram.h \
    GenericMap.h

