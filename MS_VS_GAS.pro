TEMPLATE = app

TARGET = MS_VS_GAS

QT += core gui widgets charts printsupport

SOURCES += \
    qcustomplot/qcustomplot.cpp \
    ArrayWindow.cpp \
    GraphWindow.cpp \
    MatrixWindow.cpp \
    MS_VS_GAS.cpp \
    main.cpp

HEADERS += \
    qcustomplot/qcustomplot.h \
    ArrayWindow.h \
    GraphWindow.h \
    MatrixWindow.h \
    MS_VS_GAS.h

FORMS += \
    arrayWindow.ui \
    graphWindow.ui \
    matrixWindow.ui \
    MS_VS_GAS.ui

RESOURCES += \
    MS_VS_GAS.qrc

CONFIG += debug
CONFIG(debug, debug|release) {
    QT_BUILD_CONFIG = debug
}

CONFIG(release, debug|release) {
    QT_BUILD_CONFIG = release
}
