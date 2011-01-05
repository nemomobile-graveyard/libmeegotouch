include(../common_top.pri)

TARGET = meegotouchcore

DEFINES += M_BUILD_CORE

INCLUDEPATH += .

include(animation/animation.pri)
include(core/core.pri)
include(effects/effects.pri)
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
contains(DEFINES, HAVE_DBUS) {
    include(help/help.pri)
    include(servicefwif/servicefwif.pri)
    include(notification/notification.pri)
}

include(../common_bot.pri)
