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

#ifndef FAKEINSTALLATIONSOURCE_H
#define FAKEINSTALLATIONSOURCE_H

#include <duiappletinstallationsourceinterface.h>
#include <QString>
#include <DuiWidget>

class FakeInstallationSource : public QObject, public DuiAppletInstallationSourceInterface
{
    Q_OBJECT
    Q_INTERFACES(DuiAppletInstallationSourceInterface)

public:
    virtual DuiWidget *constructWidget();

private:
};

class InstallationSourceWidget : public DuiWidget
{
    Q_OBJECT
    Q_PROPERTY(QString installationSourceTitle READ installationSourceTitle)
    Q_PROPERTY(QString installationSourceText READ installationSourceText)
    Q_PROPERTY(QString installationSourceIcon READ installationSourceIcon)

public:
    InstallationSourceWidget();

    QString installationSourceTitle() {
        return "Applet Store";
    };
    QString installationSourceText() {
        return "42";
    };
    QString installationSourceIcon() {
        return "icon-s-alarm";
    };

signals:
    /*!
      * Called to instantiate an applet from a package
      *
      * \param packageName the name of the applet package to be instantiated
      */
    void packageSelectedForInstallation(const QString &packageName);

private slots:
    /*!
     * Installs an applet from a local debian package. The package name is fetched
     * from an environment variable called FAKEAPPLETINSTALLATIONSOURCEPACKAGE.
     */
    void installApplet();

    /*!
     * Installs an applet from a package called fakeappletpackage-installable.deb
     */
    void installableApplet();

    /*!
     * Installs an applet from a package called fakeappletpackage-uninstallable.deb
     */
    void uninstallableApplet();

    /*!
     * Installs an applet from a package called fakeappletpackage-notdownloadable.deb
     */
    void notDownloadableApplet();

    /*!
     * Installs an applet from a package called fakeappletpackage-metadataonly.deb
     */
    void metadataOnly();

    /*!
     * Installs an applet from a package called fakeappletpackage-nonexistent.deb
     */
    void nonExistent();

private:
    /*!
     * Installs an applet package.
     * \param packageName the name of the package to install
     */
    void installAppletPackage(const QString &packageName);
};

#endif // FAKEINSTALLATIONSOURCE_H
