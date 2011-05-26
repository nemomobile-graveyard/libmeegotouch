/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MBUTTONTABTRANSITION_H
#define MBUTTONTABTRANSITION_H

#include "mbuttondefaulttransition.h"
#include <QObject>
/*! \internal */

/*
  this transition works tha same way as default transition, except that when
  a button is in checked state and it is clicked it does not transition to
  pressed state.
*/
class MButtonTabTransition : public MButtonDefaultTransition
{
    Q_OBJECT
public:
    MButtonTabTransition(MButtonStyleContainer& s,
			     MButtonModel* m,
			     MWidgetController* c,
			     MButtonViewPrivate* p);
    virtual ~MButtonTabTransition();

    virtual void onPress();
};

/*! \internal_end */

#endif
