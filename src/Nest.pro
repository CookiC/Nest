#-------------------------------------------------
#
# Project created by QtCreator 2018-06-08T11:05:09
#
#-------------------------------------------------

QT      +=  core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG  +=  c++11 console
CONFIG  -=  debug_and_release_target \
            debug_and_release \
            release

TARGET = Nest

#TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
    cpython.cpp \
    cparray.cpp \
    genericdata.cpp \
    standarddata.cpp \
    nstring.cpp \
    abstractclassifier.cpp \
    cartclassifier.cpp \
    nimage.cpp \
    abstractestimate.cpp

HEADERS += \
    cpython.h \
    header.h \
    cparray.h \
    genericdata.h \
    standarddata.h \
    nstring.h \
    abstractdata.h \
    ntable.h \
    abstractclassifier.h \
    cartclassifier.h \
    nimage.h \
    abstractestimate.h

#指定生成的应用程序放置的目录
DESTDIR = ./bin

#指定uic命令将.ui文件转化成ui_*.h文件的存放的目录
UI_DIR = ./tmp

#指定rcc命令将.qrc文件转换成qrc_*.h文件的存放目录
RCC_DIR = ./tmp

#指定moc命令将含Q_OBJECT的头文件转换成标准.h文件的存放目录
MOC_DIR = ./tmp

#指定目标文件(obj)的存放目录
OBJECTS_DIR = ./tmp

#Note: This variable applies only to Windows targets.
#Specifies where to copy the target dll.
DLLDESTDIR =

CONFIG(release){
    PYTHONPATH = 'D:/Programming/Python 3.6'
    NUMPYPATH = '$$PYTHONPATH/Lib/site-packages/numpy/core'
    INCLUDEPATH +=  '$$PYTHONPATH/include' \
                    '$$NUMPYPATH/include/numpy'
    win32:LIBS +=   -L'./lib/' -lpython36 -lnpymath
    DEFINES += RELEASE
}

#message($$LIBS)
#message($$INCLUDEPATH)
#message($$NUMPYPATH)
#message($$Path)
#message($$CONFIG)
#message($$DESTDIR)
#message($$FORMS)
#message($$HEADERS)
#message($$QT)
#message($$SOURCES)
#message($$TEMPLATE)

