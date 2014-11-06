! include(../common.pri) {
  error( "Could not find the common.pri file!" )
}

QT       += network xml

TARGET = CartaLib
TEMPLATE = lib

DEFINES += CARTALIB_LIBRARY

SOURCES += CartaLib.cpp \
    HtmlString.cpp \
    IColormapScalar.cpp \
    Hooks/ColormapsScalar.cpp

HEADERS += CartaLib.h\
        cartalib_global.h \
    HtmlString.h \
    IColormapScalar.h \
    Hooks/ColormapsScalar.h \
    Hooks/HookIDs.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

OTHER_FILES += \
    readme.txt
