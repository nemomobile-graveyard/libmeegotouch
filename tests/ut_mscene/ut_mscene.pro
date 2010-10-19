include(../common_top.pri)

TARGET = ut_mscene

INCLUDEPATH += \
    $$MSRCDIR/corelib/scene \

support_files.files += \
    ut_mscene_image.svg \
    ut_mscene_template.css \

SOURCES += \
    $$MSRCDIR/corelib/scene/mscene.cpp \
    ut_mscene.cpp \

HEADERS += \
    ut_mscene.h \
    $$MSRCDIR/corelib/scene/mscene.h \

include(../common_bot.pri)
