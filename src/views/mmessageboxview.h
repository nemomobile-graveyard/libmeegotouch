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

#ifndef MMESSAGEBOXVIEW_H
#define MMESSAGEBOXVIEW_H

#include "mdialogview.h"
#include "mmessageboxmodel.h"
#include <mmessageboxstyle.h>

class MMessageBox;
class MMessageBoxViewPrivate;

class M_VIEWS_EXPORT MMessageBoxView : public MDialogView
{
    Q_OBJECT
    M_VIEW(MMessageBoxModel, MMessageBoxStyle)

public:
    MMessageBoxView(MMessageBox *controller);
    virtual ~MMessageBoxView();

protected:
    //! \reimp
    virtual void setupModel();
    //! \reimp_end

protected Q_SLOTS:
    virtual void updateData(const QList<const char *>& modifications);
    virtual void applyStyle();

private:
    Q_DECLARE_PRIVATE(MMessageBoxView)
};

#endif

