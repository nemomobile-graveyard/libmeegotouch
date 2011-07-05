/*
 * automatically generated with the command line :
 * m-servicefwgen -p com.nokia.widgetsgallery.AccessManager
 *
 * m-servicefwgen is Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * This file may have been hand-edited. Look for HAND-EDIT comments
 * before re-generating it.
 */

#include "accessmanagerproxy.h"

/*
 * Implementation of interface class AccessManagerProxy
 */

AccessManagerProxy::AccessManagerProxy(const QString &service, const QString &path, const QDBusConnection &connection, QObject *parent)
    : QDBusAbstractInterface(service, path, staticInterfaceName(), connection, parent)
{
}

AccessManagerProxy::~AccessManagerProxy()
{
}

