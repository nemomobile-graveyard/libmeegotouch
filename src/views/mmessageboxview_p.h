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

#ifndef MMESSAGEBOXVIEW_P_H
#define MMESSAGEBOXVIEW_P_H

#include "mdialogview_p.h"
#include "mmessageboxview.h"

class MImageWidget;
class MLabel;

class MMessageBoxViewPrivate : public MDialogViewPrivate
{
public:
    MMessageBoxViewPrivate();
    virtual ~MMessageBoxViewPrivate();

    void prepareLayout();
    void updateLayout();
    void clearLayout();

    void updateIconWidget();
    void updateTitleWidget();
    void updateTextWidget();

    MImageWidget *iconImageWidget();
    MLabel *titleLabelWidget();
    MLabel *textLabelWidget();

    MImageWidget *iconImage;
    MLabel *titleLabel;
    MLabel *textLabel;

    MMessageBox *controller;

private:
    Q_DECLARE_PUBLIC(MMessageBoxView)
};

#endif

