TEMPLATE = app

QT += qml quick widgets

CONFIG += c++11

QMAKE_CXXFLAGS += -pedantic -pedantic-errors -Wall -Wextra -Werror

INCLUDEPATH += /home/mtrath/Projekte/boost_1_61_0/ ../ThreadPoolExecutor/
LIBS += "-L/home/mtrath/Projekte/boost_1_61_0/" -lboost_system -lboost_thread

SOURCES += main.cpp \
    MainViewQmlContext.cpp \
    ../ThreadPoolExecutor/ThreadPoolExecutor.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    MainViewQmlContext.h \
    PrintThread.h \
    ../ThreadPoolExecutor/ThreadPoolExecutor.h \
    ../ThreadPoolExecutor/Task.h
