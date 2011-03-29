/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSERVICELISTENER_H
#define MSERVICELISTENER_H

#include <QDBusConnection>
#include <QDBusServiceWatcher>

#include "mnamespace.h"
#include "mexport.h"

class QDBusPendingCallWatcher;

//! \internal

/*! \class MServiceListener

  \note Not part of the public API. This is an internal class that should not be used by
        libmeegotouch clients.

  \note This is a copy of ContextProvider::DBusNameListener as it is not public class
  and the same functionality is needed here to detect disappearance of the
  'RemoteScreen.TopEdge' property so that we don't end up with screen locked to some
  orientation indefinitely.

  \brief Extends the QDBusServiceWatcher; adds the possibility to do an
  initial check of the service status (asyncronously) and to query the status
  of the service any time.

  When you create an instance of this class, it won't open any D-Bus
  connections. When startListening is called, the instance starts using the
  QDBusServiceWatcher to get updates when the service is registered or
  unregistered.  It can also check the current status of the service by
  executing an asynchronous NameHasOwner call.

  If the specified service appears on D-Bus, it will emit the \c
  nameAppeared() signal, if disappears, then the nameDisappeared()
  signal.  An initial query and signal emission will be done if \c
  initialCheck is true, which is the default.

  Anytime you can check with <tt>isServicePresent()</tt> if according
  to our current knowledge (last signal emission) the service is
  present or not.  This means that if \c initialCheck is false,
  <tt>isServicePresent()</tt> can return false, even though the service
  is present.
*/
class M_CORE_EXPORT MServiceListener : public QDBusServiceWatcher
{
    Q_OBJECT
public:
    enum ServicePresence {NotPresent = 0, Present, Unknown};

    explicit MServiceListener(QDBusConnection::BusType busType, const QString &busName, QObject *parent = 0);
    explicit MServiceListener(const QDBusConnection bus, const QString &busName, QObject *parent = 0);
    virtual ~MServiceListener();

    /*
      Start listening to the service registration / unregistration over
      D-Bus. If \a nameHasOwnerCheck is true, also send a NameHasOwner query to
      D-Bus (asyncronously).
     */
    void startListening(bool nameHasOwnerCheck);

    /*
      Return our current understanding (not present, present, unknown)
      of the presence of the watched service.
     */
    ServicePresence isServicePresent() const;

private Q_SLOTS:
    /*
      Handling of the asynchronous reply of the initial query.
     */
    void onNameHasOwnerFinished(QDBusPendingCallWatcher *watcher);
    void setServicePresent();
    void setServiceGone();

private:
    /*
      Initializes the QDBusServiceWatcher base part and connects the signals
      from it.
    */
    void init();
    ServicePresence servicePresent; ///< Our current understanding about the service name's state
    QDBusConnection::BusType busType; ///< The service bus type we are interested in
    QString busName; ///< The service name we are interested in
    bool listeningStarted;
    QDBusConnection *connection;

Q_SIGNALS:
    void nameAppeared();
    void nameDisappeared();
};

//! \internal_end

#endif // MSERVICELISTENER_H
