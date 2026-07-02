TEMPLATE = app
TARGET   = tests
CONFIG  += c++17 console
QT      += widgets printsupport testlib

DEFINES += CHRONOPLOTTER_TESTS
DEFINES += FIXTURES_DIR=\\\"$$PWD/fixtures\\\"

QXLSX_PARENTPATH=../QXlsx/
QXLSX_HEADERPATH=../QXlsx/header/
QXLSX_SOURCEPATH=../QXlsx/source/
include(../QXlsx/QXlsx.pri)

INCLUDEPATH += .. ../qcustomplot

SOURCES += \
    main.cpp \
    TestMath.cpp \
    TestShotGroup.cpp \
    TestParsers.cpp \
    TestUntar.cpp \
    ../ChronoPlotter.cpp \
    ../qcustomplot/qcustomplot.cpp \
    ../untar.cpp \
    ../miniz.c \
    ../PowderTest.cpp \
    ../SeatingDepthTest.cpp \
    ../TunerTest.cpp \
    ../About.cpp

HEADERS += \
    TestMath.h \
    TestShotGroup.h \
    TestParsers.h \
    TestUntar.h \
    ../ChronoPlotter.h \
    ../qcustomplot/qcustomplot.h \
    ../PowderTest.h \
    ../SeatingDepthTest.h \
    ../TunerTest.h \
    ../About.h

RESOURCES += ../resources.qrc

coverage {
    QMAKE_CXXFLAGS += --coverage
    QMAKE_LFLAGS   += --coverage
}
