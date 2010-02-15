CONFIG += ordered
TEMPLATE    = subdirs
SUBDIRS     =  \
    widgetsgallery \
    widgetsgallery/translations \
    animatedlayout \
    animatedlayout/translations \
    qtstyleexample \
    qtstyledialogs \
    appletinstallationsource
    
# we do not have compiled QtDBusd4
macx:SUBDIRS -= appletinstallationsource
win32:SUBDIRS -= appletinstallationsource 

contains(DEFINES, HAVE_DBUS) {
    SUBDIRS +=  servicefw
}

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
