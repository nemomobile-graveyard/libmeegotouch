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

#include "mhelpbutton.h"
#include "mhelpbutton_p.h"
#include "mdebug.h"

#ifdef HAVE_DBUS
#include "muserguideif.h"
#endif

MHelpButtonPrivate::MHelpButtonPrivate()
    : MButtonPrivate()
{
}

MHelpButtonPrivate::~MHelpButtonPrivate()
{
}

void MHelpButtonPrivate::_q_openHelp()
{
#ifdef HAVE_DBUS
    MUserGuideIf iface;
    if (!iface.isValid()) {
        mWarning("HelpButton") << "userguide service unavailable";
        return;
    }
    bool success = iface.pageByPathChained(pageID);
    if (!success) {
        mWarning("HelpButton") << "userguide experienced an error when trying to "
            "open the page id";
    }
#endif
}

MHelpButton::MHelpButton(const QString& pageID, QGraphicsItem* parent):
    MButton(new MHelpButtonPrivate, new MButtonModel, parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // set a default icon which can be overridden in the theme
    setIconID("icon-m-common-help");

    // a default style name so that the icon can be specified in the theme
    setStyleName("HelpButton");
    setPageID(pageID);

    connect(this, SIGNAL(clicked()), this, SLOT(_q_openHelp()));
}

MHelpButton::MHelpButton(MHelpButtonPrivate *dd, MButtonModel *model,
                QGraphicsItem *parent)
    : MButton(dd, model, parent)
{
}

MHelpButton::~MHelpButton()
{
}

QString MHelpButton::pageID() const
{
    Q_D(const MHelpButton);
    return d->pageID;
}

void MHelpButton::setPageID(const QString& id)
{
    Q_D(MHelpButton);
    d->pageID = id;
}

#include "moc_mhelpbutton.cpp"