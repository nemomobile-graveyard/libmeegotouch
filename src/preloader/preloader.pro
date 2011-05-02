include( ../common_top.pri )

TARGET = meegotouchpreloader

INCLUDEPATH += \
    ../corelib/preloader \

LIBS += $$mAddLibrary(meegotouchcore)

SOURCES += preloader.cpp \

include(../common_bot.pri)
