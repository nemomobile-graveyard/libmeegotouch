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

#ifndef TEXTENTRYPANNINGPAGE_H
#define TEXTENTRYPANNINGPAGE_H

#include "templatepage.h"
#include <mtexteditmodel.h>
#include <QList>
#include <QPointer>

class MContentItem;
class MTextEdit;

class TextEntryPanningPage : public TemplatePage
{
    Q_OBJECT

public:
    TextEntryPanningPage();
    virtual ~TextEntryPanningPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    virtual void retranslateUi();
    virtual bool sceneEventFilter(QGraphicsItem *watched, QEvent *event);

private:
    void addTextEdit(MLinearLayoutPolicy *layoutPolicy,
                     M::TextContentType contentType,
                     MTextEditModel::LineMode lineMode = MTextEditModel::SingleLine);
    void addContentItems(MLinearLayoutPolicy *layoutPolicy, int count);
    MPannableViewport *addNestedViewport(MLinearLayoutPolicy *parentLayoutPolicy);

private:
    MLabel *infoLabel;
    MButton *loseFocusOnPanButton;
    MPannableViewport *nestedViewport;
    QList< QPointer<MTextEdit> > textEdits;
    QList< QPointer<MContentItem> > contentItems;
};

#endif
