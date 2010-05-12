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

#ifndef LABELPAGE_H
#define LABELPAGE_H

#include "templatepage.h"

class MLabel;
class MTextEdit;
class MLabelHighlighter;

class LabelPage : public TemplatePage
{
    Q_OBJECT
public:
    LabelPage();
    virtual ~LabelPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    virtual void retranslateUi();

protected slots:
    void emailClicked(const QString &link);
    void emailLongPressed(const QString &link);
    void phoneNumberClicked(const QString &link);
    void phoneNumberLongPressed(const QString &link);
    void urlClicked(const QString &link);
    void urlLongPressed(const QString &link);

private:
    MLabel *simpleLabel;
    MLabel *richLabel;

    MLabelHighlighter *phoneHighlighter;
    MLabelHighlighter *urlHighlighter;
    MLabelHighlighter *emailHighlighter;
    MLabelHighlighter *commonHighlighter;
};

#endif // LABELPAGE_H
