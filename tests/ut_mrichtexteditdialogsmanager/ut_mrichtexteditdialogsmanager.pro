include(../common_top.pri)

TARGET = ut_mrichtexteditdialogsmanager

SOURCES += \
    ut_mrichtexteditdialogsmanager.cpp \
    $$MSRCDIR/corelib/widgets/mrichtexteditdialogsmanager.cpp \

HEADERS += \
    ut_mrichtexteditdialogsmanager.h \
    $$MSRCDIR/corelib/widgets/mrichtexteditdialogsmanager_p.h \

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets

include(../common_bot.pri)
