#ifndef MTEXTSELECTIONOVERLAY_H
#define MTEXTSELECTIONOVERLAY_H

#include <QGraphicsWidget>
#include <QPointer>

class MSceneManager;
class MTextEdit;
class MTextEditView;

//! \internal

class MTextSelectionOverlay : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit MTextSelectionOverlay(MTextEdit *,
                                   MTextEditView *)
    {
    };

    void disappear()
    {
    }

    void skipTransitions()
    {
    }

Q_SIGNALS:
    void anchorHandleMoved(const QPointF &position);

    void cursorHandleMoved(const QPointF &position);

    void selectionHandlePressed();

    void selectionHandleReleased();
};

//! \internal_end

#endif

