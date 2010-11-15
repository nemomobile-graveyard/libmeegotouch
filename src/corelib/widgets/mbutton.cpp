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

#include <QGraphicsSceneContextMenuEvent>
#include <QSettings>
#include "mbuttongroup.h"
#include "mbutton.h"
#include "mbutton_p.h"
#include "mtheme.h"

#include "mwidgetcreator.h"
M_REGISTER_WIDGET(MButton)

const MTheme::ViewType MButton::toggleType      = "toggle";
const MTheme::ViewType MButton::checkboxType    = "checkbox";
const MTheme::ViewType MButton::iconType        = "icon";
const MTheme::ViewType MButton::switchType      = "switch";
const MTheme::ViewType MButton::groupType       = "group";

MButtonPrivate::MButtonPrivate()
    : MWidgetControllerPrivate(),
    buttonGroup(0)
{
}

MButtonPrivate::~MButtonPrivate()
{
}

void MButtonPrivate::init()
{
}

void MButtonPrivate::init(const QString &newIconID, const QString &newText)
{
    Q_Q(MButton);
    init();

    q->setText(newText);
    q->setIconID(newIconID);
}

MButton::MButton(MButtonPrivate *dd, MButtonModel *model, QGraphicsItem *parent)
    : MWidgetController(dd, model, parent)
{
    Q_D(MButton);

    d->init();
}

MButton::MButton(QGraphicsItem *parent, MButtonModel *model)
    : MWidgetController(new MButtonPrivate, model == NULL ? new MButtonModel : model, parent)
{
    Q_D(MButton);

    d->init();
}

MButton::MButton(const QString &text, QGraphicsItem *parent, MButtonModel *model)
    : MWidgetController(new MButtonPrivate, model == NULL ? new MButtonModel : model, parent)
{
    Q_D(MButton);

    d->init(QString(), text);
}

MButton::MButton(const QString &iconID, const QString &text, QGraphicsItem *parent, MButtonModel *model)
    : MWidgetController(new MButtonPrivate, model == NULL ? new MButtonModel : model, parent)
{
    Q_D(MButton);

    d->init(iconID, text);
}

MButton::~MButton()
{
    Q_D(MButton);

    if (d->buttonGroup) {
        d->buttonGroup->removeButton(this);
    }
}

void MButton::setIconID(const QString &iconID)
{
    model()->setIconID(iconID);
}


QString MButton::iconID() const
{
    return model()->iconID();
}

void MButton::setToggledIconID(const QString &toggledIconID)
{
    model()->setToggledIconID(toggledIconID);
}

QString MButton::toggledIconID() const
{
    return model()->toggledIconID();
}

void MButton::setIcon(const QIcon &icon)
{
    model()->setIcon(icon);
}

QIcon MButton::icon() const
{
    return model()->icon();
}

void MButton::setText(const QString &text)
{
    model()->setText(text);
}

QString MButton::text() const
{
    return model()->text();
}

bool MButton::isTextVisible() const
{
    return model()->textVisible();
}

void MButton::setTextVisible(bool textVisible)
{
    model()->setTextVisible(textVisible);
}

bool MButton::isIconVisible() const
{
    return model()->iconVisible();
}

void MButton::setIconVisible(bool iconVisible)
{
    model()->setIconVisible(iconVisible);
}

bool MButton::isCheckable() const
{
    return model()->checkable();
}

void MButton::setCheckable(bool buttonCheckable)
{
    if (!buttonCheckable && model()->checked())
        model()->setChecked(false);
    model()->setCheckable(buttonCheckable);
}

bool MButton::isChecked() const
{
    return model()->checked();
}

//TODO Remove this when delayed model initialization bug is properly fixed.
static bool toggleEmitted = false;

void MButton::setChecked(bool buttonChecked)
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

bool MButton::isDown() const
{
    return model()->down();
}

void MButton::setDown(bool status)
{
    model()->setDown(status);
}

void MButton::click()
{
    model()->setDown(true);
    model()->setDown(false);
    modelClick();
}

void MButton::toggle()
{
    setChecked(!isChecked());
}

MButtonGroup *MButton::group() const
{
    Q_D(const MButton);
    return d->buttonGroup;
}

void MButton::setGroup(MButtonGroup *group)
{
    Q_D(MButton);

    d->buttonGroup = group;
}

void MButton::setupModel()
{
    MWidgetController::setupModel();

    connect(model(), SIGNAL(clicked()), SLOT(modelClick()));
}

void MButton::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    MWidgetController::contextMenuEvent(event);
}

void MButton::updateData(const QList<const char *>& modifications)
{
    MWidgetController::updateData(modifications);

    const char *member;
    foreach(member, modifications) {
        if (member == MButtonModel::Checked) {
            emit toggled(model()->checked());

            //TODO Remove this when delayed model initialization bug is properly fixed.
            toggleEmitted = true;
        } else if (member == MButtonModel::Down) {
            if (model()->down())
                emit pressed();
            else
                emit released();
        }
    }
}

void MButton::modelClick()
{
    toggle();
    emit clicked(isChecked());
}

