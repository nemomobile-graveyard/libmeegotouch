include(../mkspecs/common.pri)

TEMPLATE = subdirs
SUBDIRS = \
    mapplettester \
    mapplicationextensiontester \
    mnotificationtool \
    mcssvalidator

macx:SUBDIRS -= mnotificationtool
macx:SUBDIRS -= mapplettester
macx:SUBDIRS -= mapplicationextensiontester
macx:SUBDIRS -= mcssvalidator

QMAKE_EXTRA_TARGETS += check
check.depends =
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends =
check-xml.commands = $$system(true)

tools.target = .dummy
tools.commands = touch $$tools.target
tools.path = $$M_INSTALL_BIN
tools.files = mnotificationstresstest dui-rename-files dui-rename-symbols

INSTALLS += \
    tools
