# ########################################################################### #
#                                                                             #
# Copyright © 2021 Kalynovsky Valentin. All rights reserved.                  #
# Licensed under the Apache License, Version 2.0                              #
#                                                                             #
# Licensed under the Apache License, Version 2.0 (the "License");             #
# you may not use this file except in compliance with the License.            #
# You may obtain a copy of the License at                                     #
#                                                                             #
#     http://www.apache.org/licenses/LICENSE-2.0                              #
#                                                                             #
# Unless required by applicable law or agreed to in writing, software         #
# distributed under the License is distributed on an "AS IS" BASIS,           #
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.    #
# See the License for the specific language governing permissions and         #
# limitations under the License.                                              #
#                                                                             #
# ########################################################################### #

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11
RC_ICONS = icon.ico
VERSION = 1.1.0.1
QMAKE_TARGET_COMPANY = Kalynovsky Valentin
QMAKE_TARGET_PRODUCT = NotePad
QMAKE_TARGET_DESCRIPTION = A program for notes, documents and coding.
QMAKE_TARGET_COPYRIGHT = Copyright (C) 2021 Kalynovsky Valentin. All rights reserved.

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
	highlight_search.cpp \
	highlight_syntax.cpp \
    sheetofnotes.cpp

HEADERS += \
    highlight_search.h \
	highlight_syntax.h \
    sheetofnotes.h

FORMS += \
    sheetofnotes.ui

TRANSLATIONS += \
    NotePad_en_US.ts \
	NotePad_jp_JP.ts \
	NotePad_ru_RU.ts \
	NotePad_ru_UA.ts \
	NotePad_ua_UA.ts

CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

DISTFILES += \
    LICENSE \
    icon.ico
