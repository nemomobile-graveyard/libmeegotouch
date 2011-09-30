/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
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

protected:
    MButtonStyleContainer& style;
    MButtonModel* model;
    MWidgetController* controller;
    MButtonViewPrivate* priv;
    QTimer liftTimer;
    QTimer stayUpTimer;
    bool userReleased;

protected slots:
    virtual void liftButton();
    virtual void pressButton();
};

/*! \internal_end */

#endif
