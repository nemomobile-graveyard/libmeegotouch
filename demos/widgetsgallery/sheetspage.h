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
#include <QPointer>

class MBasicListItem;
class QGraphicsLinearLayout;
class MSheet;

class QPropertyAnimation;

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
    void openFullscreenPhotoSheet();
    void reparentPhotoImage();
    void updateFullscreenPhotoSheetGeometry();
    void triggerHeaderVisibility();
    void processFullscreenSave();
    void processFullscreenSaveSuccess();

    void openLoginSheet();
    void showSystemwideSheetPage();
    void openLongSheet();
    void openListSheet();
    void openPhotoSheet();

private:
    void populatePhotoSheetCentralWidget(QGraphicsWidget *centralWidget);
    void populatePhotoSheetHeader(MSheet *sheet);

    void populateFullscreenPhotoSheetCentralWidget(QGraphicsWidget *centralWidget);
    void populateFullscreenPhotoSheetHeader(MSheet *sheet);


    MBasicListItem *createListItemAndAddToLayout();
    void populateLayout();

    QGraphicsLinearLayout *mainLayout;

    MBasicListItem *loginSheetItem;
    MBasicListItem *systemWideItem;
    MBasicListItem *sheetFromPhotoItem;
    MBasicListItem *longSheetItem;
    MBasicListItem *listSheetItem;

    MSheet *photoSheet;
    MSheet *fullScreenPhotoSheet;
    QPointer<QPropertyAnimation> photoSheetFullScreenTransition;

    ClickableImageWidget *sheetPhoto;
};

#endif // SHEETSPAGE_H
