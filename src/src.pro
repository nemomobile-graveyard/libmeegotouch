CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS += \
    corelib \
    views \
    settings \
    extensions
macx:SUBDIRS -= settings extensions

unix {
  include(../mkspecs/common.pri)
  schemas.CONFIG += no_check_exist
  schemas.files = $${IN_PWD}/meegotouch.schemas
  schemas.path = $${M_SYSCONFDIR}/gconf/schemas
  schemas.commands = GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) gconftool-2 --makefile-install-rule $${IN_PWD}/meegotouch.schemas

  INSTALLS += schemas
}
