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

// Make doxygen skip this internal class
//! \cond
#ifndef SERVICEMAPPER_H
#define SERVICEMAPPER_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QFileSystemWatcher>

#include "mservicemapper_p.h"

class MServiceMapper : public QObject
{
    Q_OBJECT
public:
    /*
     * \brief Constructs a service mapper
     */
    MServiceMapper(const QString &serviceFileDir);

    /*
     * \brief Destructor
     */
    virtual ~MServiceMapper();

public Q_SLOTS:
    /*
     * \brief Returns a list of service names for services that implement the given interface.
     * \param interfaceName Name of the interface; default="" lists all services
     * \return List of service names
     */
    QStringList serviceNames(const QString &interfaceName = "");

    /*
     * \brief List the preferred service for the given interface
     * \param interfaceName Name of the interface
     * \return Name of the service
     */
    QString serviceName(const QString &interfaceName);

    /*
     * \brief Get the path for the given interface
     * \param interfaceName Name of the interface
     * \return Name of the service path
     */
    QString servicePath(const QString &interfaceName) const;

    /*
     * \brief Get the interface for the given service
     * \param serviceName Name of the interface
     * \return Name of the service
     */
    QString interfaceName(const QString &serviceName) const;

Q_SIGNALS:
    /*
     * \brief A signal that is emitted when a service becomes available
     * \param service The name of the service that has become available
     * \param interface The name of the interface
     */
    void serviceAvailable(const QString &service, const QString &interface);

    /*
     * \brief A signal that is emitted when the last service implementing
     * the interface becomes unavailable.
     * \param service The name of the service that has become unavailable
     */
    void serviceUnavailable(const QString &service);

private Q_SLOTS:
    /*
     * \brief Handle the situation when a service has changed by a dbus
     * .service file being created or deleted.
     * \param path The directory that has been changed.
     */
    void handleServiceChanged(const QString &path);

private:
    enum { LAST_LIST, CURR_LIST };
    void init();

    MServiceMapperPrivate *d_ptr;

#ifdef UNIT_TEST
public:
    MServiceMapper(MServiceMapperPrivate *priv);
    friend class Ut_MServiceMapper;
#endif // UNIT_TEST
};
#endif
//! \endcond
