#ifndef MSNAPSHOTITEM_H
#define MSNAPSHOTITEM_H

#include <QGraphicsObject>

class QGraphicsScene;
class QRectF;
class QImage;
class QPixmap;

class MSnapshotItem : public QGraphicsObject
{
    Q_OBJECT
public:
    MSnapshotItem(const QRectF &targetRect,
            QGraphicsItem *parent = 0);

    virtual ~MSnapshotItem();

    void updateSnapshot();

    //! \reimp
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    //! \reimp_end

private:
    QRectF m_boundingRect;
    QPixmap pixmap;
};

#endif
