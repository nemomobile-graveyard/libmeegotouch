#ifndef MSNAPSHOTITEM_H
#define MSNAPSHOTITEM_H

#include <QGraphicsObject>

class QGLFramebufferObject;
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

protected:
#ifdef HAVE_MEEGOGRAPHICSSYSTEM
     virtual bool eventFilter(QObject *obj, QEvent *event);
#endif

private:
    QRectF m_boundingRect;
    QPixmap pixmap;
    QGLFramebufferObject *framebufferObject;
};

#endif
