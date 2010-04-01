/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duilabelhighlighter.h"

class DuiCommonLabelHighlighterPrivate
{
public:
    QRegExp m_regExp;
};

DuiLabelHighlighter::~DuiLabelHighlighter()
{
}

bool DuiLabelHighlighter::validate(QString &item) const
{
    Q_UNUSED(item);
    return true;
}
DuiCommonLabelHighlighter::DuiCommonLabelHighlighter(const QRegExp &regExp)
    : d_ptr(new DuiCommonLabelHighlighterPrivate)
{
    Q_D(DuiCommonLabelHighlighter);
    d->m_regExp = regExp;
}

DuiCommonLabelHighlighter::~DuiCommonLabelHighlighter()
{
}

QRegExp DuiCommonLabelHighlighter::highlightExpression() const
{
    Q_D(const DuiCommonLabelHighlighter);
    return d->m_regExp;
}

void DuiCommonLabelHighlighter::click(const QString &item)
{
    emit clicked(item);
}

void DuiCommonLabelHighlighter::longPress(const QString &item)
{
    emit longPressed(item);
}

