#ifndef MBUTTON_IMPLODING_BACKGROUND_TRANSITION
#define MBUTTON_IMPLODING_BACKGROUND_TRANSITION

#include "mbuttontransition.h"
#include "mbuttonstyle.h"
#include <QTimeLine>
#include <QEasingCurve>


class MButtonModel;
class MWidgetController;
class MButtonViewPrivate;

class MButtonImplodingBackgroundTransition : public MButtonTransition
{
    Q_OBJECT
public:
    MButtonImplodingBackgroundTransition(MButtonStyleContainer& s,
					 MButtonModel* m,
					 MWidgetController* c,
					 MButtonViewPrivate* p);
    virtual ~MButtonImplodingBackgroundTransition();
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
    QEasingCurve inScaleCurve;
    QEasingCurve inOpacityCurve;
    QEasingCurve outScaleCurve;
    QEasingCurve outOpacityCurve;
    bool released;
private slots:
    void valueChanged(qreal v);
    void finished();
};

#endif
