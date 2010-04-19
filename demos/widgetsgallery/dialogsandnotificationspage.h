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

#ifndef DIALOGSANDNOTIFICATIONSPAGE_H
#define DIALOGSANDNOTIFICATIONSPAGE_H

#include "templatepage.h"

class MGridLayoutPolicy;
class MDialog;
class MButton;
class MLabel;
class MContainer;

class DialogsAndNotificationsPage : public TemplatePage
{
    Q_OBJECT
public:
    DialogsAndNotificationsPage();
    virtual ~DialogsAndNotificationsPage();
    virtual QString timedemoTitle();

    virtual void createContent();

protected:
    virtual void retranslateUi();

private slots:
    void openQuestionDialog();
    void openEntryDialog();
    void openLongDialog();
    void openStackedDialogs();
    void openNestedDialog();
    void openNestedMessageBox();
    void openSystemModalDialog();
    void openDialogWithProgressIndicator();
    void openMessageBox();
    void showEventBanner();
    void showInformationBanner();
    void showSystemInformationBanner();
    void setDialogProgressIndicatorVisible(bool);

private:
    void populateLayout(MLinearLayoutPolicy *layoutPolicy);

    MDialog *dialog;
    MDialog *nestedDialog;
    MDialog *nestedMessageBox;

    MLabel *label1;
    MLabel *label2;
    MLabel *label3;

    MContainer *dialogsContainer;
    MContainer *notificationsContainer;

    MButton *button1;
    MButton *button2;
    MButton *button3;
    MButton *button4;
    MButton *button5;
    MButton *button6;
    MButton *button7;

    MButton *buttonStackedDialogs;
    MButton *buttonSystemModalDialog;
    MButton *buttonDialogWithProgressIndicator;
};

#endif // DIALOGSANDNOTIFICATIONSPAGE_H
