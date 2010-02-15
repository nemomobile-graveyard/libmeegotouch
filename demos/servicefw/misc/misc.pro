DESTDIR=.
TEMPLATE = subdirs
SUBDIRS =

include( ../../../mkspecs/common.pri )

QMAKE_EXTRA_TARGETS += check
check.depends = $$TARGET
check.commands = $$system(true)

QMAKE_EXTRA_TARGETS += check-xml
check-xml.depends = $$TARGET
check-xml.commands = $$system(true)


myname = com.nokia.emailservice

service1.CONFIG += no_check_exist
service1.target = $${myname}.service
service1.commands = cat $$IN_PWD/$${myname}.service.in \
                 | sed -e "s:@DUI_INSTALL_BIN@:$${DUI_INSTALL_BIN}:g" \
                 > $${myname}.service
service1.files = $$OUT_PWD/$${myname}.service
service1.path = $$DUI_DBUS_SERVICES_DIR

myname = org.maemo.emailservice

service2.CONFIG += no_check_exist
service2.target = $${myname}.service
service2.commands = cat $$IN_PWD/$${myname}.service.in \
                 | sed -e "s:@DUI_INSTALL_BIN@:$${DUI_INSTALL_BIN}:g" \
                 > $${myname}.service
service2.files = $$OUT_PWD/$${myname}.service
service2.path = $$DUI_DBUS_SERVICES_DIR

myname = com.nokia.textprocessor

service3.CONFIG += no_check_exist
service3.target = $${myname}.service
service3.commands = cat $$IN_PWD/$${myname}.service.in \
                 | sed -e "s:@DUI_INSTALL_BIN@:$${DUI_INSTALL_BIN}:g" \
                 > $${myname}.service
service3.files = $$OUT_PWD/$${myname}.service
service3.path = $$DUI_DBUS_SERVICES_DIR

myname = org.maemo.textprocessor

service4.CONFIG += no_check_exist
service4.target = $${myname}.service
service4.commands = cat $$IN_PWD/$${myname}.service.in \
                 | sed -e "s:@DUI_INSTALL_BIN@:$${DUI_INSTALL_BIN}:g" \
                 > $${myname}.service
service4.files = $$OUT_PWD/$${myname}.service
service4.path = $$DUI_DBUS_SERVICES_DIR


INSTALLS += service1 service2 service3 service4
