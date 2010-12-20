#ifndef SCROLLER_H
#define SCROLLER_H

#include <mabstractscroller.h>

class ScrollableWidget;

class Scroller : public MAbstractScroller
{
public:
    Scroller();
    virtual ~Scroller();

    virtual QPoint queryScrollingAmount(const QGraphicsWidget *widget,
                                        const QRect &targetRect,
                                        const QPoint &originPoint,
                                        const QPoint &currentOffset);
    virtual void applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset);
    virtual void restoreScrolling(QGraphicsWidget *widget);

    static QPoint maximumOffset; // absolute offset values
};

#endif // SCROLLER_H
