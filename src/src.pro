CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS += \
    corelib \
    views \
    settings \
    extensions
win32|macx:SUBDIRS -= settings extensions

include(../mkspecs/common.pri)

contains(DEFINES, HAVE_GCONF) {
    # install gconf schema
    unix {
        schemas.CONFIG += no_check_exist
        schemas.files = $${IN_PWD}/data/meegotouch.schemas
        schemas.path = $${M_SYSCONFDIR}/gconf/schemas
        schemas.commands = GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) gconftool-2 --makefile-install-rule $${IN_PWD}/data/meegotouch.schemas
        INSTALLS += schemas
    }
}

#install device configuration file
devices.CONFIG += no_check_exist
devices.files = $${IN_PWD}/data/devices.conf
devices.path = $$M_INSTALL_SYSCONF/meegotouch
INSTALLS += devices

# install MeeGo Touch feature files
install_prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
install_prf.files = \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouch_common.prf \
    $${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouch_translations.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouchcore.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouchsettings.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouch.prf

# causes problems when installing on windows with INSTALL_ROOT
!win32 {
    INSTALLS += install_prf
}

# install Rich Text Editor xml file
install_xmltoolbar.files = data/RichTextEditorToolbar1.xml
install_xmltoolbar.path = $$M_INSTALL_PREFIX/share/meegotouch/imtoolbars
INSTALLS += install_xmltoolbar

# install pkgconfig file
install_pkgconfig.path = $$M_INSTALL_LIBS/pkgconfig
install_pkgconfig.files = \
    data/meegotouchcore.pc \
    data/meegotouchsettings.pc \
    data/meegotouch.pc

INSTALLS += install_pkgconfig

# install forwarding headers
win32 {
    # for windows we can live with the broken pattern matching
    FORWARDING_HEADERS = include/M*
} else {
    # match only the camel case forwarding headers here
    FORWARDING_HEADERS = $$system( find include/M* )
}
forwarding_headers.path = $$M_INSTALL_HEADERS
forwarding_headers.files = $$FORWARDING_HEADERS
INSTALLS += forwarding_headers
