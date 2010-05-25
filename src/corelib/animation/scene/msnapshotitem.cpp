#include "msnapshotitem.h"

#include <QGraphicsScene>
#include <QImage>
#include <QPainter>

MSnapshotItem::MSnapshotItem(QGraphicsScene *scene, const QRectF &sceneTargetRect, QGraphicsItem *parent)
    : QGraphicsObject(parent), m_boundingRect(sceneTargetRect)
{
    // Assumes that the resolution is one scene unit per pixel
    image = new QImage(sceneTargetRect.width(), sceneTargetRect.height(),
            QImage::Format_ARGB32_Premultiplied);

    QRectF targetRect;
    targetRect.setX(0.0f);
    targetRect.setY(0.0f);
    targetRect.setWidth(sceneTargetRect.width());
    targetRect.setHeight(sceneTargetRect.height());

    QRectF sourceRect;
    sourceRect = sceneTargetRect;

    QPainter painter(image);
    scene->render(&painter, targetRect, sourceRect);
}

MSnapshotItem::~MSnapshotItem()
{
    delete image;
    image = 0;
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

    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    painter->drawImage(0, 0, *image);
}
