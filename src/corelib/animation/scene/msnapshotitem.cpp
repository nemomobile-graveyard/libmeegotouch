#include "msnapshotitem.h"

#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QGraphicsView>

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
    if (scene() && scene()->views().count() > 0)
#if defined(Q_WS_MAC) || defined(Q_WS_WIN)
        pixmap = QPixmap::grabWidget(scene()->views().at(0));
#else
        pixmap = QPixmap::grabWindow(scene()->views().at(0)->effectiveWinId());
#endif
}

void MSnapshotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(0,0, pixmap);
}
