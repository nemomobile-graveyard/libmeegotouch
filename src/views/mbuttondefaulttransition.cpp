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
#include "mbuttondefaulttransition.h"
#include "mdebug.h"
#include "mbutton.h"
#include "mbuttonview.h"
#include "mbuttonview_p.h"

MButtonDefaultTransition::MButtonDefaultTransition(MButtonStyleContainer& s,
						   MButtonModel* m,
						   MWidgetController* c,
						   MButtonViewPrivate* p)
    :style(s),model(m),controller(c),priv(p)
{
    userReleased = false;
    liftTimer.setSingleShot(true);
    connect(&liftTimer, SIGNAL(timeout()), SLOT(liftButton()));
    liftTimer.setInterval(style->pressTimeout());

    stayUpTimer.setSingleShot(true);
    connect(&stayUpTimer, SIGNAL(timeout()), SLOT(pressButton()));
    stayUpTimer.setInterval(style->pressTimeout()>>1);
}

MButtonDefaultTransition::~MButtonDefaultTransition()
{
}

void MButtonDefaultTransition::onPress()
{
    if(controller->isEnabled()) {
	if(liftTimer.isActive()) {
	    // second press during "down" phase
	    // lift the button at once, and keep it
	    // up a while and the press down for a while again
	    liftTimer.stop();
	    stayUpTimer.start();
	    if(model->checked())
		style.setModeSelected();
	    else
		style.setModeDefault();
	    userReleased = false;
	} else if(stayUpTimer.isActive()) {
	    // we'd need longer queue, bail
	    return;
	} else {
	    // normal procedure
	    style.setModePressed();
	    liftTimer.start();
	    userReleased = false;
	}
	priv->updateItemsAfterModeChange();
    }
}

void MButtonDefaultTransition::onRelease()
{
    if(controller->isEnabled()) {
	if(liftTimer.isActive() || stayUpTimer.isActive()) {
	    // user released too soon
	    // let the timer delay the lift
	    userReleased = true;
	} else {
	    // user released after timeout, normal procedure
	    if(model->checked())
		style.setModeSelected();
	    else
		style.setModeDefault();
	    priv->updateItemsAfterModeChange();
	}
    }
}

void MButtonDefaultTransition::onCancel()
{
    if(controller->isEnabled()) {
	if(model->checked())
	    style.setModeSelected();
	else
	    style.setModeDefault();
    }
    // stop timers
    liftTimer.stop();
    stayUpTimer.stop();
    priv->updateItemsAfterModeChange();
}

void MButtonDefaultTransition::refreshStyle()
{
    if(controller->isEnabled()) {
        if(model->down()) {
            style.setModePressed();
        } else if(model->checked()) {
            style.setModeSelected();
        } else {
            style.setModeDefault();
        }
    }
    priv->updateItemsAfterModeChange();
}

void MButtonDefaultTransition::liftButton()
{
    if(userReleased)
    {
	if(controller->isEnabled()) {
	    if(model->checked())
		style.setModeSelected();
	    else
		style.setModeDefault();

	    priv->updateItemsAfterModeChange();
	    controller->update();
	}
    }
}

void MButtonDefaultTransition::pressButton()
{
    if(controller->isEnabled()) {
	style.setModePressed();
	liftTimer.start();
	priv->updateItemsAfterModeChange();
	controller->update();
    }
}
