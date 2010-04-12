/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef TEMPLATEPAGE_H
#define TEMPLATEPAGE_H

#include "timedemopage.h"

#include <MApplicationPage>

class MLayout;
class MLinearLayoutPolicy;
class MWidget;
class MButton;
class MSeparator;
class MLabel;
class QGraphicsLinearLayout;

class TemplatePage  : public TimedemoPage
{
    Q_OBJECT

public:
    enum GroupID {
        ViewsAndDialogs = 0,
        ContainersAndApplets,
        LayoutsAndVisuals,
        Buttons,
        IndicatorsAndNotifications,
        ListsGridsAndMenus,
        UserInput,
        InputFeedback
    };

    TemplatePage();
    virtual ~TemplatePage();

    int groupID();
    static QStringList groupNames();

    virtual void createContent();

protected:
    virtual void createLayout();
    virtual void retranslateUi();

    int gid;

    MLayout              *layout;

    MLinearLayoutPolicy  *landscapePolicy;
    MLinearLayoutPolicy  *portraitPolicy;

    MWidget              *container;

    MSeparator           *separator;
    MLabel               *infoLabel;

    MLayout              *containerLayout;
    MLinearLayoutPolicy  *containerPolicy;
};

#endif
