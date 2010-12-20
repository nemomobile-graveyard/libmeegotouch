#include "scroller.h"
#include "scrollablewidget.h"

QPoint Scroller::maximumOffset;

Scroller::Scroller()
{
}

Scroller::~Scroller()
{
}

QPoint Scroller::queryScrollingAmount(const QGraphicsWidget *widget,
                                      const QRect &targetRect,
                                      const QPoint &originPoint,
                                      const QPoint &currentOffset)
{
    const ScrollableWidget *scrollable = dynamic_cast<const ScrollableWidget *>(widget);
    if (!scrollable) {
        return QPoint();
    }

    const QRect boundaries(scrollable->boundingRect().toRect());

    QRect movedTargetRect(targetRect);
    moveRectInsideArea(boundaries, movedTargetRect);

    const QPoint offset(movedTargetRect.topLeft() - originPoint);
    const QPoint lowerBound(-maximumOffset - currentOffset);
    const QPoint upperBound(maximumOffset - currentOffset);
    const QPoint limitedOffset(qBound(lowerBound.x(), offset.x(), upperBound.x()),
                               qBound(lowerBound.y(), offset.y(), upperBound.y()));
    return limitedOffset;
}

void Scroller::applyScrolling(QGraphicsWidget *widget, const QPoint &contentsOffset)
{
    ScrollableWidget *scrollableWidget = dynamic_cast<ScrollableWidget *>(widget);
    if (scrollableWidget) {
        scrollableWidget->scrollContents(contentsOffset);
    }
}

void Scroller::restoreScrolling(QGraphicsWidget *widget)
{
    ScrollableWidget *scrollableWidget = dynamic_cast<ScrollableWidget *>(widget);
    if (scrollableWidget) {
        scrollableWidget->clearScroll();
    }
}
