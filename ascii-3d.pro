TEMPLATE = app
CONFIG += console c++11
QMAKE_CXXFLAGS += -std=c++11 -O2
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lncurses

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
    GenericMap.h \

