#!/bin/bash
g++ -c -pipe -std=c++11 -O2 -pg -O2 -march=x86-64 -mtune=generic -O2 -pipe -std=c++0x -Wall -W -fPIC  -I. -I/usr/lib/qt/mkspecs/linux-g++ *.cpp
ar rcs libascii-gl.a *.o
cp libascii-gl.a *.h ../AsciiEngine/AsciiGL/
