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

#ifndef MAPPLICATIONSERVICE_H
#define MAPPLICATIONSERVICE_H

#include <QObject>

#include "mexport.h"

class MApplicationServicePrivate;

/*!
 * \class MApplicationService
 * \brief Adaptor class for interface com.nokia.MApplicationIf
 *
 * This is the implementation for the MApplication QDBus service.
 * Derive from this class to implement alternative behaviour.
 *
 * If multiple processes are required, override the launch() method and
 * from there call launchAnotherWithQProcess().
 *
 * If it is desired for the subsequent processes to also have a
 * MApplicationIf, then override handleServiceRegistrationFailure()
 * and from there call incrementAndRegister().
 *
 * If it is desired that there be no MApplicationIf interface, override
 * registeredName() so that it returns QString(), isRegistered() to
 * return false, and registerService() to return true.
 *
 * Here is a code sample that allows multiple instances, each with its own
 * DBus service :
 *  \dontinclude multipleinstances/main.cpp
 *  \skip class MyApplicationService
 *  \until MApplication app
 *
 * Tip: You can use qdbusviewer from the qt4-dev-tools package to see the
 * services appearing and disappearing when you run/kill multiple instances.
 *
 */
class M_CORE_EXPORT MApplicationService : public QObject
{
    Q_OBJECT

public:
    MApplicationService(const QString &serviceName, QObject *parent = 0);
    virtual ~MApplicationService();

public Q_SLOTS: // METHODS
    /**
     * \brief Launch the application.
     *
     * \return void
     * By default, this method will raise the application's window
     *
     * If it is required that it launch another instance of this
     * application, derive a class from MApplicationService and override
     * launch() to call launchAnotherWithQProcess().
     **/
    virtual void launch();

    /**
     * \brief Launch the application with parameters.
     *
     * \return void
     * By default, this method will ignore the parameters and call launch()
     *
     * If you want to process the parameters, derive a class from
     * MApplicationService and override launch(const QStringList&).
     *
     * This function is called when there is already an instance
     * of this program running and registered to the DBus, and
     * a second instance of this application gets executed. It will
     * then try to register the same DBus interface, and fail at it.
     * In this case when the second application was invoked with
     * command line arguments, it will call this launch function
     * with arguments on the first application instance via DBus.
     * In this case no command line parameters are removed by
     * libmeegotouch. The parameter list does not contain the
     * executable name (argv[0]), but only the command line parameters.
     **/
    virtual void launch(const QStringList& parameters);

    /**
     * \brief Close the GUI.
     *
     * \return void
     **/
    virtual void close();

    /**
     * \brief Exit the application.
     *
     * \return void
     **/
    virtual void exit();

    /**
     * \brief launch another process using QProcess
     *
     * \return void
     **/
    virtual void launchAnotherWithQProcess();

    /**
     * \brief Handles the situation when a service is already registered with this application's name.
     *
     * \return void
     *
     * This method implements the default behaviour when there is already
     * a service registrated with our service name, which is to call
     * 'launch()' on that service and then exit.
     * This method can be overridden and alternative behaviour
     * implemented. One alternative behaviour is to register with another
     * name and this is implemented in incrementAndRegister() which can
     * be called from an overridden handleServiceRegistrationFailure().
     **/
    virtual void handleServiceRegistrationFailure();

    /**
     * \brief increment service name and register
     *
     * \return void
     *
     * It is intended that this method be called from within an overridden handleServiceRegistrationFailure() method.
     * The behaviour is to add an integer to the name of the service being registered until registration succeeds, or the maximum number of attempts (10) is exceeded.
     **/
    virtual void incrementAndRegister();

    /**
     * \brief return the registered service name
     *
     * \return registered service name
     *
     * If you want no MApplicationIf, override this to return QString().
     **/
    virtual QString registeredName();

    /**
     * \brief return if the service is registered
     *
     * \return true if the service is registered
     *
     * If you want no MApplicationIf, override this to always return false.
     **/
    virtual bool isRegistered();

    /**
     * \brief registers the service
     *
     * \return true is registration was successful
     *
     * If you want no MApplicationIf, override this to always return true.
     **/
    virtual bool registerService();

    /**
     * \brief sets the service name
     *
     * \return void
     **/
    void setServiceName(const QString &serviceName);

protected:
    MApplicationServicePrivate *const d_ptr;

private:
    Q_DECLARE_PRIVATE(MApplicationService)
    Q_DISABLE_COPY(MApplicationService)
};

#endif
