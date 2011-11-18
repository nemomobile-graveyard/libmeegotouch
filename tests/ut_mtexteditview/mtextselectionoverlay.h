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
        return QRegion();
    }

Q_SIGNALS:
    void selectionHandlePressed(const QPointF &);
    void selectionHandleMoved(const QPointF &);
    void selectionHandleReleased(const QPointF &);

private Q_SLOTS:
    void onSelectionChange(int newAnchor, const QRectF &, bool,
                           int newCursor, const QRectF &, bool)
    {
        if (newAnchor != newCursor) {
            show();
        } else {
            hide();
        }
    };

};

//! \internal_end

#endif

