CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS += \
    corelib \
    views \
    settings \
    extensions
win32|macx:SUBDIRS -= settings extensions

unix {
  include(../mkspecs/common.pri)
  schemas.CONFIG += no_check_exist
  schemas.files = $${IN_PWD}/data/meegotouch.schemas
  schemas.path = $${M_SYSCONFDIR}/gconf/schemas
  schemas.commands = GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) gconftool-2 --makefile-install-rule $${IN_PWD}/data/meegotouch.schemas

  INSTALLS += schemas
}

devices.CONFIG += no_check_exist
devices.files = $${IN_PWD}/data/devices.conf
devices.path = $$M_INSTALL_SYSCONF/meegotouch

INSTALLS += devices
