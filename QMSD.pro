#-------------------------------------------------
#
# Project created by QtCreator 2011-02-06T10:49:50
#
#-------------------------------------------------

QT       += core gui xml network

TARGET = QMSD
TEMPLATE = app
OBJECTS_DIR = ./obj/
MOC_DIR = ./obj/
DESTDIR = ./bin/
UI_DIR = ./obj/

TRANSLATIONS = ./lang/qmsd_ru.ts \
                ./lang/qmsd_def.ts \
                ./lang/qmsd_en.ts \


SOURCES += main.cpp\
        src/medisitedownloder.cpp

HEADERS  += headers/medisitedownloder.h

FORMS    += ui/medisitedownloder.ui

OTHER_FILES += \
    README
