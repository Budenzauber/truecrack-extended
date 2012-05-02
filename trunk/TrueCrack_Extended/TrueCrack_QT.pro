#-------------------------------------------------
#
# Project created by QtCreator 2012-02-06T14:41:27
#
#-------------------------------------------------

QT       += core \
            gui \
            network


TARGET = TrueCrack_Extended
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    ServerModel.cpp \
    ClientView.cpp \
    ClientModel.cpp \
    aboutwindow.cpp \
    ServerView.cpp

HEADERS += \
    ServerModel.h \
    ClientView.h \
    ClientModel.h \
    aboutwindow.h \
    ServerView.h

OTHER_FILES += \
    Namenskonvention.txt \
    TODO_und_Ideen.txt

FORMS += \
    aboutwindow.ui \
    ServerView.ui
