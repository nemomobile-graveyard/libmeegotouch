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
#ifndef MOBJECTMENUBASICVIEW_H
#define MOBJECTMENUBASICVIEW_H

#include "mscenewindowview.h"
#include "mobjectmenumodel.h"
#include "mobjectmenubasicstyle.h"
#include <mnamespace.h>
#ifdef HAVE_CONTENTACTION
#include <contentaction/contentaction.h>
#endif

class MObjectMenu;
class MButton;
class MAction;
class MLayout;
class MLinearLayoutPolicy;
class MGridLayoutPolicy;

class MObjectMenuBasicView : public MSceneWindowView
{
    Q_OBJECT
    Q_DISABLE_COPY(MObjectMenuBasicView)
    M_VIEW(MObjectMenuModel, MObjectMenuBasicStyle)

public:
    MObjectMenuBasicView(MObjectMenu *controller);
    virtual ~MObjectMenuBasicView();

protected:

    //! \reimp
    virtual void setupModel();
    virtual void updateData(const QList<const char *> &modifications);
    //! \reimp_end

protected slots:
    void actionAdded(MAction *action);
    void actionModified(MAction *action);
    void actionRemoved(MAction *action);
private slots:
    void contentActionTriggered();

private:


#ifdef HAVE_CONTENTACTION
    QHash<MAction*, ContentAction::Action> contentActions;
#endif
    QHash<MAction *, MButton *> buttons;

    MLayout *layout;
    MLinearLayoutPolicy *portraitPolicy;
    MGridLayoutPolicy *landscapePolicy;

    MObjectMenu *controller;
};

#endif
