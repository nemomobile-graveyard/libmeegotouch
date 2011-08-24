/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#include "minputmethodstate.h"
#include "minputmethodstate_p.h"

#include <MDebug>

#include <QApplication>
#include <QInputContext>
#include <QTimer>
#include <QKeyEvent>

namespace {
    const char * const ToolbarExtension("/toolbar");
}

MInputMethodStatePrivate::MInputMethodStatePrivate()
    : orientationAngle(M::Angle0),
      rotationInProgress(false),
      q_ptr(NULL)
{
}

MInputMethodStatePrivate::~MInputMethodStatePrivate()
{
    Q_Q(MInputMethodState);

    foreach (int id, attributeExtensions.keys()) {
        emit q->attributeExtensionUnregistered(id);
        delete attributeExtensions[id];
    }
}

void MInputMethodStatePrivate::init()
{
}

MInputMethodState::MInputMethodState()
    : d_ptr(new MInputMethodStatePrivate)
{
    Q_D(MInputMethodState);

    d->q_ptr = this;
    d->init();
    connect(this, SIGNAL(attributeExtensionRegistered(int, QString)),
            this, SIGNAL(toolbarRegistered(int,QString)));
    connect(this, SIGNAL(attributeExtensionUnregistered(int)),
            this, SIGNAL(toolbarUnregistered(int)));
}

MInputMethodState::~MInputMethodState()
{
    delete d_ptr;
}

MInputMethodState *MInputMethodState::instance()
{
    static MInputMethodState singleton;

    return &singleton;
}

void MInputMethodState::setInputMethodArea(const QRect &newRegion)
{
    Q_D(MInputMethodState);

    if (d->region != newRegion) {
        d->region = newRegion;
        emit inputMethodAreaChanged(d->region);
    }
}

QRect MInputMethodState::inputMethodArea() const
{
    Q_D(const MInputMethodState);

    return d->region;
}

void MInputMethodState::startActiveWindowOrientationAngleChange(M::OrientationAngle newOrientationAngle)
{
    Q_D(MInputMethodState);

    if (d->orientationAngle != newOrientationAngle) {
        d->orientationAngle = newOrientationAngle;
        d->rotationInProgress = true;
        emit activeWindowOrientationAngleAboutToChange(newOrientationAngle);
    }
}

void MInputMethodState::setActiveWindowOrientationAngle(M::OrientationAngle newOrientationAngle)
{
    Q_D(MInputMethodState);

    if (d->orientationAngle != newOrientationAngle) {
        d->orientationAngle = newOrientationAngle;
        d->rotationInProgress = true;
    }
    if (d->rotationInProgress) {
        d->rotationInProgress = false;
        emit activeWindowOrientationAngleChanged(newOrientationAngle);
    }
}

M::OrientationAngle MInputMethodState::activeWindowOrientationAngle() const
{
    Q_D(const MInputMethodState);

    return d->orientationAngle;
}

void MInputMethodState::requestSoftwareInputPanel()
{
    QInputContext *inputContext = qApp->inputContext();

    if (!inputContext) {
        return;
    }

    QEvent request(QEvent::RequestSoftwareInputPanel);
    inputContext->filterEvent(&request);
}

void MInputMethodState::closeSoftwareInputPanel()
{
    QInputContext *inputContext = qApp->inputContext();

    if (!inputContext) {
        return;
    }

    QEvent close(QEvent::CloseSoftwareInputPanel);
    inputContext->filterEvent(&close);
    inputContext->reset();
}

void MInputMethodState::emitKeyPress(const QKeyEvent &event)
{
    emit keyPress(event);
}

void MInputMethodState::emitKeyRelease(const QKeyEvent &event)
{
    emit keyRelease(event);
}

QList<int> MInputMethodState::toolbarIds() const
{
    mWarning("MInputMethodState") << Q_FUNC_INFO << "is deprecated. Use attributeExtensionIds() instead";
    return attributeExtensionIds();
}

