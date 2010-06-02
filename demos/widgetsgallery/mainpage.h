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

#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <QHash>
#include <QModelIndex>

#include "templatepage.h"

class QGraphicsLayoutItem;
class MGridLayoutPolicy;
class MLinearLayoutPolicy;
class MLabel;
class MList;
class MButton;
class QSettings;

class LanguagePage;

class MainPage : public TimedemoPage
{
    Q_OBJECT

public:
    typedef QHash<QString, TemplatePage*> TemplatePageHashMap;

    enum DataRole {
        Page = Qt::UserRole,
        PageName,
        PageTimedemoName
    };

public:
    MainPage(const QString &title = "");
    virtual ~MainPage();
    virtual QString timedemoTitle();

    virtual void createContent();

    QList<TemplatePage*> demoPages();
    TemplatePage *findPageByTimedemoTitle(const QString& title) const;
    void showPageByTimedemoTitle(const QString& name);

    void setInitialPageToShow(const QString& initialPageToShow);

public slots:
    void handleListItemClick();
    void showThemeSelectionDialog();
    void showOrientationSelectionDialog();
    void toggleFps();
    void showInitialPage();
    void showLanguageSettingsPage();

    void categoryItemClicked(const QModelIndex &index);

protected:
    virtual void retranslateUi();

private:
    void populateLayout();
    void showPage(MApplicationPage *page);

private:
    MList *list;

    MApplicationPage *shownPage;
    MLinearLayoutPolicy *policy;
    LanguagePage *languageSettingsPage;

    QHash<MButton *, TemplatePage *> buttons;

    QString initialPageToShow;
};

#endif
