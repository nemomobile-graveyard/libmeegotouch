/*
 * automatically generated with the command line :
 * m-servicefwgen -a com.nokia.widgetsgallery.AccessManager
 *
 * m-servicefwgen is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "accessmanageradaptor.h"

#include <MComponentData>
#include <MDebug>

#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class AccessManagerAdaptor
 */

AccessManagerAdaptor::AccessManagerAdaptor(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

AccessManagerAdaptor::~AccessManagerAdaptor()
{
    // destructor
}

bool AccessManagerAdaptor::login(const uint _windowId, const QString _taskTitle, bool fullscreen, bool autofocus, int orientation)
{
    MComponentData::ChainData thisData( _windowId, _taskTitle );
    MComponentData::pushChainData( thisData );

    // handle method call com.nokia.widgetsgallery.AccessManager.login
    bool out0;
    QMetaObject::invokeMethod(parent(), "login", Q_RETURN_ARG(bool, out0), Q_ARG(bool, fullscreen), Q_ARG(bool, autofocus), Q_ARG(int, orientation));
    return out0;
}

