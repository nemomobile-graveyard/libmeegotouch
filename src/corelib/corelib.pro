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

include(../common_bot.pri)
