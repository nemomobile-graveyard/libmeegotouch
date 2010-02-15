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

#ifndef DUIDIALOG_P_H
#define DUIDIALOG_P_H

#include "duiscenewindow_p.h"
#include "duidialog.h"

class QSignalMapper;
class DuiHomeButtonPanel;
class DuiWindow;

class DuiDialogPrivate : public DuiSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(DuiDialog)

public:

    DuiDialogPrivate();
    ~DuiDialogPrivate();
    void init();
    void appear(bool now, DuiSceneWindow::DeletionPolicy policy);
    void addStandardButtons(Dui::StandardButtons standardButtons);
    void updateStandAloneHomeButtonVisibility();
    void _q_onStandAloneDialogHidden();
    void _q_buttonClicked(QObject *obj);
    bool prepareStandAloneAppearance(DuiSceneWindow::DeletionPolicy policy);
    void updateButtonClickedMappings();
    int  resultFromStandardButtonId(int buttonId);

    DuiButtonModel *clickedButton;

    QSignalMapper *buttonClickedMapper;

    // When some calls DuiDialog::setLayout() it effectively cripples the dialog
    // by removing all its contents. After such call we cannot rely on anything
    // anymore and have to behave as a plain, empty, scene window.
    bool dumbMode;

    // Objects used to display the dialog in stand-alone mode (inside a separate
    // top-level DuiWindow)
    DuiWindow *standAloneWindow;
    DuiHomeButtonPanel *homeButtonPanel;
    bool suicideAfterDestroyingStandAloneWindow;
};

#endif
