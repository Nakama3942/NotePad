QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11
RC_ICONS = icon.ico
VERSION = 1.0.0.1
QMAKE_TARGET_COMPANY = Kalynovsky Valentin
QMAKE_TARGET_PRODUCT = NotePad
QMAKE_TARGET_DESCRIPTION = A program for creating simple and complex notes.
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2021 Kalynovsky Valentin. All rights reserved.

#LIBS += \
#    -LLibrary
#	-LLibrary -llibstdc++-6.dll
#	-LLibrary -llibwinpthread-1.dll
#	-LLibrary -lQt6Core.dll
#	-LLibrary -lQt6Gui.dll
#	-LLibrary -lQt6PrintSupport.dll
#	-LLibrary -lQt6Svg.dll
#	-LLibrary -lQt6Widgets.dll

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    aboutprog.cpp \
    createfile.cpp \
    deletefile.cpp \
    imageinserter.cpp \
    main.cpp \
    sheetofnotes.cpp

HEADERS += \
    aboutprog.h \
    createfile.h \
    deletefile.h \
    imageinserter.h \
    sheetofnotes.h

FORMS += \
    aboutprog.ui \
    createfile.ui \
    deletefile.ui \
    imageinserter.ui \
    sheetofnotes.ui

TRANSLATIONS += \
    NotePad_en_US.ts \
	NotePad_ru_RU.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc
