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

#include "mlabelhighlighter.h"
#include "mlabelhighlighter_p.h"
#include <QHash>

namespace {
    //FIXME: Fake private functionality to avoid ABI break. Implement this with proper one
    //when ABI breaks are allowed

    typedef QHash<const MLabelHighlighter*, MLabelHighlighterPrivate*> MLabelHighlighterHash;
    Q_GLOBAL_STATIC(MLabelHighlighterHash, d_func)

    static MLabelHighlighterPrivate* d(const MLabelHighlighter* q)
    {
        MLabelHighlighterPrivate* ret = d_func()->value(q, 0);
        if (!ret) {
            ret = new MLabelHighlighterPrivate;
            d_func()->insert(q, ret);
        }
        return ret;
    }

    static void delete_d(const MLabelHighlighter* q)
    {
        MLabelHighlighterPrivate* ret = d_func()->value(q, 0);
        delete ret;
        d_func()->remove(q);
    }
}

MLabelHighlighterPrivate::MLabelHighlighterPrivate() :
    ignoreClickEvents(false), ignoreLongPressEvents(false)
{
}

MLabelHighlighterPrivate::~MLabelHighlighterPrivate()
{
}

class MCommonLabelHighlighterPrivate
{
public:
    QRegExp m_regExp;
};

MLabelHighlighter::~MLabelHighlighter()
{
    delete_d(this);
}

void MLabelHighlighter::setIgnoreClickAndLongPressEvents(bool ignore)
{
    setIgnoreClickEvents(ignore);
    setIgnoreLongPressEvents(ignore);
}

bool MLabelHighlighter::ignoreClickAndLongPressEvents() const
{
    return isIgnoringClickEvents() && isIgnoringLongPressEvents();
}

bool MLabelHighlighter::isIgnoringClickEvents() const
{
    return d(this)->ignoreClickEvents;
}

void MLabelHighlighter::setIgnoreClickEvents(bool ignore)
{
    d(this)->ignoreClickEvents = ignore;
}

bool MLabelHighlighter::isIgnoringLongPressEvents() const
{
    return d(this)->ignoreLongPressEvents;
}

void MLabelHighlighter::setIgnoreLongPressEvents(bool ignore)
{
     d(this)->ignoreLongPressEvents = ignore;
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
