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

#ifndef MREMOTEACTION_H_
#define MREMOTEACTION_H_

#include <MAction>
#include <MExport>

class MRemoteActionPrivate;

/*!
 * \class MRemoteAction
 *
 * \brief MRemoteAction implements a MAction that executes a D-Bus call when triggered.
 *        The D-Bus related parameters can be serialized and unserialized into a string.
 */
class M_CORE_EXPORT MRemoteAction : public MAction
{
    Q_OBJECT

public:
    /*!
     * \brief Constructs a MRemoteAction from a D-Bus service path, object path, interface and arguments.
     *
     * \param serviceName the service path of the D-Bus object to be called
     * \param objectPath the object path of the D-Bus object to be called
     * \param interface the interface of the D-Bus object to be called
     * \param methodName the name of the D-Bus method to call
     * \param arguments the arguments of the D-Bus call. Defaults to no arguments.
     * \param parent Parent object
     */
    explicit MRemoteAction(const QString &serviceName, const QString &objectPath, const QString &interface, const QString &methodName, const QList<QVariant> &arguments = QList<QVariant>(), QObject *parent = NULL);

    /*!
     * \brief Constructs a MRemoteAction from a string representation of a D-Bus remote action acquired with toString().
     *
     * \param string the QString to construct the MRemoteAction from
     * \param parent Parent object
     */
    explicit MRemoteAction(const QString &string = QString(), QObject *parent = NULL);

    /*!
     * \brief Constructs a copy of another MRemoteAction.
     *
     * \param action the MRemoteAction to copy
     */
    MRemoteAction(const MRemoteAction &action);

    /*!
     * \brief Destroys the MRemoteAction.
     */
    virtual ~MRemoteAction();

    /*!
     * Returns a string representation of this remote action.
     *
     * \return a string representation of this remote action
     */
    QString toString() const;

protected Q_SLOTS:
    /*!
     * \brief A slot for calling the D-Bus function when the action is triggered
     */
    void call();

protected:
    /*!
     * \brief Initializes the MRemoteAction from a string representation
     *
     * \param string a string representation of a remote action
     */
    void fromString(const QString &string);

    //! \internal
    MRemoteAction(MRemoteActionPrivate &dd, QObject *parent = NULL);
    //! \internal_end

private:
    Q_DECLARE_PRIVATE(MRemoteAction)
};

#endif /* MREMOTEACTION_H_ */
