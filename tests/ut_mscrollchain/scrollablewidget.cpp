#include "scrollablewidget.h"

ScrollableWidget::ScrollableWidget(QGraphicsItem *parent)
    : QGraphicsWidget(parent),
      mContentItem(0)
{
    // Not really needed but illustrates the use of this widget.
    setFlag(QGraphicsItem::ItemClipsChildrenToShape, true);
}
ScrollableWidget::~ScrollableWidget() {}

void ScrollableWidget::setContentItem(QGraphicsItem *item)
{
    delete mContentItem;
    mContentItem = item;
    item->setParentItem(this);
    item->setPos(QPointF());
}

const QGraphicsItem *ScrollableWidget::contentItem() const
{
    return mContentItem;
}

void ScrollableWidget::scrollContents(const QPoint &offset)
{
    if (mContentItem) {
        mContentItem->setPos(mContentItem->pos() + offset);
    }
}

void ScrollableWidget::clearScroll()
{
    if (mContentItem) {
        mContentItem->setPos(QPointF());
    }
}

