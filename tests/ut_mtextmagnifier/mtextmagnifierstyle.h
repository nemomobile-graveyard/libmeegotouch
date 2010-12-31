#ifndef MTEXTMAGNIFIERSTYLE_H
#define MTEXTMAGNIFIERSTYLE_H

#include <MWidgetStyle>

class QString;
class MWidgetController;

class MTextMagnifierStyle
{
public:
    qreal magnification() const { return 0.0; }
    QPixmap *magnifierMask() const { return 0; }
    QPixmap *magnifierFrame() const { return 0; }
    QPointF visualOffset() const { return QPointF(); }

}static gMagnifierStyle;

class MTextMagnifierStyleContainer : public MWidgetStyleContainer
{
public:
    void initialize(const QString &, const QString &, const MWidgetController *){}
    void setModeDefault() const {}
    const MTextMagnifierStyle* operator->() const
    {
        return &gMagnifierStyle;
    }
};


#endif // MTEXTMAGNIFIERSTYLE_H
