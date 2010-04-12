include(../mkspecs/common.pri)

TEMPLATE = subdirs
SUBDIRS =

QMAKE_EXTRA_TARGETS += check
check.depends =
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends =
check-xml.commands = $$system(true)

mmoc.target = .dummy
mmoc.commands = touch $$mmoc.target
mmoc.path = $$M_INSTALL_BIN
mmoc.files = mmoc

INSTALLS += \
     mmoc\
