/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef SHEETSPAGE_H
#define SHEETSPAGE_H

#include "templatepage.h"

#include <MTextEdit>
#include <MImageWidget>
#include <QModelIndex>

class MBasicListItem;
class MLinearLayoutPolicy;
class MList;
class MSheet;

class ClickableImageWidget : public MImageWidget
{
    Q_OBJECT

public:
    ClickableImageWidget(QGraphicsItem *parent = 0);
    virtual ~ClickableImageWidget();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void cancelEvent(MCancelEvent *event);

Q_SIGNALS:
    void clicked();

private:
    bool down;

};

class SheetsPage : public TemplatePage
{
    Q_OBJECT
public:
    SheetsPage();
    virtual QString timedemoTitle();
    virtual void createContent();

protected:
    virtual void retranslateUi();

private Q_SLOTS:
    void itemClicked(const QModelIndex &index);
    void openFullscreenPhotoSheet();
    void reparentPhotoImage();
    void triggerHeaderVisibility();
    void processFullscreenSave();
    void processFullscreenSaveSuccess();

private:
    void openLoginSheet();
    void openSystemwideSheet();
    void openLongSheet();
    void openListSheet();

    void openPhotoSheet();
    void populatePhotoSheetCentralWidget(QGraphicsWidget *centralWidget);
    void populatePhotoSheetHeader(MSheet *sheet);

    void populateFullscreenPhotoSheetCentralWidget(QGraphicsWidget *centralWidget);
    void populateFullscreenPhotoSheetHeader(MSheet *sheet);


    void populateLayout();

    MLinearLayoutPolicy *policy;
    MList *list;

    MSheet *photoSheet;
    MSheet *fullScreenPhotoSheet;
    ClickableImageWidget *sheetPhoto;
};

#endif // SHEETSPAGE_H
