include(../common_top.pri)
TARGET = ft_duislider
QT += dbus svg network
LIBRARYPATH += $$DUISRCDIR

SOURCES += \
	ft_duislider.cpp \

HEADERS += \
	ft_duislider.h \

support_files.files += \
        duisliderstyle.css \
        duiimagestyle.css \
        duilabelstyle.css \
        duislider.svg

include(../common_bot.pri)
