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

#ifndef MCOLORCOMBOBOXVIEW_P_H
#define MCOLORXOMBOBOXVIEW_P_H

#include <QPointer>
#include <MDialog>

class QString;
class MLayout;
class MColorList;
class MColorComboBox;
class MColorComboBoxView;
class MGridLayoutPolicy;
class MColorComboBoxButton;

//! \internal

/**
 * \brief Private part of MColorComboBoxView.
 */
class MColorComboBoxViewPrivate
{
    MColorComboBoxView *q_ptr;
    Q_DISABLE_COPY(MColorComboBoxViewPrivate)
    Q_DECLARE_PUBLIC(MColorComboBoxView)

public:

    MColorComboBoxViewPrivate(MColorComboBoxView *q, MColorComboBox *controller);

    ~MColorComboBoxViewPrivate();

    void init();

    void updateTitle(const QString &title);

    MColorComboBoxButton *buttonWidget();

    MColorList *colorWidget();

    MDialog *popupWidget();

    void _q_showPopup();

    void _q_colorPicked();

    void _q_popupDisappeared();

private:

    MColorComboBox *controller;

    MColorComboBoxButton *button;

    QPointer<MDialog> popup;

    MColorList *widget;

    MLayout *layout;

    MGridLayoutPolicy *policy;
};

//! \internal_end

#endif
