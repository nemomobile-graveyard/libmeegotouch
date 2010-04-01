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

#ifndef DUIMESSAGEBOX_P_H
#define DUIMESSAGEBOX_P_H

#include "duimessagebox.h"
#include "duidialog_p.h"

class DuiMessageBoxPrivate : public DuiDialogPrivate
{
    Q_DECLARE_PUBLIC(DuiMessageBox)
public:
    DuiMessageBoxPrivate();
};

#endif
