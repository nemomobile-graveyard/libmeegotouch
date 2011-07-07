#include "msnapshotitem.h"

#include <mgraphicssystemhelper.h>
#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QGraphicsView>
#include <QGLFramebufferObject>

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
#include <QtMeeGoGraphicsSystemHelper>
#endif

MSnapshotItem::MSnapshotItem(const QRectF &sceneTargetRect, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_boundingRect(sceneTargetRect), pixmap(), framebufferObject(0)
{
}

MSnapshotItem::~MSnapshotItem()
{
    delete framebufferObject;
    framebufferObject = 0;
}

QRectF MSnapshotItem::boundingRect() const
{
    return m_boundingRect;
}

void MSnapshotItem::updateSnapshot()
{
    pixmap = QPixmap();
    delete framebufferObject;
    framebufferObject = 0;

    if (scene() && scene()->views().count() == 0)
        return;

    QGraphicsView *graphicsView = scene()->views().at(0);
    Q_ASSERT(graphicsView);

    bool grabContent = true;
    if (MGraphicsSystemHelper::isRunningMeeGoGraphicsSystem()) {
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
        graphicsView->installEventFilter(this);
#endif
        const QRect rect = m_boundingRect.toRect();
        framebufferObject = new QGLFramebufferObject(rect.size());
        if (framebufferObject->isValid()) {
            QPainter painter(framebufferObject);
            graphicsView->render(&painter, QRectF(), rect);
            grabContent = false;
        }
    }

    if (grabContent) {
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
        pixmap = QPixmap::grabWidget(graphicsView);
#else
        pixmap = QPixmap::grabWindow(graphicsView->effectiveWinId());
#endif
    }
}

void MSnapshotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (framebufferObject && framebufferObject->isValid()) {
        framebufferObject->drawTexture(m_boundingRect, framebufferObject->texture());
    } else if (!pixmap.isNull()) {
        painter->drawPixmap(0,0, pixmap);
    }
}

#ifdef HAVE_MEEGOGRAPHICSSYSTEM
bool MSnapshotItem::eventFilter(QObject *obj, QEvent *event)
{
    QMeeGoSwitchEvent* switchEvent = dynamic_cast<QMeeGoSwitchEvent*>(event);
    if (switchEvent && switchEvent->state() == QMeeGoSwitchEvent::WillSwitch) {
        delete framebufferObject;
        framebufferObject = 0;
    }

    return QGraphicsObject::eventFilter(obj, event);
}
#endif
