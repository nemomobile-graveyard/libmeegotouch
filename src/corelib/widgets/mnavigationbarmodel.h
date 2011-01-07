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

#ifndef MNAVIGATIONBARMODEL_H
#define MNAVIGATIONBARMODEL_H

#include <mscenewindowmodel.h>

class MToolBar;

class M_CORE_EXPORT MNavigationBarModel : public MSceneWindowModel
{
    Q_OBJECT
    M_MODEL_INTERNAL(MNavigationBarModel)

public:
    /*!
        This enum specifies the possible modes of the escape button.
    */
    enum EscapeButtonModeEnum {
        EscapeButtonBack, /*!< The escape button is a back button.*/
        EscapeButtonClose /*!< The escape button is a close button.*/
    };

private:
    M_MODEL_PROPERTY(bool, notifyUser, NotifyUser, true, false)
    M_MODEL_PROPERTY(QString, viewMenuDescription, ViewMenuDescription, true, "Menu")
    M_MODEL_PROPERTY(QString, viewMenuIconID, ViewMenuIconID, true, QString())
    //! \deprecated do not use
    M_MODEL_PROPERTY(bool, progressIndicatorVisible, ProgressIndicatorVisible, true, false)
    M_MODEL_PROPERTY(bool, arrowIconVisible, ArrowIconVisible, true, false)
    M_MODEL_PTR_PROPERTY(MToolBar *, toolBar, ToolBar, true, 0)
    // in the line above we should have something like:
    // M_MODEL_PROPERTY(QPointer<MToolbar>, toolBar, ToolBar, true, 0)
    // but it causes a potential ABI break.
    M_MODEL_PROPERTY(MNavigationBarModel::EscapeButtonModeEnum, escapeButtonMode, EscapeButtonMode, true,
                     MNavigationBarModel::EscapeButtonClose)
    M_MODEL_PROPERTY(bool, escapeButtonEnabled, EscapeButtonEnabled, true, true)
    M_MODEL_PROPERTY(bool, escapeButtonVisible, EscapeButtonVisible, true, true)

    //! \internal
    M_MODEL_PTR_PROPERTY(QGraphicsWidget *, customContent, CustomContent, true, 0)
    //! \internal_end
};

#endif
