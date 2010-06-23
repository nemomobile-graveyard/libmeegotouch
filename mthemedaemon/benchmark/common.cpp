#include <QApplication>
#include <QList>
#include <QPair>
#include <QSize>
#include <QString>

#include "common.h"

QList<QPair<QString,QSize> > Common::getPixmapsToRequest() {
    // list of ids requested when browsing around in widgetsgallery
    QList<QPair<QString,QSize> > pixmapsToRequest;
    pixmapsToRequest << QPair<QString,QSize>("icon-m-list", QSize(30, 30))
            << QPair<QString,QSize>("icon-m-grid", QSize(30, 30))
            << QPair<QString,QSize>("icon-l-gallery", QSize(0, 0))
            << QPair<QString,QSize>("icon-l-video", QSize(30, 30))
            << QPair<QString,QSize>("icon-m-toolbar-send-sms", QSize(30, 30))
            << QPair<QString,QSize>("icon-l-contacts", QSize(30, 30))
            << QPair<QString,QSize>("duilist-singlecolumn-center-background", QSize(0, 0))
            << QPair<QString,QSize>("duislider-background-received", QSize(0, 0))
            << QPair<QString,QSize>("duilist-singlerow-center-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duinavigationbar-toolbarbutton-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-topleft-background", QSize(0, 0))
            << QPair<QString,QSize>("duiapplicationpage-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-top-background", QSize(0, 0))
            << QPair<QString,QSize>("duislider-handle-value-background", QSize(0, 0))
            << QPair<QString,QSize>("duislider-handle", QSize(64, 64))
            << QPair<QString,QSize>("icon-m-framework-home", QSize(50, 50))
            << QPair<QString,QSize>("duilist-singlecolumn-top-background", QSize(0, 0))
            << QPair<QString,QSize>("duinavigationbar-button-home-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-bottomright-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duinavigationbar-button-close-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-left-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duilist-singlerow-right-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duilist-top-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duilist-singlerow-left-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duislider-vertical-background-elapsed", QSize(0, 0))
            << QPair<QString,QSize>("duilist-right-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duilist-topright-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duilist-topright-background", QSize(0, 0))
            << QPair<QString,QSize>("duislider-background-elapsed", QSize(0, 0))
            << QPair<QString,QSize>("duilist-singlerow-left-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-bottomleft-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-left-background", QSize(0, 0))
            << QPair<QString,QSize>("duibutton-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duicontainer-content-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-center-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-bottomleft-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duislider-vertical-background-received", QSize(0, 0))
            << QPair<QString,QSize>("duilist-topleft-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duinavigationbar-toolbar-background", QSize(0, 0))
            << QPair<QString,QSize>("icon-m-menuarrow", QSize(24, 24))
            << QPair<QString,QSize>("duilist-bottomright-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-singlecolumn-top-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duicontainer-header-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duicontainer-header-background", QSize(0, 0))
            << QPair<QString,QSize>("duinavigationbar-viewmenu-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duiprogressindicator-spinner-large-on", QSize(16, 16))
            << QPair<QString,QSize>("duinavigationbar-viewmenu-background", QSize(0, 0))
            << QPair<QString,QSize>("duislider-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-singlerow-center-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-singlerow-right-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-singlecolumn-center-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duiprogressindicator-spinner-large-off", QSize(16, 16))
            << QPair<QString,QSize>("duinavigationbar-portrait-toolbar-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-single-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duislider-vertical-background", QSize(0, 0))
            << QPair<QString,QSize>("duiseparator-horizontal-background", QSize(0, 0))
            << QPair<QString,QSize>("duitextedit-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-bottom-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-bottom-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duipositionindicator-background", QSize(6, 6))
            << QPair<QString,QSize>("icon-m-framework-back", QSize(50, 50))
            << QPair<QString,QSize>("duilist-single-background", QSize(0, 0))
            << QPair<QString,QSize>("duilist-center-background-pressed", QSize(0, 0))
            << QPair<QString,QSize>("duilist-singlecolumn-bottom-background", QSize(0, 0))
            << QPair<QString,QSize>("duislider-vertical-handle-pressed", QSize(64, 64))
            << QPair<QString,QSize>("duilist-right-background", QSize(0, 0))
            << QPair<QString,QSize>("duislider-handle-pressed", QSize(64, 64))
            << QPair<QString,QSize>("duipositionindicator-indicator", QSize(6, 6))
            << QPair<QString,QSize>("duibutton-background", QSize(0, 0))
            << QPair<QString,QSize>("icon-m-framework-close", QSize(50, 50))
            << QPair<QString,QSize>("duislider-vertical-handle", QSize(64, 64))
            << QPair<QString,QSize>("duilist-singlecolumn-bottom-background-pressed", QSize(0, 0));
    return pixmapsToRequest;
}

void Common::printResults(const QString& serverType, QMap<QString, RequestInfo> answeredRequests, QMap<QString, RequestInfo> unansweredRequests)
{
    QTextStream log(stdout, QIODevice::WriteOnly | QIODevice::Text);
    log << "###Timing results for " << serverType << "###" << '\n';

    const int idWidth = 60;
    log << left << qSetFieldWidth(idWidth) << "Pixmap Id" << qSetFieldWidth(0) << "Delay" << '\n';

    QMapIterator<QString, RequestInfo> iter(answeredRequests);
    while (iter.hasNext()) {
        iter.next();
        log << qSetFieldWidth(idWidth) << iter.key() << qSetFieldWidth(0) << iter.value().delay() << "ms" << '\n';
    }

    if (!unansweredRequests.isEmpty()) {
        log << '\n';
        log << "The following pixmap requests have not been answered by the themedaemon: ";
        QMapIterator<QString, RequestInfo> iter(unansweredRequests);
        while (iter.hasNext()) {
            iter.next();
            log << iter.key() << " ";
        }
    }
    log << '\n';
    log.flush();
}
