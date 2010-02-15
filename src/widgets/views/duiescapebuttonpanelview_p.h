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

#ifndef DUIESCAPEBUTTONPANELVIEW_P_H
#define DUIESCAPEBUTTONPANELVIEW_P_H

#include "duiscenewindowview_p.h"
#include "duiescapebuttonpanelmodel.h"

class DuiEscapeButtonPanel;
class DuiButton;
class QTimeLine;

class DuiEscapeButtonPanelViewPrivate : public DuiSceneWindowViewPrivate
{
    Q_DECLARE_PUBLIC(DuiEscapeButtonPanelView)

public:
    DuiEscapeButtonPanelViewPrivate();
    virtual ~DuiEscapeButtonPanelViewPrivate();

    virtual void init();

    void finalizeEscapeButtonTransition();
    void opacityChange(qreal step);

    DuiButton *backButton;
    DuiButton *closeButton;
    QTimeLine *opacityTimeLine;
    DuiEscapeButtonPanelModel::EscapeMode escapeMode;

    DuiEscapeButtonPanel *controller;
};

#endif
