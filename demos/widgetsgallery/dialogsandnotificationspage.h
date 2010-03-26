/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

class DuiGridLayoutPolicy;
class DuiDialog;
class DuiButton;
class DuiLabel;
class DuiContainer;

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
    void populateLayout(DuiLinearLayoutPolicy *layoutPolicy);

    DuiDialog *dialog;
    DuiDialog *nestedDialog;
    DuiDialog *nestedMessageBox;

    DuiLabel *label1;
    DuiLabel *label2;
    DuiLabel *label3;

    DuiContainer *dialogsContainer;
    DuiContainer *notificationsContainer;

    DuiButton *button1;
    DuiButton *button2;
    DuiButton *button3;
    DuiButton *button4;
    DuiButton *button5;
    DuiButton *button6;
    DuiButton *button7;
    DuiButton *buttonStackedDialogs;
    DuiButton *buttonSystemModalDialog;
    DuiButton *buttonDialogWithProgressIndicator;
};

#endif // DIALOGSANDNOTIFICATIONSPAGE_H
