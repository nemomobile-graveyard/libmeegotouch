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

#ifndef MLISTHEADER_P_H
#define MLISTHEADER_P_H

#include <QObject>

class MLabel;
class MListHeader;
class QGraphicsLinearLayout;
class MSeparator;

class MListHeaderPrivate
{
public:
    MListHeaderPrivate();
    virtual ~MListHeaderPrivate();

    void init();

protected:
    MLabel *titleLabelWidget();
    MSeparator *separatorWidget();

private:
    Q_DECLARE_PUBLIC(MListHeader)
    MListHeader *q_ptr;

    QGraphicsLinearLayout *layout;
    MLabel *titleLabel;
    MSeparator *separator;
};

#endif // MLISTHEADER_P_H
