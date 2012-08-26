#-------------------------------------------------
#
# Project created by QtCreator 2012-05-25T15:50:57
#
#-------------------------------------------------

QT       -= core gui

TARGET = RuntimeLibrary
TEMPLATE = lib
CONFIG += staticlib
SOURCES += \
    isstring.cpp \
    system.cpp \
    string.cpp \
    math.cpp

HEADERS += \
    isstring.h \
    common.h \
    funcdef.h \
    string.h \
    system.h \
    math.h

#INCLUDEPATH += "$$(BOOST_INCLUDE)"
DEFINES += RUNTIME

TARGET_EXT = .bc
QMAKE_EXT_OBJ = .bc
QMAKE_CXXFLAGS -= -mthreads
QMAKE_CXXFLAGS += -emit-llvm
QMAKE_CXX = clang++
QMAKE_CC = clang
QMAKE_LIB = llvm-link -o
QMAKE_RUN_CXX = $(CXX) $(CXXFLAGS) $(INCPATH) -c $src -o $obj
QMAKE_RUN_CC = $(CC) $(CCFLAGS) $(INCPATH) -c $src -o $obj
