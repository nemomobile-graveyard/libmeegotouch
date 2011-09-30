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
#ifndef TEXTENTRYNAVIGATIONPAGE_H
#define TEXTENTRYNAVIGATIONPAGE_H

#include "templatepage.h"
#include <MTextEdit>
#include <MRichTextEdit>

class TextEntryNavigationPage : public TemplatePage
{
    Q_OBJECT

public:
    TextEntryNavigationPage();
    virtual ~TextEntryNavigationPage();
    virtual QString timedemoTitle();

    virtual void createContent();
};

#endif // TEXTENTRYNAVIGATIONPAGE_H
