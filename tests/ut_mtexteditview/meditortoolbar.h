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
    explicit MEditorToolbar(const MWidget &)
        : appeared(false) {}
    virtual ~MEditorToolbar() {}

    void setPosition(const QPointF &,
                     MEditorToolbarArrow::ArrowDirection) {}
    void setSelectionPresent(bool) {}
    bool isAppeared() const
    {
        return appeared;
    }

signals:
    void sizeChanged();

public slots:
    void appear(bool) { appeared = true; }
    void disappear() { appeared = false; }
    void disappearTemporarily() { appeared = false; }
    void removeTemporaryDisappearance() { appeared = true; }
    void setStyleName(const QString &) { }

private:
    bool appeared;
};

#endif // MEDITORTOOLBAR_H
