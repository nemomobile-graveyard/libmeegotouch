#include "msnapshotitem.h"

#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QGraphicsView>

#if defined(HAVE_X11_XCB) && defined(HAVE_XCB_COMPOSITE)
#include <QX11Info>
#include <X11/Xlib-xcb.h>
#include <xcb/xcb.h>
#include <xcb/composite.h>
#endif

MSnapshotItem::MSnapshotItem(const QRectF &sceneTargetRect, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_boundingRect(sceneTargetRect)
{
}

MSnapshotItem::~MSnapshotItem()
{
}

QRectF MSnapshotItem::boundingRect() const
{
    return m_boundingRect;
}

void MSnapshotItem::updateSnapshot()
{
    pixmap = QPixmap();

    if (scene() && scene()->views().count() == 0)
        return;

#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
    pixmap = QPixmap::grabWidget(scene()->views().at(0));
#elif defined(HAVE_X11_XCB) && defined(HAVE_XCB_COMPOSITE)
    xcb_connection_t *c = XGetXCBConnection(QX11Info::display());
    xcb_window_t windowId = scene()->views().at(0)->effectiveWinId();
    xcb_pixmap_t x11Pixmap = xcb_generate_id(c);
    xcb_void_cookie_t cookie = xcb_composite_name_window_pixmap_checked(c, windowId, x11Pixmap);

    if (xcb_request_check(c, cookie) == NULL) {
        // We are being composited. If we use Qt's QPixmap::grabWindow() our snapshot
        // will contain not only our window but also any other window that happens
        // to be on top of it (e.g., a translucent window showing a small dialog box with
        // a translucent dimming background).
        //
        // We use a copy because we don't want the live pixmap where our window is
        // rendering to.
        pixmap = QPixmap::fromX11Pixmap(x11Pixmap).copy();
        XFreePixmap(QX11Info::display(), x11Pixmap);
    } else {
        // We are most likely not being composited. Thus Qt's QPixmap::grabWindow() will
        // work just fine.
        pixmap = QPixmap::grabWindow(windowId);
    }
#else
    pixmap = QPixmap::grabWindow(scene()->views().at(0)->effectiveWinId());
#endif
}

void MSnapshotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!pixmap.isNull())
        painter->drawPixmap(0,0, pixmap);
}
