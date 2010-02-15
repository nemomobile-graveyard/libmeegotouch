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

#include <QSettings>
#include "duibuttongroup.h"
#include "duibutton.h"
#include "duibutton_p.h"
#include "duitheme.h"

#include "duiwidgetcreator.h"
DUI_REGISTER_WIDGET(DuiButton)

const DuiTheme::ViewType DuiButton::toggleType      = "toggle";
const DuiTheme::ViewType DuiButton::checkboxType    = "checkbox";
const DuiTheme::ViewType DuiButton::iconType        = "icon";
const DuiTheme::ViewType DuiButton::switchType      = "switch";

DuiButtonPrivate::DuiButtonPrivate()
    : buttonGroup(0)
{
}

DuiButtonPrivate::~DuiButtonPrivate()
{
}

void DuiButtonPrivate::init()
{
    Q_Q(DuiButton);
    q->setFocusPolicy(Qt::ClickFocus);
}

void DuiButtonPrivate::init(const QString &newIconID, const QString &newText)
{
    Q_Q(DuiButton);
    init();

    q->setText(newText);
    q->setIconID(newIconID);
}

DuiButton::DuiButton(DuiButtonPrivate *dd, DuiButtonModel *model, QGraphicsItem *parent)
    : DuiWidgetController(dd, model, parent)
{
    Q_D(DuiButton);

    d->init();
}

DuiButton::DuiButton(QGraphicsItem *parent, DuiButtonModel *model)
    : DuiWidgetController(new DuiButtonPrivate, model == NULL ? new DuiButtonModel : model, parent)
{
    Q_D(DuiButton);

    d->init();
}

DuiButton::DuiButton(const QString &text, QGraphicsItem *parent, DuiButtonModel *model)
    : DuiWidgetController(new DuiButtonPrivate, model == NULL ? new DuiButtonModel : model, parent)
{
    Q_D(DuiButton);

    d->init(QString(), text);
}

DuiButton::DuiButton(const QString &iconID, const QString &text, QGraphicsItem *parent, DuiButtonModel *model)
    : DuiWidgetController(new DuiButtonPrivate, model == NULL ? new DuiButtonModel : model, parent)
{
    Q_D(DuiButton);

    d->init(iconID, text);
}

DuiButton::~DuiButton()
{
    Q_D(DuiButton);

    if (d->buttonGroup) {
        d->buttonGroup->removeButton(this);
    }
}

void DuiButton::setIconID(const QString &iconID)
{
    model()->setIconID(iconID);
}


QString DuiButton::iconID() const
{
    return model()->iconID();
}

void DuiButton::setToggledIconID(const QString &toggledIconID)
{
    model()->setToggledIconID(toggledIconID);
}

QString DuiButton::toggledIconID() const
{
    return model()->toggledIconID();
}

void DuiButton::setText(const QString &text)
{
    model()->setText(text);
}


QString DuiButton::text() const
{
    return model()->text();
}

bool DuiButton::isTextVisible() const
{
    return model()->textVisible();
}

void DuiButton::setTextVisible(bool textVisible)
{
    model()->setTextVisible(textVisible);
}

bool DuiButton::isIconVisible() const
{
    return model()->iconVisible();
}

void DuiButton::setIconVisible(bool iconVisible)
{
    model()->setIconVisible(iconVisible);
}

bool DuiButton::isCheckable() const
{
    return model()->checkable();
}

void DuiButton::setCheckable(bool buttonCheckable)
{
    if (!buttonCheckable && model()->checked())
        model()->setChecked(false);
    model()->setCheckable(buttonCheckable);
}

bool DuiButton::isChecked() const
{
    return model()->checked();
}

//TODO Remove this when delayed model initialization bug is properly fixed.
static bool toggleEmitted = false;

void DuiButton::setChecked(bool buttonChecked)
{
    // FIXME: these checks could be done in model side, the group needs moved to model first
    if (isCheckable() && buttonChecked != isChecked()) {

        /* The active button in an exclusive group cannot be deselected */
        if (group() && group()->exclusive() && group()->checkedButton() == this) {
            return;
        }

        //TODO This is here just because of the delayed model initialization, this
        //     call should be removed when the delayed model initialization bug/feature
        //     is properly fixed.
        //
        //     The bug causes invalid functionaliy for buttons inside a buttongroup,
        //     if state of a button(s) is changed before messageloop is run first
        //     time. The updateData() slot does does not get called and toggled
        //     signal is not emitted. Exclusive buttongroup uses the toggled signal
        //     to uncheck buttons.
        bool shouldEmit = (model()->checked() != buttonChecked);
        toggleEmitted = false;

        model()->setChecked(buttonChecked);

        //TODO Remove this when delayed model initialization bug is properly fixed.
        if (shouldEmit && !toggleEmitted)
            emit toggled(model()->checked());
    }
}

bool DuiButton::isDown() const
{
    return model()->down();
}

void DuiButton::setDown(bool status)
{
    model()->setDown(status);
}

void DuiButton::click()
{
    model()->setDown(true);
    model()->setDown(false);
    modelClick();
}

void DuiButton::toggle()
{
    setChecked(!isChecked());
}

DuiButtonGroup *DuiButton::group() const
{
    Q_D(const DuiButton);
    return d->buttonGroup;
}

void DuiButton::setGroup(DuiButtonGroup *group)
{
    Q_D(DuiButton);

    d->buttonGroup = group;
}

void DuiButton::setupModel()
{
    DuiWidgetController::setupModel();

    connect(model(), SIGNAL(clicked()), SLOT(modelClick()));
}

void DuiButton::updateData(const QList<const char *>& modifications)
{
    DuiWidgetController::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == DuiButtonModel::Checked) {
            emit toggled(model()->checked());

            //TODO Remove this when delayed model initialization bug is properly fixed.
            toggleEmitted = true;
        } else if (member == DuiButtonModel::Down) {
            if (model()->down())
                emit pressed();
            else
                emit released();
        }
    }
}

void DuiButton::modelClick()
{
    toggle();
    emit clicked(isChecked());
}

