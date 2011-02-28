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

#include "mcolorcomboboxview.h"
#include "mcolorcombobox.h"
#include "mcolorlist.h"
#include "mcolorlistview.h"
#include "mcolorcomboboxbutton.h"

#include "mlayout.h"
#include "mdialog.h"
#include "mgridlayoutpolicy.h"

#include <QString>

M_REGISTER_VIEW_NEW(MColorComboBoxView, MColorComboBox)

MColorComboBoxViewPrivate::MColorComboBoxViewPrivate(MColorComboBoxView *q, MColorComboBox *controller)
    :q_ptr(q),
    controller(controller),
    button(0),
    popup(0),
    widget(0),
    layout(0),
    policy(0)
{
}

MColorComboBoxViewPrivate::~MColorComboBoxViewPrivate()
{
    delete popupWidget();
}

void MColorComboBoxViewPrivate::init()
{
    layout = new MLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    controller->setLayout(layout);

    policy = new MGridLayoutPolicy(layout);
    policy->setContentsMargins(0, 0, 0, 0);
    policy->setSpacing(0);
    policy->addItem(buttonWidget(),0,0);
    layout->setPolicy(policy);
}

void MColorComboBoxViewPrivate::updateTitle(const QString &title)
{
    buttonWidget()->setTitle(title);
    popupWidget()->setTitle(title);
}

MColorComboBoxButton *MColorComboBoxViewPrivate::buttonWidget()
{
    Q_Q(MColorComboBoxView);

    if (!button) {
        button = new MColorComboBoxButton(controller);
        QObject::connect(button, SIGNAL(clicked()), q, SLOT(_q_showPopup()));
    }
    return button;
}

MColorList *MColorComboBoxViewPrivate::colorWidget()
{
    if (!widget) {
        widget = new MColorList(popupWidget());
        widget->setView(new MColorListView(widget));
    }

    return widget;
}

MDialog *MColorComboBoxViewPrivate::popupWidget()
{
    if (!popup) {
        popup = new MDialog();
        popup->setCentralWidget(colorWidget());
    }

    return popup;
}

void MColorComboBoxViewPrivate::_q_showPopup()
{
    Q_Q(MColorComboBoxView);

    popupWidget()->setTitle(q->model()->title());
    colorWidget()->setSelectedColor(q->model()->color());

    q->connect(popupWidget(), SIGNAL(disappeared()), q, SLOT(_q_popupDisappeared()));
    q->connect(colorWidget(), SIGNAL(colorPicked()), q, SLOT(_q_colorPicked()));
    popupWidget()->appear();
}

void MColorComboBoxViewPrivate::_q_colorPicked()
{
    Q_Q(MColorComboBoxView);

    q->model()->setColor(colorWidget()->selectedColor());
    popupWidget()->disappear();
}

void MColorComboBoxViewPrivate::_q_popupDisappeared()
{
    Q_Q(MColorComboBoxView);

    q->disconnect(popupWidget(), SIGNAL(disappeared()), q, SLOT(_q_popupDisappeared()));
    q->disconnect(colorWidget(), SIGNAL(colorPicked()), q, SLOT(_q_colorPicked()));
}

MColorComboBoxView::MColorComboBoxView(MColorComboBox *controller)
    :MWidgetView(controller), d_ptr(new MColorComboBoxViewPrivate(this, controller))
{
    Q_D(MColorComboBoxView);

    d->init();
}

MColorComboBoxView::~MColorComboBoxView()
{
    delete d_ptr;
}

void MColorComboBoxView::applyStyle()
{
    Q_D(MColorComboBoxView);

    MWidgetView::applyStyle();

    d->buttonWidget()->setStyleName(style()->buttonStyleName());
    d->buttonWidget()->setTitleStyleName(style()->titleStyleName());
    d->buttonWidget()->setColorStyleName(style()->colorStyleName());
}

void MColorComboBoxView::setupModel()
{
    Q_D(MColorComboBoxView);

    MWidgetView::setupModel();

    d->updateTitle(model()->title());
    d->buttonWidget()->setColor(model()->color());
}

void MColorComboBoxView::updateData(const QList<const char*> &modifications)
{
    Q_D(MColorComboBoxView);

    MWidgetView::updateData(modifications);

    const char *member;
    foreach (member, modifications) {
        if (member == MColorComboBoxModel::Title) {
            d->updateTitle(model()->title());
        } else if (member == MColorComboBoxModel::Color) {
            d->buttonWidget()->setColor(model()->color());
        }
    }
}
