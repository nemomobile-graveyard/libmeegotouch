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


#ifndef MLISTHEADER_H
#define MLISTHEADER_H

#include <MStylableWidget>
#include "mlistheaderstyle.h"

/*! \internal */

class MListHeaderPrivate;

class MListHeader : public MStylableWidget
{
    Q_OBJECT

    Q_PROPERTY(QString text READ text WRITE setText)

public:
    MListHeader(QGraphicsItem *parent=NULL);
    virtual ~MListHeader();

    void setText(const QString &text);
    QString text() const;

protected:
    void applyStyle();

private:
    Q_DECLARE_PRIVATE(MListHeader)
    M_STYLABLE_WIDGET(MListHeaderStyle)

    MListHeaderPrivate *d_ptr;
};

/*! \internal_end */

#endif // MLISTHEADER_H
