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

#ifndef TEXTENTRYATTRIBUTEEXTENSIONPAGE_H
#define TEXTENTRYATTRIBUTEEXTENSIONPAGE_H

#include "templatepage.h"
#include <QList>

class MTextEdit;
class MLabel;
class MButton;

class TextEntryAttributeExtensionPage : public TemplatePage
{
    Q_OBJECT

public:
    TextEntryAttributeExtensionPage();
    virtual ~TextEntryAttributeExtensionPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    virtual void retranslateUi();

private slots:
    void setUseLabel(bool);
    void setUseIcon(bool);
    void setKeyHighlighted(bool);
    void setKeyDisabled(bool);

private:
    MLabel *noteLabel;
    QList<MTextEdit *> edits;
    QList<MLabel *> labels;
    MButton *pushButton1;
    MButton *pushButton2;
    MButton *pushButton3;
    MButton *pushButton4;
    MButton *pushButton5;
    MButton *pushButton6;
    MButton *pushButton7;
    MButton *pushButton8;
    int registeredAttributeExtensionId1;
    int registeredAttributeExtensionId2;
};

#endif
