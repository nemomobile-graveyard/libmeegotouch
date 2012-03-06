include(../common_top.pri)
TARGET = ut_mpopuplistitem

OBJECTS_DIR = .obj
MOC_DIR = .moc

# unit test and unit
# MPopupListItem is defined in mpopuplistview_p.h
SOURCES += \
    ut_mpopuplistitem.cpp \
    $$MSRCDIR/views/mpopuplistview.cpp \
    $$MSRCDIR/views/mdialogview.cpp \
    $$MSRCDIR/corelib/widgets/mpopuplistmodel.cpp \
    $$MSRCDIR/views/mbuttongrouplayoutpolicy_p.cpp \

# unit test and unit
HEADERS += \
    ut_mpopuplistitem.h \
    $$MSRCDIR/views/mpopuplistview_p.h \
    $$MSRCDIR/views/mdialoginternalbox_p.h \
    $$MSRCDIR/corelib/widgets/mpopuplistmodel.h \

INCLUDEPATH += \
    $$MSRCDIR/corelib/widgets \
    $$MSRCDIR/views \
    ../../src/views/.moc \

include(../common_bot.pri)
