include(../common_top.pri)
TARGET = ft_mslider
QT += dbus svg network
LIBRARYPATH += $$MSRCDIR

SOURCES += \
	ft_mslider.cpp \

HEADERS += \
	ft_mslider.h \

support_files.files += \
        msliderstyle.css \
        mimagestyle.css \
        mlabelstyle.css \
        mslider.svg

include(../common_bot.pri)
