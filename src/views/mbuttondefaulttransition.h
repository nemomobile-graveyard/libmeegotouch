#ifndef MBUTTON_DEFAULT_TRANSITION
#define MBUTTON_DEFAULT_TRANSITION

#include "mbuttontransition.h"
#include "mbuttonstyle.h"
#include <QTimer>

/*! \internal */

class MButtonModel;
class MWidgetController;
class MButtonViewPrivate;

/*
  this transition handles delays for the following press/release combos
  (p=press,r=release, __=button up/down, time-->)
  1) press+hold+release
         p            r
     ____              ____
         |____________|

  2) fast click (release is delayed for visual feedback)
         p  r
     ____          _______
         |________|

  3) fast click + press+hold+release
         p  r  p            r
     ____       __           ____
         |_____|  |_________|

  4) two fast clicks
         p r  p r
     ____      __          ____
         |____|  |________|
 */

class MButtonDefaultTransition : public MButtonTransition
{
    Q_OBJECT
public:
    MButtonDefaultTransition(MButtonStyleContainer& s,
			     MButtonModel* m,
			     MWidgetController* c,
			     MButtonViewPrivate* p);
    virtual ~MButtonDefaultTransition();
    virtual void onPress();
    virtual void onRelease();
    virtual void onCancel();
    virtual void refreshStyle();
private:
    MButtonStyleContainer& style;
    MButtonModel* model;
    MWidgetController* controller;
    MButtonViewPrivate* priv;
    QTimer liftTimer;
    QTimer stayUpTimer;
    bool userReleased;
private slots:
    void liftButton();
    void pressButton();
};

/*! \internal_end */

#endif
