#ifndef MEDITORTOOLBAR_H
#define MEDITORTOOLBAR_H

#include <QGraphicsWidget>
class QPointF;
class MButton;
class MWidget;

class MEditorToolbar : public QGraphicsWidget
{
    Q_OBJECT
public:
    enum ToolbarPlacement {
        BelowPointOfInterest,
        AbovePointOfInterest
    };

    explicit MEditorToolbar(const MWidget &)
        : appeared(false) {}
    virtual ~MEditorToolbar() {}

    void setPosition(const QPointF &,
                     MEditorToolbar::ToolbarPlacement) {}
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
    void setStyleName(const QString &name) { styleName = name; }

public:
    QString styleName;
    bool appeared;
};

#endif // MEDITORTOOLBAR_H
