include(../mkspecs/common.pri)

CONFIG += ordered
TEMPLATE    = subdirs
SUBDIRS     =  \
    widgetsgallery \
    widgetsgallery/translations \

# we do not have compiled QtDBusd4
!macx:!win32 {
    contains(DEFINES, HAVE_DBUS) {
        SUBDIRS += \
        appletinstallationsource \
        applicationextension \
        applicationextension/demoextension \
        applicationextension/demoextension2
    }
}

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
