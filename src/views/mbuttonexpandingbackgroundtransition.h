#ifndef MBUTTON_EXPANDING_BACKGROUND_TRANSITION
#define MBUTTON_EXPANDING_BACKGROUND_TRANSITION

#include "mbuttontransition.h"
#include "mbuttonstyle.h"
#include <QTimeLine>
#include <QEasingCurve>


class MButtonModel;
class MWidgetController;
class MButtonViewPrivate;

class MButtonExpandingBackgroundTransition : public MButtonTransition
{
    Q_OBJECT
public:
    MButtonExpandingBackgroundTransition(MButtonStyleContainer& s,
					 MButtonModel* m,
					 MWidgetController* c,
					 MButtonViewPrivate* p);
    virtual ~MButtonExpandingBackgroundTransition();
    virtual void onPress();
    virtual void onRelease();
    virtual void onCancel();
    virtual void modifyBackgroundPainter(QPainter *painter) const;
    virtual void refreshStyle();
private:
    MButtonStyleContainer& style;
    MButtonModel* model;
    MWidgetController* controller;
    MButtonViewPrivate* priv;
    QTimeLine timeline;
    qreal scale;
    qreal opacity;
    QEasingCurve scaleCurve;
    QEasingCurve opacityCurve;
private slots:
    void valueChanged(qreal v);
    void finished();
};

#endif
