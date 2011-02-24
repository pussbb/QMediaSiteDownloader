#-------------------------------------------------
#
# Project created by QtCreator 2011-02-06T10:49:50
#
#-------------------------------------------------

QT       += core gui network sql thread

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
        src/mediasitedownloder.cpp \
    src/addtask.cpp \
    src/taskdb.cpp \
    src/qparsesite.cpp


HEADERS  += headers/mediasitedownloder.h \
    headers/addtask.h \
    headers/taskdb.h \
    headers/qparsesite.h

FORMS    += \
    ui/mediasitedownloder.ui \
    ui/addtask.ui

OTHER_FILES += \
    README

RESOURCES += \
    resource/qmsd_main.qrc
