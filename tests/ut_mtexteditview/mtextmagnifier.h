#ifndef MTEXTMAGNIFIER_H
#define MTEXTMAGNIFIER_H

class MWidget;
class QSizeF;
class QPointF;

// Stub text magnifier
class MTextMagnifier {
public:
    enum DeletionPolicy {
        KeepWhenDone,
        DestroyWhenDone
    };
    MTextMagnifier(const MWidget &, const QSizeF &)
    {
    }
    void appear()
    {
    }
    void disappear(DeletionPolicy)
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

