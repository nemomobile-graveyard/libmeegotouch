include(../mkspecs/common.pri)

TEMPLATE = subdirs
SUBDIRS = \
    duiapplettester \
    duiapplicationextensiontester \
    duinotificationtool \
    dui-servicefwgen.d \

macx:SUBDIRS -= duinotificationtool
macx:SUBDIRS -= duiapplettester
macx:SUBDIRS -= duiapplicationextensiontester

QMAKE_EXTRA_TARGETS += check
check.depends =
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends =
check-xml.commands = $$system(true)

tools.target = .dummy
tools.commands = touch $$tools.target
tools.path = $$DUI_INSTALL_BIN
tools.files = duinotificationstresstest

INSTALLS += \
    tools
