#ifndef SCROLLABLEWIDGET_H
#define SCROLLABLEWIDGET_H

#include <QGraphicsWidget>

class ScrollableWidget : public QGraphicsWidget
{
    Q_OBJECT
public:
    ScrollableWidget(QGraphicsItem *parent = 0);
    virtual ~ScrollableWidget();

    void setContentItem(QGraphicsItem *item);
    const QGraphicsItem *contentItem() const;

    void scrollContents(const QPoint &offset);
    void clearScroll();

private:
    QGraphicsItem *mContentItem;
};

#endif // SCROLLABLEWIDGET_H
