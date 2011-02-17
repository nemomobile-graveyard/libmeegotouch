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

#include "mlabelhighlighter.h"

class MCommonLabelHighlighterPrivate
{
public:
    QRegExp m_regExp;
};

MLabelHighlighter::~MLabelHighlighter()
{
}

bool MLabelHighlighter::validate(QString &item) const
{
    Q_UNUSED(item);
    return true;
}
MCommonLabelHighlighter::MCommonLabelHighlighter(const QRegExp &regExp)
    : d_ptr(new MCommonLabelHighlighterPrivate)
{
    Q_D(MCommonLabelHighlighter);
    d->m_regExp = regExp;
}

MCommonLabelHighlighter::~MCommonLabelHighlighter()
{
    delete d_ptr;
}

QRegExp MCommonLabelHighlighter::highlightExpression() const
{
    Q_D(const MCommonLabelHighlighter);
    return d->m_regExp;
}

void MCommonLabelHighlighter::click(const QString &item)
{
    emit clicked(item);
}

void MCommonLabelHighlighter::longPress(const QString &item)
{
    emit longPressed(item);
}

