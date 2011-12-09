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

#ifndef CLIPBOARDPAGE_H
#define CLIPBOARDPAGE_H

#include "templatepage.h"

class ClipboardPage: public TemplatePage
{
public:
    ClipboardPage();
    virtual ~ClipboardPage();

    virtual void createContent();
    virtual QString timedemoTitle();

protected:
    virtual void retranslateUi();
};

#endif
