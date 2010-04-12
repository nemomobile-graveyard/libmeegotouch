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

#ifndef LISTPAGE_H
#define LISTPAGE_H

#include <MContainer>
#include <QHash>

#include "timedemopage.h"

class TemplatePage;
class QGraphicsLayoutItem;
class MGridLayoutPolicy;
class MLinearLayoutPolicy;
class MLabel;
class MButton;
class QSettings;

class MyContainer : public MContainer
{
public:
    MyContainer(QGraphicsItem *parent = 0);
    virtual ~MyContainer();

    MGridLayoutPolicy  *landscapePolicy;
    MLinearLayoutPolicy  *portraitPolicy;

    void addItem(QGraphicsLayoutItem *button);

};

class ListPage : public TimedemoPage
{
    Q_OBJECT

public:
    ListPage(const QString &title = "");
    virtual ~ListPage();
    virtual QString timedemoTitle();

    virtual void createContent();

    void addPage(TemplatePage *page);
    int pageCount() const;
    TemplatePage *findPageByIndex(int index) const;
    TemplatePage *findPageByTimedemoTitle(const QString& title) const;
    void showPageByTimedemoTitle(const QString& name);

    static MGridLayoutPolicy *createAndSetupGridPolicy(MWidget *panel);

    void setInitialPageToShow(const QString& initialPageToShow);

    QList<TemplatePage *> pages;

public slots:
    void handleListItemClick();
    void showThemeSelectionDialog();
    void showPageByIndex(int index);
    void showOrientationSelectionDialog();
    void toggleFps();
    void showInitialPage();

protected:
    virtual void retranslateUi();

private:
    void populateLayout();
    void showPage(MApplicationPage *page);

private:
    QList<MLabel *> groupLabels;
    QList<MContainer *> containers;

    MApplicationPage *shownPage;
    MLinearLayoutPolicy *policy;

    QHash<MButton *, TemplatePage *> buttons;

    QString initialPageToShow;
};

#endif
