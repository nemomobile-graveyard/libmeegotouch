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

#include <DuiDebug>
#include <QList>

#include "duibutton.h"
#include "duibuttongroup.h"
#include "duibuttongroup_p.h"


DuiButtonGroupPrivate::DuiButtonGroupPrivate()
    : checkedButton(0),
      exclusive(true)
{
}

DuiButtonGroupPrivate::~DuiButtonGroupPrivate()
{
}

void DuiButtonGroupPrivate::buttonPress()
{
    Q_Q(DuiButtonGroup);

    DuiButton *button = static_cast<DuiButton *>(this->sender());
    emit q->buttonPressed(button);
    int i = q->id(button);
    if (i != -1)
        emit q->buttonPressed(i);
}

void DuiButtonGroupPrivate::buttonRelease()
{
    Q_Q(DuiButtonGroup);

    DuiButton *button = static_cast<DuiButton *>(this->sender());
    emit q->buttonReleased(button);
    int i = q->id(button);
    if (i != -1)
        emit q->buttonReleased(i);
}

void DuiButtonGroupPrivate::buttonClick()
{
    Q_Q(DuiButtonGroup);

    DuiButton *button = static_cast<DuiButton *>(this->sender());
    emit q->buttonClicked(button);
    int i = q->id(button);
    if (i != -1)
        emit q->buttonClicked(i);
}

void DuiButtonGroupPrivate::buttonToggle(bool checked)
{
    Q_Q(DuiButtonGroup);

    DuiButton *button = static_cast<DuiButton *>(this->sender());
    if (!button) {
        duiWarning("DuiButtonGroup") << "buttonToggle(): slot unexpectedly called by someone else than DuiButton";
        return;
    }

    if (!button->isCheckable()) {
        duiWarning("DuiButtonGroup") << "buttonToggle(): Cannot check non-checkable button.";
        return;
    }

    if (checked && q->exclusive() && checkedButton) {
        exclusive = false;
        checkedButton->setChecked(false);
        exclusive = true;
    }

    if (checked) {
        checkedButton = button;
    } else if (checkedButton == button) {
        /* If a button is unchecked and its same as checkedButton, then
           checkedButton should be reset as well */
        checkedButton = 0;
    }
}

DuiButtonGroup::DuiButtonGroup(QObject *parent)
    : QObject(parent),
      d_ptr(new DuiButtonGroupPrivate)
{
    d_ptr->q_ptr = this;
}

DuiButtonGroup::~DuiButtonGroup()
{
    delete d_ptr;
}

void DuiButtonGroup::addButton(DuiButton *button)
{
    Q_D(DuiButtonGroup);

    if (d->buttonList.contains(button)) {
        return;
    }

    d->buttonList.append(button);

    button->setGroup(this);
    connect(button, SIGNAL(pressed()), d, SLOT(buttonPress()));
    connect(button, SIGNAL(released()), d, SLOT(buttonRelease()));
    connect(button, SIGNAL(clicked()), d, SLOT(buttonClick()));
    connect(button, SIGNAL(toggled(bool)), d, SLOT(buttonToggle(bool)));

    /* Adding a checked button to a group makes it the currently checked button */
    if (button->isCheckable() && button->isChecked()) {
        if (exclusive() && d->checkedButton) {
            //TODO do this somehow differently if possible.
            d->exclusive = false;
            d->checkedButton->setChecked(false);
            d->exclusive = true;
        }
        d->checkedButton = button;
    }
}

void DuiButtonGroup::addButton(DuiButton *button, int id)
{
    Q_D(DuiButtonGroup);

    if (id == -1)
        duiWarning("DuiButtonGroup") << "addButton(): Ignoring invalid id.";
    else
        d->buttonIdMap.insert(button, id);

    addButton(button);
}

DuiButton *DuiButtonGroup::button(int id) const
{
    Q_D(const DuiButtonGroup);
    return d->buttonIdMap.key(id);
}

QList<DuiButton *> DuiButtonGroup::buttons() const
{
    Q_D(const DuiButtonGroup);

    return d->buttonList;
}

DuiButton *DuiButtonGroup::checkedButton() const
{
    Q_D(const DuiButtonGroup);
    return d->checkedButton;
}

int DuiButtonGroup::checkedId() const
{
    Q_D(const DuiButtonGroup);
    return d->buttonIdMap.value(d->checkedButton, -1);
}

bool DuiButtonGroup::exclusive(void) const
{
    Q_D(const DuiButtonGroup);
    return d->exclusive;
}

void DuiButtonGroup::setExclusive(bool exclusive)
{
    Q_D(DuiButtonGroup);
    d->exclusive = exclusive;
    validateExclusivity();
}

int DuiButtonGroup::id(DuiButton *button) const
{
    Q_D(const DuiButtonGroup);
    return d->buttonIdMap.value(button, -1);
}

void DuiButtonGroup::removeButton(DuiButton *button)
{
    Q_D(DuiButtonGroup);

    if (button->group() == this) {
        d->buttonList.removeAll(button);
        d->buttonIdMap.remove(button);
        button->setGroup(0);

        disconnect(button, 0, d, 0);

        if (d->checkedButton == button) {
            d->checkedButton = 0;
            validateExclusivity();
        }
    }
}

void DuiButtonGroup::setId(DuiButton *button, int id)
{
    Q_D(DuiButtonGroup);
    if (d->buttonList.contains(button)) {
        if (id == -1)
            duiWarning("DuiButtonGroup") << "setId(): Ignoring invalid id.";
        else
            d->buttonIdMap[button] = id;
    } else {
        duiWarning("DuiButtonGroup") << "setId(): button(" << (void *)button << ") is not in the group.";
    }
}

void DuiButtonGroup::validateExclusivity()
{
    Q_D(DuiButtonGroup);

    if (exclusive()) {
        /* In case of exclusive mode:
        case 1: If there is no button checked, find the first checkable
        button to be checked and set it checked
        case 2: If there are more than one buttons that are checked,
        uncheck all the buttons except the first checked one
        */
        DuiButton *checkableButton = 0;
        DuiButton *checkedButton = 0;
        QList<DuiButton *>::iterator i = d->buttonList.begin();
        while (i != d->buttonList.end()) {
            DuiButton *b = *i;
            /* Get the first checkable button */
            if (b->isCheckable() && checkableButton == 0) {
                checkableButton = b;
            }
            /* Set the first checked button and uncheck the rest */
            if (b->isChecked()) {
                if (checkedButton == 0) {
                    checkedButton = b;
                    d->checkedButton = b;
                } else {
                    b->setChecked(false);
                }
            }
            ++i;
        }
        if (d->checkedButton == 0 && checkableButton != 0) {
            /* Make sure that exactly one button is checked */
            checkableButton->setChecked(true);
            d->checkedButton = checkableButton;
        }
    }
}
