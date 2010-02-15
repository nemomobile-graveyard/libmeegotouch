include(../mkspecs/common.pri)

TARGET = duiservicemapper
TEMPLATE = app
SOURCES += \
    duiservicemapperadaptor.cpp \
    duiservicemapper.cpp \
    duiservicemapper_p.cpp \
    main.cpp \

HEADERS += \
    duiservicemapperadaptor.h \
    duiservicemapper.h \
    duiservicemapper_p.h \

QT += dbus


myname = com.nokia.servicefw

services.CONFIG += no_check_exist
services.target = $${myname}.service
services.commands = cat $$IN_PWD/$${myname}.service.in \
                 | sed -e "s:@DUI_INSTALL_BIN@:$${DUI_INSTALL_BIN}:g" \
                 > $${myname}.service
services.files = $$OUT_PWD/$${myname}.service
services.path = $$DUI_DBUS_SERVICES_DIR


QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)

target.path = $$DUI_INSTALL_BIN
INSTALLS += \
    target \
    services
