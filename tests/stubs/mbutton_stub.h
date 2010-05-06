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

#ifndef MBUTTON_STUB_H
#define MBUTTON_STUB_H

#include <stubbase.h>
#include <mbutton.h>
#include <mbuttongroup.h>
class MButtonStub
    : public StubBase
{
public:
    virtual void mButtonConstructor(MWidget *parent, const QString &viewType);
    virtual void mButtonDestructor();
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
    virtual void setView(MWidgetView *view);
    virtual bool isCheckable() const;
    virtual void setCheckable(bool buttonCheckable);
    virtual bool isChecked() const;
    virtual void setChecked(bool buttonChecked);
    virtual void toggleStateChanged();
    virtual MButtonGroup *group() const;
};

void MButtonStub::mButtonConstructor(MWidget *parent, const QString &viewType)
{
    Q_UNUSED(parent);
    Q_UNUSED(viewType);
}

void MButtonStub::mButtonDestructor()
{
}

void MButtonStub::setIconID(const QString &iconID)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(iconID));
    stubMethodEntered("setIconID", params);
    Q_UNUSED(iconID);
}

const QString &MButtonStub::iconID() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("iconID", params);
    return stubReturnValue<QString>(QString("iconID"));
    QString *myIconID = new QString("iconID");
    return *myIconID;
}

void MButtonStub::setToggledIconID(const QString &toggledIconID)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(toggledIconID));
    stubMethodEntered("setToggledIconID", params);
    Q_UNUSED(toggledIconID);

}

const QString &MButtonStub::toggledIconID() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("toggledIconID", params);
    return stubReturnValue<QString>(QString("toggledIconID"));
    QString *myToggledIconID = new QString("toggledIconID");
    return *myToggledIconID;
}

void MButtonStub::setText(const QString &text)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QString>(text));
    stubMethodEntered("setText", params);
    Q_UNUSED(text);
}

const QString &MButtonStub::text() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("text", params);
    return stubReturnValue<QString>(QString("text"));
}

bool MButtonStub::isTextVisible() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("isTextVisible", params);
    return stubReturnValue<bool>(QString("isTextVisible"));
}

void MButtonStub::setTextVisible(bool textVisible)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(textVisible));
    stubMethodEntered("setTextVisible", params);
    Q_UNUSED(textVisible);
}

bool MButtonStub::isIconVisible() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("isIconVisible", params);
    return stubReturnValue<bool>(QString("isIconVisible"));
}

void MButtonStub::setIconVisible(bool iconVisible)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(iconVisible));
    stubMethodEntered("setIconVisible", params);
    Q_UNUSED(iconVisible);
}
void MButtonStub::setView(MWidgetView *view)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MWidgetView *>(view));
    stubMethodEntered("setView", params);
    Q_UNUSED(view);
}

bool MButtonStub::isCheckable() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("isCheckable", params);
    return stubReturnValue<bool>(QString("isCheckable"));
}

void MButtonStub::setCheckable(bool buttonCheckable)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(buttonCheckable));
    stubMethodEntered("setCheckable", params);
    Q_UNUSED(buttonCheckable);
}

bool MButtonStub::isChecked() const
{
    QList<ParameterBase *> params;
    stubMethodEntered("isChecked", params);
    return stubReturnValue<bool>(QString("isChecked"));
}

void MButtonStub::setChecked(bool buttonChecked)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool>(buttonChecked));
    stubMethodEntered("setChecked", params);
    Q_UNUSED(buttonChecked);
}

void MButtonStub::toggleStateChanged()
{
    stubMethodEntered("toggleStateChanged");
}
MButtonGroup *MButtonStub::group() const
{
    stubMethodEntered("group");
    return stubReturnValue<MButtonGroup *>("group");
}
MButtonStub gDefaultMButtonStub;
MButtonStub *gMButtonStub = &gDefaultMButtonStub;

MButton::MButton(MWidget *parent, const QString &viewType)
{
    gMButtonStub->mButtonConstructor(parent, "");
    Q_UNUSED(viewType);
}

MButton::~MButton()
{
    gMButtonStub->mButtonDestructor();
}

void MButton::setIconID(const QString &iconID)
{
    gMButtonStub->setIconID(iconID);
}

const QString &MButton::iconID() const
{
    return gMButtonStub->iconID();
}

void MButton::setToggledIconID(const QString &toggledIconID)
{
    gMButtonStub->setToggledIconID(toggledIconID);
}

const QString &MButton::toggledIconID() const
{
    return gMButtonStub->toggledIconID();
}

void MButton::setText(const QString &text)
{
    gMButtonStub->setText(text);
}

const QString &MButton::text() const
{
    return gMButtonStub->text();
}

void MButton::setView(MWidgetView *view)
{
    gMButtonStub->setView(view);
}

bool MButton::isTextVisible() const
{
    return gMButtonStub->isTextVisible();
}

void MButton::setTextVisible(bool textVisible)
{
    gMButtonStub->setTextVisible(textVisible);
}

bool MButton::isIconVisible() const
{
    return gMButtonStub->isIconVisible();
}

void MButton::setIconVisible(bool iconVisible)
{
    gMButtonStub->setIconVisible(iconVisible);
}
bool MButton::isCheckable() const
{
    return gMButtonStub->isCheckable();
}

void MButton::setCheckable(bool buttonCheckable)
{
    gMButtonStub->setCheckable(buttonCheckable);
}

bool MButton::isChecked() const
{
    return gMButtonStub->isChecked();
}

void MButton::setChecked(bool buttonChecked)
{
    gMButtonStub->setChecked(buttonChecked);
}
void MButton::toggleStateChanged()
{
    gMButtonStub->toggleStateChanged();
}
MButtonGroup *MButton::group() const
{
    return gMButtonStub->group();
}
#endif
