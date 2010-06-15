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

#ifndef FAKEINSTALLATIONSOURCE_H
#define FAKEINSTALLATIONSOURCE_H

#include <mappletinstallationsourceinterface.h>
#include <QString>
#include <MWidget>

class InstallationSourceWidget;

class FakeInstallationSource : public QObject, public MAppletInstallationSourceInterface
{
    Q_OBJECT
    Q_INTERFACES(MAppletInstallationSourceInterface MApplicationExtensionInterface)

public:

    FakeInstallationSource();
    virtual ~FakeInstallationSource();
    //! Methods derived from MApplicationExtensionInterface
    virtual bool initialize(const QString&);
    virtual QGraphicsWidget *widget();

    //! Methods derived from MAppletInstallationSourceInterface
    void setMAppletInventoryInterface(MAppletInventoryInterface &installationSource) ;

    //! For accessing applet inventory interface
    MAppletInventoryInterface *appletInventoryInterface() const;
private:
    InstallationSourceWidget *source;
    MAppletInventoryInterface *appletInventory;
};

class InstallationSourceWidget : public MWidget
{
    Q_OBJECT
    Q_PROPERTY(QString installationSourceTitle READ installationSourceTitle)
    Q_PROPERTY(QString installationSourceText READ installationSourceText)
    Q_PROPERTY(QString installationSourceIcon READ installationSourceIcon)

public:
    InstallationSourceWidget(FakeInstallationSource &source);

    QString installationSourceTitle() {
        return "Applet Store";
    };
    QString installationSourceText() {
        return "42";
    };
    QString installationSourceIcon() {
        return "icon-s-alarm";
    };

private slots:

    /*!
      * Called to instantiate an applet from a package
      *
      * \param packageName the name of the applet package to be instantiated
      */
    void instantiateAppletsInPackage(const QString &packageName);

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
    //! For accessing installation source
    FakeInstallationSource &installationSource;
};

#endif // FAKEINSTALLATIONSOURCE_H
