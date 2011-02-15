#ifndef MBUTTON_TRANSITION
#define MBUTTON_TRANSITION

#include <QObject>

/*! \internal */

class QPainter;

class MButtonTransition : public QObject
{
    Q_OBJECT
public:
    virtual void onPress() = 0;
    virtual void onRelease() = 0;
    virtual void onCancel() = 0;

    virtual void refreshStyle() = 0;

    virtual void modifyBackgroundPainter(QPainter *) const {}
};

/*! \internal_end */

#endif
