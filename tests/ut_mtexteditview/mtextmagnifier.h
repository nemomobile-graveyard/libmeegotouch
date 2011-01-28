#ifndef MTEXTMAGNIFIER_H
#define MTEXTMAGNIFIER_H

class MWidget;
class QSizeF;
class QPointF;

// Stub text magnifier
class MTextMagnifier {
public:
    MTextMagnifier(const MWidget &, const QSizeF &)
    {
    }
    void appear()
    {
    }
    void disappear()
    {
    }
    void setMagnifiedPosition(const QPointF &)
    {
    }
    bool isAppeared() const
    {
        return false;
    }
};

#endif

