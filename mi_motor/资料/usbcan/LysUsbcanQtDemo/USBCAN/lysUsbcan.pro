QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
TARGET = lysUsbCanDemo
RC_ICONS = logo.ico

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/res.qrc

#ifdef __linux__
unix:!macx: LIBS += -L$$PWD/lib/linux/ -lControlCAN

INCLUDEPATH += $$PWD/include/linux
DEPENDPATH += $$PWD/include/linux

unix:!macx: PRE_TARGETDEPS += $$PWD/lib/linux/libControlCAN.a

unix:!macx: LIBS += -L$$PWD/lib/linux/ -lusb-1.0

CONFIG(debug, debug|release){
unix|macx: LIBS += -L$$PWD/lib/linux/debug -llysStyle
}
else {
unix|macx: LIBS += -L$$PWD/lib/linux/release -llysStyle
}

INCLUDEPATH += $$PWD/include/linux/
DEPENDPATH += $$PWD/include/linux/

unix:!macx: PRE_TARGETDEPS += $$PWD/lib/linux/libusb-1.0.a
#else

#endif

CONFIG(debug, debug|release){
unix|win32: LIBS += -L$$PWD/lib/window/debug -llysStyle
}
else {
unix|win32: LIBS += -L$$PWD/lib/window/release -llysStyle
}

INCLUDEPATH += $$PWD/lib/window/
DEPENDPATH += $$PWD/lib/window/
