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

#ifndef DUIBUTTON_STUB_H
#define DUIBUTTON_STUB_H

#include <stubbase.h>
#include <duibutton.h>
#include <duibuttongroup.h>
class DuiButtonStub
    : public StubBase
{
public:
    virtual void duiButtonConstructor(DuiWidget *parent, const QString &viewType);
    virtual void duiButtonDestructor();
    virtual void setIconID(const QString &iconID);
    virtual const QString &iconID() const;
    virtual void setToggledIconID(const QString &toggledIconID);
    virtual const QString &toggledIconID() const;
    virtual void setText(const QString &text);
    virtual const QString &text() const;
    virtual bool isTextVisible() const;
    virtual void setTextVisible(bool textVisible);
    virtual bool isIconVisible() const;
    virtual void setIconVisible(bool iconVisible);
    virtual void setView(DuiWidgetView *view);
    virtual bool isCheckable() const;
    virtual void setCheckable(bool buttonCheckable);
    virtual bool isChecked() const;
    virtual void setChecked(bool buttonChecked);
    virtual void toggleStateChanged();
    virtual DuiButtonGroup *group() const;
};

void DuiButtonStub::duiButtonConstructor(DuiWidget *parent, const QString &viewType)
{
    Q_UNUSED(parent);
    Q_UNUSED(viewType);
}

void DuiButtonStub::duiButtonDestructor()
{
}

void DuiButtonStub::setIconID(const QString &iconID)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(iconID));
    stubMethodEntered("setIconID", params);
    Q_UNUSED(iconID);
}

const QString &DuiButtonStub::iconID() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("iconID", params);
    return stubReturnValue<QString>(QString("iconID"));
    QString *myIconID = new QString("iconID");
    return *myIconID;
}

void DuiButtonStub::setToggledIconID(const QString &toggledIconID)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(toggledIconID));
    stubMethodEntered("setToggledIconID", params);
    Q_UNUSED(toggledIconID);

}

const QString &DuiButtonStub::toggledIconID() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("toggledIconID", params);
    return stubReturnValue<QString>(QString("toggledIconID"));
    QString *myToggledIconID = new QString("toggledIconID");
    return *myToggledIconID;
}

void DuiButtonStub::setText(const QString &text)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(text));
    stubMethodEntered("setText", params);
    Q_UNUSED(text);
}

const QString &DuiButtonStub::text() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("text", params);
    return stubReturnValue<QString>(QString("text"));
}

bool DuiButtonStub::isTextVisible() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("isTextVisible", params);
    return stubReturnValue<bool>(QString("isTextVisible"));
}

void DuiButtonStub::setTextVisible(bool textVisible)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(textVisible));
    stubMethodEntered("setTextVisible", params);
    Q_UNUSED(textVisible);
}

bool DuiButtonStub::isIconVisible() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("isIconVisible", params);
    return stubReturnValue<bool>(QString("isIconVisible"));
}

void DuiButtonStub::setIconVisible(bool iconVisible)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(iconVisible));
    stubMethodEntered("setIconVisible", params);
    Q_UNUSED(iconVisible);
}
void DuiButtonStub::setView(DuiWidgetView *view)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiWidgetView *>(view));
    stubMethodEntered("setView", params);
    Q_UNUSED(view);
}

bool DuiButtonStub::isCheckable() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("isCheckable", params);
    return stubReturnValue<bool>(QString("isCheckable"));
}

void DuiButtonStub::setCheckable(bool buttonCheckable)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(buttonCheckable));
    stubMethodEntered("setCheckable", params);
    Q_UNUSED(buttonCheckable);
}

bool DuiButtonStub::isChecked() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("isChecked", params);
    return stubReturnValue<bool>(QString("isChecked"));
}

void DuiButtonStub::setChecked(bool buttonChecked)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(buttonChecked));
    stubMethodEntered("setChecked", params);
    Q_UNUSED(buttonChecked);
}

void DuiButtonStub::toggleStateChanged()
{
    stubMethodEntered("toggleStateChanged");
}
DuiButtonGroup *DuiButtonStub::group() const
{
    stubMethodEntered("group");
    return stubReturnValue<DuiButtonGroup *>("group");
}
DuiButtonStub gDefaultDuiButtonStub;
DuiButtonStub *gDuiButtonStub = &gDefaultDuiButtonStub;

DuiButton::DuiButton(DuiWidget *parent, const QString &viewType)
{
    gDuiButtonStub->duiButtonConstructor(parent, "");
    Q_UNUSED(viewType);
}

DuiButton::~DuiButton()
{
    gDuiButtonStub->duiButtonDestructor();
}

void DuiButton::setIconID(const QString &iconID)
{
    gDuiButtonStub->setIconID(iconID);
}

const QString &DuiButton::iconID() const
{
    return gDuiButtonStub->iconID();
}

void DuiButton::setToggledIconID(const QString &toggledIconID)
{
    gDuiButtonStub->setToggledIconID(toggledIconID);
}

const QString &DuiButton::toggledIconID() const
{
    return gDuiButtonStub->toggledIconID();
}

void DuiButton::setText(const QString &text)
{
    gDuiButtonStub->setText(text);
}

const QString &DuiButton::text() const
{
    return gDuiButtonStub->text();
}

void DuiButton::setView(DuiWidgetView *view)
{
    gDuiButtonStub->setView(view);
}

bool DuiButton::isTextVisible() const
{
    return gDuiButtonStub->isTextVisible();
}

void DuiButton::setTextVisible(bool textVisible)
{
    gDuiButtonStub->setTextVisible(textVisible);
}

bool DuiButton::isIconVisible() const
{
    return gDuiButtonStub->isIconVisible();
}

void DuiButton::setIconVisible(bool iconVisible)
{
    gDuiButtonStub->setIconVisible(iconVisible);
}
bool DuiButton::isCheckable() const
{
    return gDuiButtonStub->isCheckable();
}

void DuiButton::setCheckable(bool buttonCheckable)
{
    gDuiButtonStub->setCheckable(buttonCheckable);
}

bool DuiButton::isChecked() const
{
    return gDuiButtonStub->isChecked();
}

void DuiButton::setChecked(bool buttonChecked)
{
    gDuiButtonStub->setChecked(buttonChecked);
}
void DuiButton::toggleStateChanged()
{
    gDuiButtonStub->toggleStateChanged();
}
DuiButtonGroup *DuiButton::group() const
{
    return gDuiButtonStub->group();
}
#endif