MInputMethodState::ItemAttributeMap MInputMethodState::toolbarState(int id) const
{
    mWarning("MInputMethodState") << Q_FUNC_INFO << "is deprecated. Use extendedAttributes() instead";
    Q_D(const MInputMethodState);
    AttributeExtensionInfo *attributeExtension = d->attributeExtensions.value(id);

    return (attributeExtension ? attributeExtension->extendedAttributes.value(ToolbarExtension) : ItemAttributeMap());
}

int MInputMethodState::registerToolbar(const QString &fileName)
{
    mWarning("MInputMethodState") << Q_FUNC_INFO << "is deprecated. Use registerAttributeExtension() instead";
    return registerAttributeExtension(fileName);
}

void MInputMethodState::unregisterToolbar(int id)
{
    mWarning("MInputMethodState") << Q_FUNC_INFO << "is deprecated. Use unregisterAttributeExtension() instead";
    return unregisterAttributeExtension(id);
}

void MInputMethodState::setToolbarItemAttribute(int id, const QString &item,
                                                const QString &attribute, const QVariant &value)
{
    mWarning("MInputMethodState") << Q_FUNC_INFO << "is deprecated. Use setExtendedAttribute() instead";
    Q_D(MInputMethodState);

    if (!d->attributeExtensions.contains(id)) {
        return;
    }

    AttributeExtensionInfo *attributeExtension = d->attributeExtensions[id];
    bool changed = (value != attributeExtension->extendedAttributes[ToolbarExtension][item][attribute]);

    if (changed) {
        attributeExtension->extendedAttributes[ToolbarExtension][item][attribute] = value;
        emit toolbarItemAttributeChanged(id, item, attribute, value);
    }
}

QString MInputMethodState::toolbar(int id) const
{
    mWarning("MInputMethodState") << Q_FUNC_INFO << "is deprecated. Use attributeExtensionFile() instead";
    return attributeExtensionFile(id);
}

QList<int> MInputMethodState::attributeExtensionIds() const
{
    Q_D(const MInputMethodState);
    return d->attributeExtensions.keys();
}

MInputMethodState::ExtendedAttributeMap MInputMethodState::extendedAttributes(int id) const
{
    Q_D(const MInputMethodState);
    AttributeExtensionInfo *attributeExtension = d->attributeExtensions.value(id);

    return (attributeExtension ? attributeExtension->extendedAttributes : ExtendedAttributeMap());
}

int MInputMethodState::registerAttributeExtension(const QString &fileName)
{
    Q_D(MInputMethodState);
    static int idCounter = 0;
    // generate an application local unique identifier for the toolbar.
    int newId = idCounter;
    idCounter++;
    d->attributeExtensions.insert(newId, new AttributeExtensionInfo(fileName));
    emit attributeExtensionRegistered(newId, fileName);
    return newId;
}

void MInputMethodState::unregisterAttributeExtension(int id)
{
    Q_D(MInputMethodState);
    delete d->attributeExtensions[id];
    d->attributeExtensions.remove(id);
    emit attributeExtensionUnregistered(id);
}

void MInputMethodState::setExtendedAttribute(int id, const QString &target, const QString &targetItem,
                                             const QString &attribute, const QVariant &value)
{
    Q_D(MInputMethodState);

    if (!d->attributeExtensions.contains(id)) {
        return;
    }

    AttributeExtensionInfo *attributeExtension = d->attributeExtensions[id];
    bool changed = (value != attributeExtension->extendedAttributes[target][targetItem][attribute]);

    if (changed) {
        attributeExtension->extendedAttributes[target][targetItem][attribute] = value;
        emit extendedAttributeChanged(id, target, targetItem, attribute, value);
    }
}

QString MInputMethodState::attributeExtensionFile(int id) const
{
    Q_D(const MInputMethodState);

    AttributeExtensionInfo *attributeExtension = d->attributeExtensions.value(id);

    if (attributeExtension) {
        return attributeExtension->fileName;
    } else {
        return QString();
    }
}

QString MInputMethodState::language() const
{
#ifdef HAVE_MALIIT
    return Maliit::InputMethod::instance()->language();
#else
    QInputContext *inputContext = qApp->inputContext();
    return inputContext ? inputContext->language() : QString();
#endif
}

#include "moc_minputmethodstate.cpp"
