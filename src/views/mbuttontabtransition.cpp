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

#include "mbuttontabtransition.h"
#include "mbutton.h"
#include "mbuttonview.h"
#include "mbuttonview_p.h"

MButtonTabTransition::MButtonTabTransition(MButtonStyleContainer& s,
						   MButtonModel* m,
						   MWidgetController* c,
						   MButtonViewPrivate* p)
    :MButtonDefaultTransition(s,m,c,p)
{
}

MButtonTabTransition::~MButtonTabTransition()
{
}

void MButtonTabTransition::onPress()
{
    if(controller->isEnabled()) {
        if( !model->checked() ) {
            style.setModePressed();
            liftTimer.start();
            userReleased = false;
            priv->updateItemsAfterModeChange();
        }
    }
}
