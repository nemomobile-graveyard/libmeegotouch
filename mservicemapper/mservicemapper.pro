include(../mkspecs/common.pri)

TARGET = mservicemapper
TEMPLATE = app
SOURCES += \
    mservicemapperadaptor.cpp \
    mservicemapperapplication.cpp \
    mservicemapper.cpp \
    mservicemapper_p.cpp \
    main.cpp \

HEADERS += \
    mservicemapperadaptor.h \
    mservicemapperapplication.h \
    mservicemapper.h \
    mservicemapper_p.h \

QT += dbus

contains( DEFINES, HAVE_QTM_SERVICEFRAMEWORK ) {
    CONFIG += mobility
    MOBILITY += serviceframework
}

myname = com.nokia.mservicefw

services.CONFIG += no_check_exist
services.target = $${myname}.service
services.commands = cat $$IN_PWD/$${myname}.service.in \
                 | sed -e "s:@M_INSTALL_BIN@:$${M_INSTALL_BIN}:g" \
                 > $${myname}.service
services.files = $$OUT_PWD/$${myname}.service
services.path = $$M_DBUS_SERVICES_DIR


QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)

target.path = $$M_INSTALL_BIN
INSTALLS += \
    target \
    services
