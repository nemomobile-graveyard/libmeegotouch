CONFIG += ordered
TEMPLATE = subdirs
SUBDIRS += \
    corelib \
    views \
    settings \
    extensions \
    translations \
    preloader \

macx:SUBDIRS -= settings extensions

include(../mkspecs/common.pri)

contains(DEFINES, HAVE_ICU) {
    # only install data files if we do not use the ones from libmlocale
    !contains(DEFINES, HAVE_LIBMLOCALE) {
        SUBDIRS += icu-extradata
    }
}

#install device configuration file
devices.CONFIG += no_check_exist
devices.files = $${IN_PWD}/data/targets/*.conf
devices.path = $${M_TARGETS_CONF_DIR}
INSTALLS += devices

# install MeeGo Touch feature files
install_prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
install_prf.files = \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouch_common.prf \
    $${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouch_translations.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouchcore.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouchsettings.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouch.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouch_mgen.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouch_mmoc.prf \
    $${M_SOURCE_TREE}/mkspecs/features/meegotouch_coverage.prf


# causes problems when installing on windows with INSTALL_ROOT
!win32 {
    INSTALLS += install_prf
}

# install pkgconfig file
install_pkgconfig.path = $$M_INSTALL_LIBS/pkgconfig
install_pkgconfig.files = \
    data/meegotouchcore.pc \
    data/meegotouchsettings.pc \
    data/meegotouch.pc \
    data/meegotouch-boostable.pc

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
