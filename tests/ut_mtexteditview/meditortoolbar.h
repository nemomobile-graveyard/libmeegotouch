#ifndef MEDITORTOOLBAR_H
#define MEDITORTOOLBAR_H

#include <QGraphicsWidget>
class QPointF;
class MButton;
class MWidget;

class MEditorToolbarArrow
{
public:
    enum ArrowDirection {
        ArrowUp,
        ArrowDown
    };
};

class MEditorToolbar : public QGraphicsWidget
{
    Q_OBJECT
public:
    explicit MEditorToolbar(const MWidget &) {}
    virtual ~MEditorToolbar() {}

    void setPosition(const QPointF &,
                     MEditorToolbarArrow::ArrowDirection) {}
    void setSelectionPresent(bool) {}
    bool isAppeared() const
    {
        return false;
    }
    void removeTemporaryDisappearance() {}

signals:
    void sizeChanged();

public slots:
    void appear() {}
    void disappear() {}
};

#endif // MEDITORTOOLBAR_H
