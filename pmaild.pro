######################################################################
# Automatically generated by qmake (2.01a) Thu Jun 7 17:34:21 2012
######################################################################

TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
QT -= gui
QT += network sql

QMAKE_CXXFLAGS += -std=gnu++0x

# Input
HEADERS += PMaildServer.hpp PMaildCore.hpp PMaildCoreMySQL.hpp PMaildServerBase.hpp PMaildServerSmtp.hpp PMaildMtaTxn.hpp \
    PMaildServerImap4.hpp \
    PMaildServerPop3.hpp
SOURCES += main.cpp PMaildServer.cpp PMaildCoreMySQL.cpp PMaildServerBase.cpp PMaildServerSmtp.cpp PMaildMtaTxn.cpp \
    PMaildServerImap4.cpp \
    PMaildServerPop3.cpp \
    PMaildCore.cpp
