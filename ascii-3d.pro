TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lncurses \
        -lsfml-graphics \
        -lsfml-system \
        -lsfml-window



SOURCES += main.cpp

