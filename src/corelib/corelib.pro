include(../common_top.pri)

TARGET = meegotouchcore

include(animation/animation.pri)
include(core/core.pri)
include(events/events.pri)
include(feedback/feedback.pri)
include(layout/layout.pri)
include(painting/painting.pri)
include(scene/scene.pri)
include(service/service.pri)
include(style/style.pri)
include(theme/theme.pri)
include(i18n/i18n.pri)
include(widgets/widgets.pri)
include(workspace/workspace.pri)
contains(DEFINES, HAVE_GSTREAMER) {
include(video/video.pri)
}
contains(DEFINES, HAVE_DBUS) {
    include(servicefwif/servicefwif.pri)
    include(notification/notification.pri)
}

contains(DEFINES, HAVE_ICU) {
    include(icu-extradata/extradata.pri)
    EXTRADATA_SOURCE=icu-extradata/extradata.txt
}


# install .prf files
install_prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
install_prf.files = $${M_SOURCE_TREE}/mkspecs/features/meegotouch.prf \
                    $${M_SOURCE_TREE}/mkspecs/features/meegotouch_translations.prf \
                    $${M_BUILD_TREE}/mkspecs/features/meegotouch_defines.prf

# causes problems when installing on windows with INSTALL_ROOT
!win32 {
    INSTALLS += install_prf
}


#install libmeegotouch .pc file
install_pkgconfig.path = $$M_INSTALL_LIBS/pkgconfig
install_pkgconfig.files = ../meegotouch.pc
INSTALLS += install_pkgconfig


# install forwarding headers
win32 {
    # for windows we can live with the broken pattern matching
    FORWARDING_HEADERS = include/M*
} else {
    # match only the camel case forwarding headers here
    FORWARDING_HEADERS = $$system( find ../include/M* )
}
forwarding_headers.path = $$M_INSTALL_HEADERS
forwarding_headers.files = $$FORWARDING_HEADERS
INSTALLS += forwarding_headers



include(../common_bot.pri)
