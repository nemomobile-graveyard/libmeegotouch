#include "msnapshotitem.h"

#include <QGraphicsScene>
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>

MSnapshotItem::MSnapshotItem(const QRectF &sceneTargetRect, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_boundingRect(sceneTargetRect)
{
    pixmap = QPixmap::grabWindow(QApplication::desktop()->winId());
}

MSnapshotItem::~MSnapshotItem()
{
}

QRectF MSnapshotItem::boundingRect() const
{
    return m_boundingRect;
}

void MSnapshotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
        QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(0,0, pixmap);
}
