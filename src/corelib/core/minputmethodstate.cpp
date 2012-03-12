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

#ifdef HAVE_MALIIT
#include <maliit/attributeextension.h>
#include <maliit/inputmethod.h>

using Maliit::InputMethod;
using Maliit::AttributeExtension;
#endif

namespace {
    const char * const ToolbarExtension("/toolbar");

    // Keys describing extended attributes should be combined from three parts:
    // target, item and attribute name.
    // See also MInputMethodState::setExtendedAttribute()
    const int ExtendedAttributeKeyParts = 3;

    // Separator between parts in the key pointing to extended attribute
    const QChar ExtendedAttributeKeySeparator('/');
}

AttributeExtensionInfo::AttributeExtensionInfo(const QString &attributeExtensionFile)
#ifdef HAVE_MALIIT
    : extension(new Maliit::AttributeExtension(attributeExtensionFile))
#else
    : fileName(attributeExtensionFile)
#endif
{
#ifdef HAVE_MALIIT
    connect(extension.data(), SIGNAL(extendedAttributeChanged(QString, QVariant)),
            this, SLOT(updateExtendedAttribute(QString,QVariant)));
#endif
}

#ifdef HAVE_MALIIT
void AttributeExtensionInfo::updateExtendedAttribute(const QString &key, const QVariant &value)
{
    const QStringList list = key.split(ExtendedAttributeKeySeparator, QString::SkipEmptyParts);

    if (list.count() == ExtendedAttributeKeyParts) {
        const QString target = ExtendedAttributeKeySeparator + list.at(0);
        const QString targetItem = list.at(1);
        const QString attribute = list.at(2);

        extendedAttributes[target][targetItem][attribute] = value;
        emit extendedAttributeChanged(extension->id(), target, targetItem,
                                      attribute, value);
    }
}
#endif

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
#ifdef HAVE_MALIIT
    bool ok;

    ok = connect(Maliit::InputMethod::instance(), SIGNAL(areaChanged(QRect)),
                 this, SIGNAL(inputMethodAreaChanged(QRect)));
    if (!ok) qFatal("signal connection failed!");

    ok = connect(Maliit::InputMethod::instance(), SIGNAL(keyPress(QKeyEvent)),
                 this, SIGNAL(keyPress(QKeyEvent)));
    if (!ok) qFatal("signal connection failed!");

    ok = connect(Maliit::InputMethod::instance(), SIGNAL(keyRelease(QKeyEvent)),
                 this, SIGNAL(keyRelease(QKeyEvent)));
    if (!ok) qFatal("signal connection failed!");

    ok = connect(Maliit::InputMethod::instance(), SIGNAL(languageChanged(QString)),
                 this, SIGNAL(languageChanged(QString)));
    if (!ok) qFatal("signal connection failed!");
#endif
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
#ifdef HAVE_MALIIT
    Maliit::InputMethod::instance()->setArea(newRegion);
#else
    Q_D(MInputMethodState);

    if (d->region != newRegion) {
        d->region = newRegion;
        emit inputMethodAreaChanged(d->region);
    }
#endif
}

QRect MInputMethodState::inputMethodArea() const
{
#ifdef HAVE_MALIIT
    return Maliit::InputMethod::instance()->area();
#else
    Q_D(const MInputMethodState);

    return d->region;
#endif
}

void MInputMethodState::startActiveWindowOrientationAngleChange(M::OrientationAngle newOrientationAngle)
{
    Q_D(MInputMethodState);

    if (d->orientationAngle != newOrientationAngle) {
        d->orientationAngle = newOrientationAngle;
        d->rotationInProgress = true;
        emit activeWindowOrientationAngleAboutToChange(newOrientationAngle);
    }
#ifdef HAVE_MALIIT
    Maliit::InputMethod::instance()->startOrientationAngleChange(static_cast<Maliit::OrientationAngle>(newOrientationAngle));
#endif
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
#ifdef HAVE_MALIIT
    Maliit::InputMethod::instance()->setOrientationAngle(static_cast<Maliit::OrientationAngle>(newOrientationAngle));
#endif
}

M::OrientationAngle MInputMethodState::activeWindowOrientationAngle() const
{
    Q_D(const MInputMethodState);

    return d->orientationAngle;
}

void MInputMethodState::requestSoftwareInputPanel()
{
#ifdef HAVE_MALIIT
    Maliit::requestInputMethodPanel();
#else
    QInputContext *inputContext = qApp->inputContext();

    if (!inputContext) {
        return;
    }

    QEvent request(QEvent::RequestSoftwareInputPanel);
    inputContext->filterEvent(&request);
#endif
}

void MInputMethodState::closeSoftwareInputPanel()
{
#ifdef HAVE_MALIIT
    Maliit::closeInputMethodPanel();
#else
    QInputContext *inputContext = qApp->inputContext();

    if (!inputContext) {
        return;
    }

    QEvent close(QEvent::CloseSoftwareInputPanel);
    inputContext->filterEvent(&close);
    inputContext->reset();
#endif
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

#ifdef HAVE_MALIIT
    const QString &key = QString::fromLatin1("%1/%2/%3").arg(ToolbarExtension, item, attribute);
    attributeExtension->extension->setAttribute(key, value);
#endif

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
    AttributeExtensionInfo *attributeExtension = new AttributeExtensionInfo(fileName);
#ifdef HAVE_MALIIT
    int newId = attributeExtension->extension->id();
    connect(attributeExtension,
            SIGNAL(extendedAttributeChanged(int, QString, QString, QString, QVariant)),
            this,
            SIGNAL(extendedAttributeChanged(int, QString, QString, QString, QVariant)));
#else
    static int idCounter = 0;
    // generate an application local unique identifier for the toolbar.
    int newId = idCounter;
    idCounter++;
#endif
    d->attributeExtensions.insert(newId, attributeExtension);
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

#ifdef HAVE_MALIIT
    attributeExtension->extension->setAttribute(Maliit::AttributeExtension::key(target, targetItem, attribute),
                                                value);
#endif

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
#ifdef HAVE_MALIIT
        return attributeExtension->extension->fileName();
#else
        return attributeExtension->fileName;
#endif
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
