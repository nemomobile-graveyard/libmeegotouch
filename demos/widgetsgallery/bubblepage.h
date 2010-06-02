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

#ifndef BUBBLEPAGE_H
#define BUBBLEPAGE_H

#include "templatepage.h"

#include <QPointer>

class MMessageBox;

class BubblePage: public TemplatePage
{
    Q_OBJECT

public:
    BubblePage();
    virtual ~BubblePage();

    virtual void createContent();
    virtual QString timedemoTitle();

protected Q_SLOTS:
    void bubbleLinkActivated(QString url);
    void speechBubbleClicked();

protected:
    void retranslateUi();
    
private:
    QPointer<MMessageBox> messageBox;
};

#endif
