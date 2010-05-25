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

#include "mlabelmodel.h"

void MLabelModel::addHighlighter(MLabelHighlighter *highlighter)
{
    QList<MLabelHighlighter *> &list = _highlighters();
    if (list.indexOf(highlighter) == -1) {
        list.append(highlighter);
        memberModified(Highlighters);
    }
}

void MLabelModel::removeHighlighter(MLabelHighlighter *highlighter)
{
    QList<MLabelHighlighter *> &list = _highlighters();
    if (list.removeOne(highlighter))
        memberModified(Highlighters);
}
