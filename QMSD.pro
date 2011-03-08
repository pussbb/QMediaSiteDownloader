#-------------------------------------------------
#
# Project created by QtCreator 2011-02-06T10:49:50
#
#-------------------------------------------------

QT       += core gui network sql thread exceptions

TARGET = QMSD
TEMPLATE = app
unix:OBJECTS_DIR = ./obj/
unix:MOC_DIR = ./obj/
unix:DESTDIR = ./bin/
unix:UI_DIR = ./obj/

TRANSLATIONS = ./lang/qmsd_ru.ts \
                ./lang/qmsd_def.ts \
                ./lang/qmsd_en.ts \


SOURCES += main.cpp\
        src/mediasitedownloder.cpp \
    src/addtask.cpp \
    src/taskdb.cpp \
    src/qparsesite.cpp \
    src/errorlogui.cpp \
    src/downloadfile.cpp \
    src/about.cpp


HEADERS  += headers/mediasitedownloder.h \
    headers/addtask.h \
    headers/taskdb.h \
    headers/qparsesite.h \
    headers/errorlogui.h \
    headers/downloadfile.h \
    headers/about.h

FORMS    += \
    ui/mediasitedownloder.ui \
    ui/addtask.ui \
    ui/errorlogui.ui \
    ui/about.ui

OTHER_FILES += \
    README

RESOURCES += \
    resource/qmsd_main.qrc
