#ifndef MTEXTSELECTIONOVERLAY_H
#define MTEXTSELECTIONOVERLAY_H

#include <QGraphicsWidget>
#include <QPointer>
#include <QRegion>
#include <mtexteditview.h>

class MSceneManager;
class MTextEdit;

//! \internal

class MTextSelectionOverlay : public QGraphicsWidget
{
    Q_OBJECT
public:
    enum HandleType {
        Cursor,
        Anchor
    };

    explicit MTextSelectionOverlay(MTextEdit *,
                                   MTextEditView *view)
    {
        connect(view, SIGNAL(selectionChanged(int, QRectF, bool, int, QRectF, bool)),
                this, SLOT(onSelectionChange(int, QRectF, bool, int, QRectF, bool)));
    };

    void disappear()
    {
        hide();
    }

    void skipTransitions()
    {
    }

    QRegion region()
    {
        return selectionHandleRegion;
    }

Q_SIGNALS:
    void selectionHandlePressed(const QPointF &, MTextSelectionOverlay::HandleType);
    void selectionHandleMoved(const QPointF &);
    void selectionHandleReleased(const QPointF &);
    void selectionHandlesAppeared();

private Q_SLOTS:
    void onSelectionChange(int newAnchor, const QRectF &, bool,
                           int newCursor, const QRectF &, bool)
    {
        if (newAnchor != newCursor) {
            emit selectionHandlesAppeared();
            show();
        } else {
            hide();
        }
    };

public:
    QRegion selectionHandleRegion;
};

//! \internal_end

#endif

