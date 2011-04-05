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

#ifndef MDIALOG_P_H
#define MDIALOG_P_H

#include "mscenewindow_p.h"
#include "mdialog.h"

class QSignalMapper;
class MHomeButtonPanel;
class MWindow;
class MStatusBar;

class MDialogPrivate : public MSceneWindowPrivate
{
    Q_DECLARE_PUBLIC(MDialog)

public:

    MDialogPrivate();
    ~MDialogPrivate();

    void appear(MSceneWindow::DeletionPolicy policy);
    void addStandardButtons(M::StandardButtons standardButtons);
    void updateStandAloneHomeButtonVisibility();
    void _q_onStandAloneDialogDisappeared();
    void _q_buttonClicked(QObject *obj);
    void _q_onCentralWidgetDestroyed();
    bool prepareStandAloneAppearance(MSceneWindow::DeletionPolicy policy);
    void updateButtonClickedMappings();
    int  resultFromStandardButtonId(int buttonId);
    void removeSceneWindowFromStandaloneScene(MSceneWindow *sceneWindow);

    MButtonModel *clickedButton;

    QSignalMapper *buttonClickedMapper;

    // When some calls MDialog::setLayout() it effectively cripples the dialog
    // by removing all its contents. After such call we cannot rely on anything
    // anymore and have to behave as a plain, empty, scene window.
    bool dumbMode;

    // Objects used to display the dialog in stand-alone mode (inside a separate
    // top-level MWindow)
    MWindow *standAloneWindow;
    MHomeButtonPanel *homeButtonPanel;
    bool suicideAfterDestroyingStandAloneWindow;

};

#endif
