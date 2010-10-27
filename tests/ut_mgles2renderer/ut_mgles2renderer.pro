include(../common_top.pri)

TARGET = ut_mgles2renderer
QT += opengl

#INCLUDEPATH += $$MSRCDIR/corelib/theme

SOURCES += \
    ut_mgles2renderer.cpp \

HEADERS += \
    ut_mgles2renderer.h \

support_files.files += \
    shader1.frag \
    shader1.vert \
    shader2.frag \
    shader2.vert \
    glw1.png \
    glw2.png \

include(../common_bot.pri)
