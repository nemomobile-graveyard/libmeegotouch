#ifndef SCROLLER_H
#define SCROLLER_H

#include <mabstractscroller.h>
#include <QEasingCurve>

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
    virtual void applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset,
                                int duration, const QEasingCurve &easingCurve);
    virtual void restoreScrolling(QGraphicsWidget *widget);

    static QPoint maximumOffset; // absolute offset values

    int lastScrollDuration;
    QEasingCurve lastEasingCurve;
};

#endif // SCROLLER_H
