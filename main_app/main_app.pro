#-------------------------------------------------
#
# Project created by QtCreator 2017-11-30T17:03:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = main_app
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += YOUR_PLATFORM C++11


#64bit or 32bit
contains(QMAKE_TARGET.arch, x86_64){
    OS=x64
}else{
    OS=i86
}

TARGET=$${TARGET}_$${OS}

#window release platform
win32:CONFIG(release, debug|release):{
    message("Release...")
    LIB_SUFFIX=""
}

#window debug platform
win32:CONFIG(debug, debug|release):{
    message("Debug...")
    LIB_SUFFIX=d
    TARGET=$${TARGET}d
}

YOUR_PLATFORM{
   message("YOUR_PLATFORM..................")
   #edit this var to the path where source code stored
   THIRD_DIR = $$PWD/../../pvt_libs
   CONFIG += OPENCV3
}

OPENCV3{
CV_DIR = D:\opencv\build
INCLUDEPATH += $${CV_DIR}/include
LIBS += -L$${CV_DIR}/x64\vc14\lib -lopencv_world320$${LIB_SUFFIX}
}


SOURCES += main.cpp\
        mainwindow.cpp \
    UIImageDisplay.cpp \
    imagefocusqualityssim.cpp

HEADERS  += mainwindow.h \
    UIImageDisplay.h \
    imagefocusqualityinterface.h \
    imagefocusqualityssim.h

FORMS    += mainwindow.ui
