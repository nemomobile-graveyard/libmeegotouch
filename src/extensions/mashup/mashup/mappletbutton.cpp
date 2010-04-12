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

#include "mappletbutton.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MAppletButton)

MAppletButton::MAppletButton(QGraphicsItem *parent, const QString &type) : MButton(parent)
{
    setViewType(type);
    this->setObjectName("MAppletButton");
}

MAppletButton::~MAppletButton()
{
}

bool MAppletButton::initialize(const MAppletMetaData &data)
{
    if (!data.isValid()) {
        return false;
    }

    metadataFile = data.fileName();
    setText(data.name());
    setIconID(data.icon());
    return true;
}

QString MAppletButton::metadataFilename() const
{
    return metadataFile;
}
