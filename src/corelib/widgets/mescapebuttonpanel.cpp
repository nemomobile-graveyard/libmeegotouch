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

#include "mescapebuttonpanel.h"
#include "mscenewindowmodel.h"
#include "mscenewindow_p.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MEscapeButtonPanel)


MEscapeButtonPanel::MEscapeButtonPanel(QGraphicsItem *parent) :
    MSceneWindow(new MSceneWindowPrivate,
                   new MEscapeButtonPanelModel,
                   MSceneWindow::EscapeButtonPanel, QString(""), parent)
{
}

MEscapeButtonPanel::~MEscapeButtonPanel()
{
}

MEscapeButtonPanelModel::EscapeMode MEscapeButtonPanel::escapeMode() const
{
    return model()->mode();
}

void MEscapeButtonPanel::setEscapeMode(MEscapeButtonPanelModel::EscapeMode escapeMode)
{
    if (escapeMode != model()->mode()) {
        model()->setMode(escapeMode);
        emit escapeModeChanged(escapeMode);
    }
}
